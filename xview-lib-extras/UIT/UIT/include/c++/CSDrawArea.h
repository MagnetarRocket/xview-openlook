/*H+ CSDrawArea.h
*
*    SCCS ID: @(#)CSDrawArea.h 1.1 93/02/08 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSDrawArea class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSDrawArea
*
*    DESCRIPTION:
*
*    The CSDrawArea component provides the ability to draw on
*    a CanvasShellDisplay.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSDrawArea object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setLeftX (double)
*    void setRightX (double)
*    void setUpperY (double)
*    void setLowerY (double) - Sets the dimensions of the CSDrawArea.
*
*    double getLeftX ()
*    double getRightX ()
*    double getUpperY ()
*    double getLowerY () - Gets the dimensions of the CSDrawArea.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    DrawArea encapsulated by this object.
*
*    void setCSDrawAreaAttributes (UIObject *) - Sets XView attributes.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSDrawAreaDefaults () - Initializes data members.
*
*/

#ifndef _CS_DRAW_AREA_H
#define _CS_DRAW_AREA_H

// Includes
#include "CSComponent.h"

#include <sspkg/drawobj.h>

// Class Definition
class CSDrawArea : public CSComponent {
public:
  CSDrawArea ();
  ~CSDrawArea ();

  void setLeftX (double x)
    {
      leftX = x;

      if (owObject)
        xv_set (owObject, DRAWAREA_LEFT_X, &leftX, NULL);
    }

  void setRightX (double x)
    {
      rightX = x;

      if (owObject)
        xv_set (owObject, DRAWAREA_RIGHT_X, &rightX, NULL);
    }

  void setUpperY (double y)
    {
      upperY = y;

      if (owObject)
        xv_set (owObject, DRAWAREA_UPPER_Y, &upperY, NULL);
    }

  void setLowerY (double y)
    {
      lowerY = y;

      if (owObject)
        xv_set (owObject, DRAWAREA_LOWER_Y, &lowerY, NULL);
    }

  double getLeftX ()
    {
      if (owObject)
	leftX = *(double *) xv_get (owObject, DRAWAREA_LEFT_X);

      return leftX;
    }

  double getRightX ()
    {
      if (owObject)
	rightX = *(double *) xv_get (owObject, DRAWAREA_RIGHT_X);

      return rightX;
    }

  double getUpperY ()
    {
      if (owObject)
	upperY = *(double *) xv_get (owObject, DRAWAREA_UPPER_Y);

      return upperY;
    }

  double getLowerY ()
    {
      if (owObject)
	lowerY = *(double *) xv_get (owObject, DRAWAREA_LOWER_Y);

      return lowerY;
    }

protected:
  double leftX;
  double rightX;
  double upperY;
  double lowerY;

  virtual void createObject (UIObject *parent);
  void         setCSDrawAreaAttributes (UIObject *parent);

private:    
  void setCSDrawAreaDefaults ();
};

#endif /* _CS_DRAW_AREA_H */
