/*H+ CSGrip.h
*
*    SCCS ID: @(#)CSGrip.h 1.3 93/06/17 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSGrip class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSGrip
*
*    DESCRIPTION:
*
*    The CSGrip renders a graphical Grip.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSGrip object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    int getButtonDownX () - The X coordinate of the button was press that
*    initiated the grip move.
*
*    int getButtonDownY () - The Y coordinate of the button was press that
*    initiated the grip move.
*
*    void setExceedParentDimensions (Boolean flag) - If TRUE, then a
*    grip move can exceed the dimensions of its parent.
*
*    void setGripImmediate (Boolean flag) - If TRUE, a grip move can
*    grabbed immediately regardless of what the "SlingShot.ClickMoveThreshold"
*    X resouce is set to.
*
*    void setMinX (int x)
*    void setMinY (int y)
*    void setMaxX (int x)
*    void setMaxY (int y) - Sets minimum and maximum dimensions that
*    the grip can be moved to.
*
*    void setSlideX (Boolean flag) - Specifies that the grip can be
*    moved in the X direction.
*
*    void setSlideY (Boolean flag) - Specifies that the grip can be
*    moved in the Y direction.
*
*    int getMoveX ()
*    int getMoveY () - Can be called from inside a move or done handler
*    to determine the position the grip is being moved to.
*
*    void setNewX (int)
*    void setNewY (int) - Can be called from inside a move handler to
*    reposition the grip during a move operation.
*
*    void abortMove () - If called inside of a move handler, the object
*    is not moved to the location that the move handler reports as the
*    X and Y location the grip is being moved to.
*
*    void setDoneHandler (UICHandlerFunction) - Sets the done handler.
*    This is called when a move operation has been completed.
*
*    void setMoveHandler (UICHandlerFunction) - Sets the move handler.
*    This is called for each move event.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    Grip encapsulated by this object.
*
*    void         setCSGripAttributes (UIObject *) - Sets the
*    CSGrip attributes.
*
*    static int   doneProc (Xv_window,
*                           Event *,
*                           Canvas_shell,
*                           Grip,
*                           short,
*                           short) - GRIP done proc wrapper.
*
*    static int   moveProc (Xv_window,
*                           Event *,
*                           Canvas_shell,
*                           Grip,
*                           short *,
*                           short *) - GRIP move proc wrapper.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSGripDefaults () - Initializes data members.
*
*/

#ifndef _CS_GRIP_H
#define _CS_GRIP_H

// Includes
#include "CSDrawImage.h"

#include <sspkg/grip.h>

// Class Definition
class CSGrip : public CSDrawImage {
public:
  CSGrip ();
  ~CSGrip ();

  int getButtonDownX ()
    {
      return (owObject ?
	      (int) xv_get (owObject, GRIP_BTN_DOWN_X) : UIC_NOT_SET);
    }

  int getButtonDownY ()
    {
      return (owObject ?
	      (int) xv_get (owObject, GRIP_BTN_DOWN_Y) : UIC_NOT_SET);
    }

  void setExceedParentDimensions (Boolean flag)
    {
      exceedParentDimensions = flag;
      exceedParentDimensionsSet = TRUE;

      if (owObject)
	xv_set (owObject, GRIP_EXCEED_PARENT_DIMS, flag, NULL);
    }

  void setGripImmediate (Boolean flag)
    {
      gripImmediate = flag;
      gripImmediateSet = TRUE;

      if (owObject)
	xv_set (owObject, GRIP_IMMEDIATE, flag, NULL);
    }

  void setMinX (int x)
    {
      minX = x;

      if (owObject)
	xv_set (owObject, GRIP_MIN_X, minX, NULL);
    }

  void setMinY (int y)
    {
      minY = y;

      if (owObject)
	xv_set (owObject, GRIP_MIN_Y, minY, NULL);
    }

  void setMaxX (int x)
    {
      maxX = x;

      if (owObject)
	xv_set (owObject, GRIP_MAX_X, maxX, NULL);
    }

  void setMaxY (int y)
    {
      maxY = y;

      if (owObject)
	xv_set (owObject, GRIP_MAX_Y, maxY, NULL);
    }

  void setRubberStyle (Grip_rubber_style style)
    {
      rubberStyle = style;

      if (owObject)
	xv_set (owObject, GRIP_RUBBER_STYLE, rubberStyle, NULL);
    }

  void setSlideX (Boolean flag)
    {
      slideX = flag;
      slideXSet = TRUE;

      if (owObject)
	xv_set (owObject, GRIP_SLIDE_X, flag, NULL);
    }

  void setSlideY (Boolean flag)
    {
      slideY = flag;
      slideYSet = TRUE;

      if (owObject)
	xv_set (owObject, GRIP_SLIDE_Y, flag, NULL);
    }

   int getMoveX ()
     {
       return moveX;
     }

  int getMoveY ()
    {
      return moveY;
    }

  void setNewX (int x)
    {
      newX = x;
    }

  void setNewY (int y)
    {
      newY = y;
    }

  void abortMove ()
    {
      abortMoveFlag = TRUE;
    }

  void setDoneHandler (UICHandlerFunction handler)
    {
      doneHandler = handler;

      if (owObject && !doneHandlerSet) {
        doneHandlerSet = TRUE;
 
        xv_set (owObject,
                GRIP_DONE_PROC, &CSGrip::doneProc,
                NULL);
      } 
    }

  void setMoveHandler (UICHandlerFunction handler)
    {
      moveHandler = handler;
 
      if (owObject && !moveHandlerSet) { 
        moveHandlerSet = TRUE; 
 
        xv_set (owObject, 
                GRIP_MOVE_PROC, &CSGrip::moveProc, 
                NULL); 
      } 
    }

protected:
  Boolean            exceedParentDimensions;
  Boolean            exceedParentDimensionsSet;
  Boolean            gripImmediate;
  Boolean            gripImmediateSet;
  int                minX;
  int                minY;
  int                maxX;
  int                maxY;
  Grip_rubber_style  rubberStyle;
  Boolean            slideX;
  Boolean            slideXSet;
  Boolean            slideY;
  Boolean            slideYSet;
  int                moveX;
  int                moveY;
  int                newX;
  int                newY;
  Boolean            abortMoveFlag;
  UICHandlerFunction doneHandler;
  Boolean	     doneHandlerSet;
  UICHandlerFunction moveHandler;
  Boolean	     moveHandlerSet;

  virtual void createObject (UIObject *parent);
  void         setCSGripAttributes (UIObject *parent);

#ifdef __GNUC__
public:
#endif
  static int   doneProc (Xv_window,
                         Event *,
                         Canvas_shell,
                         Grip,
                         short,
                         short);

  static int   moveProc (Xv_window,
                         Event *,
                         Canvas_shell,
                         Grip,
                         short *,
                         short *);

private:    
  void setCSGripDefaults ();
};

#endif /* _CS_GRIP_H */
