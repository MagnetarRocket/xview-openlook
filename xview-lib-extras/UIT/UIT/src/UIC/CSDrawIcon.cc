/*H+ CSDrawIcon.cc
*
*    SCCS ID: @(#)CSDrawIcon.cc 1.2 93/04/09 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSDrawIcon class.
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
static char sccsid [] = "@(#)CSDrawIcon.cc 1.2 93/04/09 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CanvasShellDisplay.h"
#include "CSDrawIcon.h"

/*F+ CSDrawIcon::CSDrawIcon ()
*
*    PURPOSE:
*
*    Constructor of the CSDrawIcon class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawIcon::CSDrawIcon ()
{
  setCSDrawIconDefaults ();
}

/*F+ CSDrawIcon::setCSDrawIconDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSDrawIcon class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawIcon::setCSDrawIconDefaults ()
{
  gap = UIC_NOT_SET;
  imageColor = UIC_NOT_SET;
  textColor = UIC_NOT_SET;
  layout = (Orientation) UIC_NOT_SET;
  drawImage = new CSDrawImage;
  drawText = new CSDrawText;
  selectedImage = (Image *) NULL;
  selectedImageMask = (Image *) NULL;
  notSelectedImage = (Image *) NULL;
  notSelectedImageMask = (Image *) NULL;
  displayLength = UIC_NOT_SET;
  editable = FALSE;
  editableSet = FALSE;
  justification = (Justification) UIC_NOT_SET;
  stringValue = (char *) NULL;
  underline = FALSE;
  underlineSet = FALSE;
  isDrawObject = TRUE;
}

/*F+ CSDrawIcon::~CSDrawIcon ()
*
*    PURPOSE:
*
*    Destructor of the CSDrawIcon class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawIcon::~CSDrawIcon ()
{
  if (drawImage) {
    drawImage->owObject = (UICGeneric) NULL;
    delete drawImage;
  }

  if (drawText) {
    drawText->owObject = (UICGeneric) NULL;
    delete drawText;
  }
}

/*F+ char *CSDrawIcon::getValue (char* &value)
*
*    PURPOSE:
*
*    Gets the current value of the object.
*
*    PARAMETERS:
*
*    value - The value of the text.
*
*    RETURN VALUES:
*
*    The value of the object is returned.  The string must
*    be freed by the caller.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *CSDrawIcon::getValue (char* &value)
{
  value = (char *) NULL;

  if (owObject) {
    char *temp_string = (char *) xv_get (owObject, DRAWTEXT_STRING);

    if (temp_string)
      value = strcpy (new char [strlen (temp_string)+1], temp_string);
  }
  else if (stringValue) {
    value = strcpy (new char [strlen (stringValue)+1], stringValue);
  }

  return value;
}

/*F+ void CSDrawIcon::setValue (const char *value)
*
*    PURPOSE:
*
*    Gets the current value of the object.
*
*    PARAMETERS:
*
*    value - The value of the text.
*
*    RETURN VALUES:
*
*    The value of the object is returned.  The string must
*    be freed by the caller.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawIcon::setValue (const char *value)
{
  if (owObject) {
    xv_set (owObject, DRAWTEXT_STRING, value, NULL);
  }
  else {
    if (stringValue) delete stringValue;

    stringValue = (value ?
		   strcpy (new char [strlen (value)+1], value) :
		   (char *) NULL);
  }
}

/*F+ void CSDrawIcon::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSDrawIcon object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawIcon::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), DRAWICON, NULL);

  if (gap != UIC_NOT_SET)
    xv_set (owObject, DRAWICON_GAP, gap, NULL);

  if (layout != UIC_NOT_SET)
    xv_set (owObject,
	    DRAWICON_LAYOUT_VERTICAL, ((layout == VERTICAL) ?
				       TRUE : FALSE),
	    NULL);

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

  if (imageColor != UIC_NOT_SET) {
    initializeColorMap (serverParent);
    xv_set (owObject, DRAWICON_IMAGE_COLOR, imageColor, NULL);
  }
 
  if (stringValue)
    xv_set (owObject, DRAWTEXT_STRING, stringValue, NULL);

  if (displayLength != UIC_NOT_SET)
    xv_set (owObject, DRAWTEXT_LENGTH, displayLength, NULL);

  if (editableSet)
    xv_set (owObject, DRAWTEXT_EDITABLE, editable, NULL);

  if (justification == JUSTIFY_LEFT)
    xv_set (owObject, DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_LEFT, NULL);
  else if (justification == JUSTIFY_CENTER)
    xv_set (owObject, DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_CENTER, NULL);
  else if (justification == JUSTIFY_RIGHT)
    xv_set (owObject, DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_RIGHT, NULL);

  if (underlineSet)
    xv_set (owObject, DRAWTEXT_SHOW_UNDERLINE, underline, NULL);

  if (textColor != UIC_NOT_SET) {
    initializeColorMap (serverParent);
    xv_set (owObject, DRAWICON_TEXT_COLOR, textColor, NULL);
  }
 
  drawImage->owObject = (UICGeneric) xv_get (owObject, DRAWICON_IMAGE);
  if (drawImage->owObject)
    drawImage->setCSComponentAttributes (parent);
 
  drawText->owObject = (UICGeneric) xv_get (owObject, DRAWICON_TEXT);
  if (drawText->owObject)
    drawText->setCSComponentAttributes (parent);
 
  setCSComponentAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
