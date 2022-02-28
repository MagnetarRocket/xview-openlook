/*H+ CSGrip.cc
*
*    SCCS ID: @(#)CSGrip.cc 1.3 93/06/17 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSGrip class.
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
static char sccsid [] = "@(#)CSGrip.cc 1.3 93/06/17 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSGrip.h"

/*F+ CSGrip::CSGrip ()
*
*    PURPOSE:
*
*    Constructor of the CSGrip class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSGrip::CSGrip ()
{
  setCSGripDefaults ();
}

/*F+ CSGrip::setCSGripDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSGrip class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSGrip::setCSGripDefaults ()
{
  exceedParentDimensions = FALSE;
  exceedParentDimensionsSet = FALSE;
  gripImmediate = FALSE;
  gripImmediateSet = FALSE;
  minX = UIC_NOT_SET;
  minY = UIC_NOT_SET;
  maxX = UIC_NOT_SET;
  maxY = UIC_NOT_SET;
  rubberStyle = (Grip_rubber_style) UIC_NOT_SET;
  slideX = FALSE;
  slideXSet = FALSE;
  slideY = FALSE;
  slideYSet = FALSE;
  moveX = UIC_NOT_SET;
  moveY = UIC_NOT_SET;
  newX = UIC_NOT_SET;
  newY = UIC_NOT_SET;
  abortMoveFlag = FALSE;
  doneHandler = (UICHandlerFunction) NULL;
  doneHandlerSet = FALSE;
  moveHandler = (UICHandlerFunction) NULL;
  moveHandlerSet = FALSE;
  isDrawObject = TRUE;
}

/*F+ CSGrip::~CSGrip ()
*
*    PURPOSE:
*
*    Destructor of the CSGrip class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSGrip::~CSGrip ()
{
}

/*F+ int CSGrip::doneProc (Xv_window     window,
*                          Event        *event,
*                          Canvas_shell  cs,
*                          Grip          grip,
*                          short         new_x,
*                          short         new_y)
*
*    PURPOSE:
*
*    Internal XView GRIP done proc wrapper.  This is called whenever an
*    move operation is completed on a CSGrip.
*
*    PARAMETERS:
*
*    window - The paint window
*    event - The XView event
*    cs - The Canvas_shell
*    grip - The XView GRIP
*    new_x - The X position moved to
*    new_y - The Y position moved to
*
*    RETURN VALUES:
*
*    Return TRUE if the move is to be accepted.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int CSGrip::doneProc (Xv_window     window,
                      Event        *event,
                      Canvas_shell  cs,
                      Grip          grip,
                      short         new_x,
                      short         new_y)
{
  CSGrip *object = (CSGrip *) xv_get (grip,
				      XV_KEY_DATA,
				      UIC_OBJECT_KEY);

  object->setEventInformation (event);
  
  object->moveX = new_x;
  object->moveY = new_y;

  if (object->doneHandler)
    (*object->doneHandler) (object);

  object->resetEventInformation ();
  
  return (object->abortMoveFlag ? FALSE : TRUE);
}

/*F+ int CSGrip::moveProc (Xv_window     window,
*                          Event        *event,
*                          Canvas_shell  cs,
*                          Grip          grip,
*                          short        *new_x,
*                          short        *new_y)
*
*    PURPOSE:
*
*    Internal XView GRIP move proc wrapper.  This is called whenever an
*    move operation occurs on a CSGrip.
*
*    PARAMETERS:
*
*    window - The paint window
*    event - The XView event
*    cs - The Canvas_shell
*    grip - The XView GRIP
*    new_x - The X position moved to
*    new_y - The Y position moved to
*
*    RETURN VALUES:
*
*    Return TRUE if the move is to be accepted.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int CSGrip::moveProc (Xv_window     window,
                      Event        *event,
                      Canvas_shell  cs,
                      Grip          grip,
                      short        *new_x,
                      short        *new_y)
{
  CSGrip *object = (CSGrip *) xv_get (grip,
				      XV_KEY_DATA,
				      UIC_OBJECT_KEY);

  object->setEventInformation (event);
  
  object->moveX = *new_x;
  object->moveY = *new_y;

  object->newX = UIC_NOT_SET;
  object->newY = UIC_NOT_SET;

  object->abortMoveFlag = FALSE;
  
  if (object->moveHandler)
    (*object->moveHandler) (object);

  if (object->newX != UIC_NOT_SET)
    *new_x = object->newX;

  if (object->newY != UIC_NOT_SET)
    *new_y = object->newY;

  object->resetEventInformation ();
  
  return (object->abortMoveFlag ? FALSE : TRUE);
}

/*F+ void CSGrip::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSGrip object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSGrip::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), GRIP, NULL);

  setCSGripAttributes (parent);
}

/*F+ void CSGrip::setCSGripAttributes (UIObject *parent)
*
*    PURPOSE:
*
*    Sets common CSGrip XView attributes that are required for creating
*    the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSGrip object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSGrip::setCSGripAttributes (UIObject *parent)
{
  if (exceedParentDimensionsSet)
    xv_set (owObject, GRIP_EXCEED_PARENT_DIMS, exceedParentDimensions, NULL);

  if (gripImmediateSet)
    xv_set (owObject, GRIP_IMMEDIATE, gripImmediate, NULL);

  if (minX != UIC_NOT_SET)
    xv_set (owObject, GRIP_MIN_X, minX, NULL);

  if (minY != UIC_NOT_SET)
    xv_set (owObject, GRIP_MIN_Y, minY, NULL);

  if (maxX != UIC_NOT_SET)
    xv_set (owObject, GRIP_MAX_X, maxX, NULL);

  if (maxY != UIC_NOT_SET)
    xv_set (owObject, GRIP_MAX_Y, maxY, NULL);

  if (rubberStyle != (Grip_rubber_style) UIC_NOT_SET)
    xv_set (owObject, GRIP_RUBBER_STYLE, rubberStyle, NULL);

  if (slideXSet)
    xv_set (owObject, GRIP_SLIDE_X, slideX, NULL);

  if (slideYSet)
    xv_set (owObject, GRIP_SLIDE_Y, slideY, NULL);

  if (doneHandler) {
    doneHandlerSet = TRUE;
    xv_set (owObject, GRIP_DONE_PROC, &CSGrip::doneProc, NULL);
  }

  if (moveHandler) {
    moveHandlerSet = TRUE;
    xv_set (owObject, GRIP_MOVE_PROC, &CSGrip::moveProc, NULL);
  }

  setCSDrawImageAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
