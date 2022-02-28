/*H+ CSDrawRect.h
*
*    SCCS ID: @(#)CSDrawRect.h 1.1 93/02/08 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSDrawRect class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSDrawRect
*
*    DESCRIPTION:
*
*    The CSDrawRect component provides the ability to draw a rectangular
*    boarder around other CSComponents.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSDrawRect object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setBorderDistanceFromXViewRect (int) - Sets the distance of the
*    illustrated border from the objects XV_RECT.
*
*    void setBorderSize (int) - Sets the size of the illustrated border.
*
*    void setBorderSpaceBetweenChildren (int) - Sets the space between
*    the illustrated boarder and children.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    DRAWRECT encapsulated by this object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSDrawRectDefaults () - Initializes data members.
*
*/

#ifndef _CS_DRAW_RECT_H
#define _CS_DRAW_RECT_H

// Includes
#include "CSComponent.h"

#include <sspkg/drawobj.h>

// Class Definition
class CSDrawRect : public CSComponent {
public:
  CSDrawRect ();
  ~CSDrawRect ();

  void setBorderDistanceFromXViewRect (int pixels)
    {
      borderDistanceFromXViewRect = pixels;

      if (owObject)
	xv_set (owObject, DRAWRECT_BORDER1, pixels, NULL);
    }

  void setBorderSize (int pixels)
    {
      borderSize = pixels;

      if (owObject)
	xv_set (owObject, DRAWRECT_BORDER2, pixels, NULL);
    }

  void setBorderSpaceBetweenChildren (int pixels)
    {
      borderSpaceBetweenChildren = pixels;

      if (owObject)
	xv_set (owObject, DRAWRECT_BORDER3, pixels, NULL);
    }

protected:
  int borderDistanceFromXViewRect;
  int borderSize;
  int borderSpaceBetweenChildren;

  virtual void createObject (UIObject *parent);

private:    
  void setCSDrawRectDefaults ();
};

#endif /* _CS_DRAW_RECT_H */
