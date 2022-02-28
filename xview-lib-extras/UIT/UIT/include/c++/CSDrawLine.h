/*H+ CSDrawLine.h
*
*    SCCS ID: @(#)CSDrawLine.h 1.2 93/06/01 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSDrawLine class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSDrawLine
*
*    DESCRIPTION:
*
*    The CSDrawLine component provides the ability to draw a line
*    on a CanvasShellDisplay.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSDrawLine object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setArrowAngle0 (int new_angle)
*    void setArrowAngle1 (int new_angle) 
*		- Set the angle of the tip arrow, new_angle is 
*		  measured in 64ths of a degree for end points 0 and 1.
*
*    void setArrowInsetLength0 (int length) 
*    void setArrowInsetLength1 (int length) 
*		- Set the length of the line that makes up the arrow
*		  for end points 0 and 1.
*
*    void setArrowHeadLength0 (int length) 
*    void setArrowHeadLength1 (int length) 
*		- Set the length of the arrowhead along the cutting edge.
*		  for end points 0 and 1.
*
*    void setArrowStyle0 (Arrow_style style)
*    void setArrowStyle1 (Arrow_style style)
*         - The style of the arrowhead for end points 0 and 1:
*
*        	ARROW_FILLED
*        	ARROW_HOLLOW
*       	ARROW_SIMPLE
*       	ARROW_NONE
*
*    void setX0 (int x) - Set the X coordinate of endpoint 0.
*
*    void setX1 (int x) - Set the X coordinate of endpoint 1.
*
*    void setY0 (int y) - Set the Y coordinate of endpoint 0.
*
*    void setY1 (int y) - Set the Y coordinate of endpoint 1.
*
*    int getX0 () - Get the X coordinate of endpoint 0.
*
*    int getX1 () - Get the X coordinate of endpoint 1.
*
*    int getY0 () - Get the Y coordinate of endpoint 0.
*
*    int getY1 () - Get the Y coordinate of endpoint 1.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    DrawLine encapsulated by this object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSDrawLineDefaults () - Initializes data members.
*
*/

#ifndef _CS_DRAW_LINE_H
#define _CS_DRAW_LINE_H

// Includes
#include "CSComponent.h"

#include <sspkg/drawobj.h>

// Class Definition
class CSDrawLine : public CSComponent {
public:
  CSDrawLine ();
  ~CSDrawLine ();

  void setArrowAngle0 (int angle)
    {
      arrowAngle0 = angle;

      if (owObject)
	xv_set (owObject, DRAWLINE_ARROW_ANGLE, 0, angle, NULL);
    }

  void setArrowAngle1 (int angle)
    {
      arrowAngle1 = angle;

      if (owObject)
	xv_set (owObject, DRAWLINE_ARROW_ANGLE, 1, angle, NULL);
    }

  void setArrowInsetLength0 (int length)
    {
      arrowInsetLength0 = length;

      if (owObject)
	xv_set (owObject, DRAWLINE_ARROW_INSET_LENGTH, 0, length, NULL);
    }

  void setArrowInsetLength1 (int length)
    {
      arrowInsetLength1 = length;

      if (owObject)
	xv_set (owObject, DRAWLINE_ARROW_INSET_LENGTH, 1, length, NULL);
    }

  void setArrowHeadLength0 (int length)
    {
      arrowHeadLength0 = length;

      if (owObject)
	xv_set (owObject, DRAWLINE_ARROW_LENGTH, 0, length, NULL);
    }

  void setArrowHeadLength1 (int length)
    {
      arrowHeadLength1 = length;

      if (owObject)
	xv_set (owObject, DRAWLINE_ARROW_LENGTH, 1, length, NULL);
    }

  void setArrowStyle0 (Arrow_style style)
    {
      arrowStyle0 = style;

      if (owObject)
	xv_set (owObject, DRAWLINE_ARROW_STYLE, 0, style, NULL);
    }

  void setArrowStyle1 (Arrow_style style)
    {
      arrowStyle1 = style;
 
      if (owObject)
        xv_set (owObject, DRAWLINE_ARROW_STYLE, 1, style, NULL);
    }
       
  void setX0 (int x)
    {
      x0 = x;

      if (owObject)
	xv_set (owObject, DRAWLINE_X, 0, x, NULL);
    }

  void setX1 (int x)
    {
      x1 = x;

      if (owObject)
	xv_set (owObject, DRAWLINE_X, 1, x, NULL);
    }

  void setY0 (int y)
    {
      y0 = y;

      if (owObject)
	xv_set (owObject, DRAWLINE_Y, 0, y, NULL);
    }

  void setY1 (int y)
    {
      y1 = y;

      if (owObject)
	xv_set (owObject, DRAWLINE_Y, 1, y, NULL);
    }

  int getX0 ()
    {
      if (owObject)
	x0 = (int) xv_get (owObject, DRAWLINE_X, 0);

      return x0;
    }

  int getX1 ()
    {
      if (owObject)
	x1 = (int) xv_get (owObject, DRAWLINE_X, 1);

      return x1;
    }

  int getY0 ()
    {
      if (owObject)
	y0 = (int) xv_get (owObject, DRAWLINE_Y, 0);

      return y0;
    }

  int getY1 ()
    {
      if (owObject)
	y1 = (int) xv_get (owObject, DRAWLINE_Y, 1);

      return y1;
    }

protected:
  int         arrowAngle0;
  int         arrowAngle1;
  int         arrowInsetLength0;
  int         arrowInsetLength1;
  int         arrowHeadLength0;
  int         arrowHeadLength1;
  Arrow_style arrowStyle0;
  Arrow_style arrowStyle1;
  int         x0;
  int         y0;
  int         x1;
  int         y1;

  virtual void createObject (UIObject *parent);

private:    
  void setCSDrawLineDefaults ();
};

#endif /* _CS_DRAW_LINE_H */
