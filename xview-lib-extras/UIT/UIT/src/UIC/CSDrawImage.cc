/*H+ CSDrawImage.cc
*
*    SCCS ID: @(#)CSDrawImage.cc 1.2 93/04/09 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSDrawImage class.
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
static char sccsid [] = "@(#)CSDrawImage.cc 1.2 93/04/09 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CanvasShellDisplay.h"
#include "CSDrawImage.h"

/*F+ CSDrawImage::CSDrawImage ()
*
*    PURPOSE:
*
*    Constructor of the CSDrawImage class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawImage::CSDrawImage ()
{
  setCSDrawImageDefaults ();
}

/*F+ CSDrawImage::setCSDrawImageDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSDrawImage class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawImage::setCSDrawImageDefaults ()
{
  selectedImage = (Image *) NULL;
  selectedImageMask = (Image *) NULL;
  notSelectedImage = (Image *) NULL;
  notSelectedImageMask = (Image *) NULL;
  isDrawObject = TRUE;
}

/*F+ CSDrawImage::~CSDrawImage ()
*
*    PURPOSE:
*
*    Destructor of the CSDrawImage class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawImage::~CSDrawImage ()
{
}

/*F+ void CSDrawImage::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSDrawImage object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawImage::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), DRAWIMAGE, NULL);

  setCSDrawImageAttributes (parent);
}

/*F+ void CSDrawImage::setCSDrawImageAttributes (UIObject *parent)
*
*    PURPOSE:
*
*    Sets common CSDrawImage XView attributes that are required for creating
*    the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSDrawImage object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawImage::setCSDrawImageAttributes (UIObject *parent)
{
  if (selectedImage) {
    ((UIObject *) selectedImage)->createObject (canvasShellDisplay);
    xv_set (owObject,
	    DRAWIMAGE_IMAGE2, selectedImage->getXViewObject (),
	    NULL);
  }

  if (selectedImageMask) {
    ((UIObject *) selectedImageMask)->createObject (canvasShellDisplay);
    xv_set (owObject,
	    DRAWIMAGE_IMAGE2_MASK, selectedImageMask->getXViewObject (),
	    NULL);
  }

  if (notSelectedImage) {
    ((UIObject *) notSelectedImage)->createObject (canvasShellDisplay);
    xv_set (owObject,
	    DRAWIMAGE_IMAGE1, notSelectedImage->getXViewObject (),
	    NULL);
  }

  if (notSelectedImageMask) {
    ((UIObject *) notSelectedImageMask)->createObject (canvasShellDisplay);
    xv_set (owObject,
	    DRAWIMAGE_IMAGE1_MASK, notSelectedImageMask->getXViewObject (),
	    NULL);
  }

  setCSComponentAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
