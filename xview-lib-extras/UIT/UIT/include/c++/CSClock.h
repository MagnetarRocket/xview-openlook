/*H+ CSClock.h
*
*    SCCS ID: @(#)CSClock.h 1.2 93/04/09 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSClock class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSClock
*
*    DESCRIPTION:
*
*    The CSClock renders a graphical clock.
*
*    INHERITED CLASSES:
*
*    CSDrawArea - A CSClock object is a CSDrawArea.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setHour (int) - Sets the clock's hour hand.
*
*    int getHour () - Gets the position of the hour hand.
*
*    void setMinute (int) - Sets the clock's minute hand.
*
*    int getMinute () - Gets the position of the minute hand.
*
*    MoveStatus getMoveStatus () - Should only be called from inside of
*    a move handler.  It returns the status of a move operation:
*
*           MOVE_NO_MOVE - No move operation in progress
*           MOVE_START - Move operation started
*           MOVE_IN_PROGRESS - Move operation in process
*           MOVE_END - Move operation completed
*
*    void setMoveHandler (UICHandlerFunction) - Sets the move handler.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    Clock encapsulated by this object.
*
*    static  void moveProc (Clockobj, int, int, int) - The XView
*    CLOCKOBJ move proc wrapper.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSClockDefaults () - Initializes data members.
*
*/

#ifndef _CS_CLOCK_H
#define _CS_CLOCK_H

// Includes
#include "CSDrawArea.h"

#include <sspkg/drawobj.h>

// Class Definition
class CSClock : public CSDrawArea {
public:
  CSClock ();
  ~CSClock ();

  void setHour (int new_hour)
    {
      hour = new_hour;

      if (owObject)
	xv_set (owObject, CLOCKOBJ_HR, hour, NULL);
    }

  int getHour ()
    {
      if (owObject)
	hour = (int) xv_get (owObject, CLOCKOBJ_HR);

      return hour;
    }

  void setMinute (int new_minute)
    {
      hour = new_minute;

      if (owObject)
	xv_set (owObject, CLOCKOBJ_MIN, minute, NULL);
    }

  int getMinute ()
    {
      if (owObject)
	minute = (int) xv_get (owObject, CLOCKOBJ_MIN);

      return minute;
    }

  void setMovable (Boolean flag)
    {
      movable = flag;
      movableSet = TRUE;

      if (owObject)
	xv_set (owObject, CLOCKOBJ_MOVABLE, flag, NULL);
    }

  MoveStatus getMoveStatus ()
    {
      return moveStatus;
    }

  void setMoveHandler (UICHandlerFunction handler)
    {
      moveHandler = handler;

      if (owObject && !moveHandlerSet) {
        moveHandlerSet = TRUE;

	xv_set (owObject,
		CLOCKOBJ_MOVE_PROC, &CSClock::moveProc,
		NULL);
      }
    }

protected:
  int                hour;
  int                minute;
  Boolean            movable;
  Boolean            movableSet;
  MoveStatus         moveStatus;
  UICHandlerFunction moveHandler;
  Boolean	     moveHandlerSet;

  virtual void createObject (UIObject *parent);

#ifdef __GNUC__
public:
#endif
  static  void moveProc (Clockobj, int, int, int);

private:    
  void setCSClockDefaults ();
};

#endif /* _CS_CLOCK_H */
