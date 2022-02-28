/*H+ CSDrawImage.h
*
*    SCCS ID: @(#)CSDrawImage.h 1.1 93/02/08 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSDrawImage class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSDrawImage
*
*    DESCRIPTION:
*
*    The CSDrawImage component provides the ability to draw an image on
*    a CanvasShellDisplay.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSDrawImage object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setSelectedImage (Image &) - The image to display when selected.
*
*    void setSelectedImageMask (Image &) - The image mask when selected.
*
*    void setNotSelectedImage (Image &) - The image to display when
*    not selected.
*
*    void setNotSelectedImageMask (Image &) - The image mask when not
*    selected.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    DrawImage encapsulated by this object.
*
*    void         setCSDrawImageAttributes (UIObject *) - Sets attribute
*    common to classes derived from DrawImage.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSDrawImageDefaults () - Initializes data members.
*
*/

#ifndef _CS_DRAW_IMAGE_H
#define _CS_DRAW_IMAGE_H

// Includes
#include "Image.h"
#include "CSComponent.h"

#include <sspkg/drawobj.h>

// Class Definition
class CSDrawImage : public CSComponent {
public:
  CSDrawImage ();
  ~CSDrawImage ();

  void setSelectedImage (Image &image)
    {
      selectedImage = &image;

      if (owObject) {
        ((UIObject *) &image)->createObject (parentObject);
	xv_set (owObject, DRAWIMAGE_IMAGE2, image.getXViewObject (), NULL);
      }
    }

  void setSelectedImageMask (Image &image)
    {
      selectedImageMask = &image;

      if (owObject) {
        ((UIObject *) &image)->createObject (parentObject);
	xv_set (owObject,
		DRAWIMAGE_IMAGE2_MASK, image.getXViewObject (),
		NULL);
      }
    }

  void setNotSelectedImage (Image &image)
    {
      notSelectedImage = &image;

      if (owObject) {
        ((UIObject *) &image)->createObject (parentObject);
	xv_set (owObject, DRAWIMAGE_IMAGE1, image.getXViewObject (), NULL);
      }
    }

  void setNotSelectedImageMask (Image &image)
    {
      notSelectedImageMask = &image;

      if (owObject) {
        ((UIObject *) &image)->createObject (parentObject);
	xv_set (owObject,
		DRAWIMAGE_IMAGE1_MASK, image.getXViewObject (),
		NULL);
      }
    }

protected:
  Image *selectedImage;
  Image *selectedImageMask;
  Image *notSelectedImage;
  Image *notSelectedImageMask;

  virtual void createObject (UIObject *parent);
  void         setCSDrawImageAttributes (UIObject *);

private:    
  void setCSDrawImageDefaults ();

friend class CSDrawIcon;
};

#endif /* _CS_DRAW_IMAGE_H */
