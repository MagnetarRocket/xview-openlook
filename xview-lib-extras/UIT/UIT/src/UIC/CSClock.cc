/*H+ CSClock.cc
*
*    SCCS ID: @(#)CSClock.cc 1.2 93/04/09 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSClock class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)CSClock.cc 1.2 93/04/09 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSClock.h"

/*F+ CSClock::CSClock ()
*
*    PURPOSE:
*
*    Constructor of the CSClock class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSClock::CSClock ()
{
  setCSClockDefaults ();
}

/*F+ CSClock::setCSClockDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSClock class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSClock::setCSClockDefaults ()
{
  hour = UIC_NOT_SET;
  minute = UIC_NOT_SET;
  movable = FALSE;
  movableSet = FALSE;
  moveStatus = MOVE_NO_MOVE;
  moveHandler = (UICHandlerFunction) NULL;
  moveHandlerSet = FALSE;
  isDrawObject = TRUE;
}

/*F+ CSClock::~CSClock ()
*
*    PURPOSE:
*
*    Destructor of the CSClock class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSClock::~CSClock ()
{
}

/*F+ void CSClock::moveProc (Clockobj clock,
*                            int      hour,
*                            int      minute,
*                            int      done)
*
*    PURPOSE:
*
*    Internal XView CLOCKOBJ move proc wrapper.  This is called whenever an
*    move operation occurs on a CSClock.
*
*    PARAMETERS:
*
*    clock - The XView CLOCKOBJ
*    hour - The new hour
*    minute - The new minute
*    done - Set to TRUE if the move operation is completed
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSClock::moveProc (Clockobj clock,
                        int      hour,
                        int      minute,
                        int      done)
{
  CSClock *object = (CSClock *) xv_get (clock,
					XV_KEY_DATA,
					UIC_OBJECT_KEY);

  if (!done) {
    if (object->moveStatus == MOVE_NO_MOVE)
      object->moveStatus = MOVE_START;
    else
      object->moveStatus = MOVE_IN_PROGRESS;
  }
  else
    object->moveStatus = MOVE_END;
  
  if (object->moveHandler)
    (*object->moveHandler) (object);
}

/*F+ void CSClock::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSClock object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSClock::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), CLOCKOBJ, NULL);

  if (hour != UIC_NOT_SET)
    xv_set (owObject, CLOCKOBJ_HR, hour, NULL);

  if (minute != UIC_NOT_SET)
    xv_set (owObject, CLOCKOBJ_MIN, minute, NULL);

  if (movableSet)
    xv_set (owObject, CLOCKOBJ_MOVABLE, movable, NULL);

  if (moveHandler) {
    moveHandlerSet = TRUE;
    xv_set (owObject, CLOCKOBJ_MOVE_PROC, &CSClock::moveProc, NULL);
  }

  setCSDrawAreaAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
