/*H+ CSDrawIcon.h
*
*    SCCS ID: @(#)CSDrawIcon.h 1.2 93/04/09 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSDrawIcon class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSDrawIcon
*
*    DESCRIPTION:
*
*    The CSDrawIcon component provides the ability to draw an image/text
*    icon on a CanvasShellDisplay.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSDrawIcon object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setGap (int) - The gap (in pixels) between the image and text on
*    the icon.
*
*    void setImageColor (Color)
*    void setImageColor (const char *) - Set the foreground color of
*    the icon's image.
*
*    void setTextColor (Color)
*    void setTextColor (const char *) - Set the foreground color of
*    the icon's text.
*
*    void setLayout (Orientation) - Specify the layout of the icon:
*
*         VERTICAL
*         HORIZONTAL
*
*    CSDrawImage *getCSDrawImage () - Get the CSDrawImage object that is
*    used internally for the icon's image.  The caller must never
*    delete the object that is returned.
*
*    CSDrawText *getCSDrawText () - Get the CSDrawText object that is
*    used internally for the icon's text.  The caller must never
*    delete the object that is returned.
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
*    void setValue (const char *) - Set the text string value of the icon.
*
*    char *getValue () - Get the text string value of the icon.
*
*    void setDisplayLength (int display_length) - Set the length of the
*    display field.
*
*    void setEditable (Boolean flag) - Specify that the text is editable.
*
*    void setJustification (Justification new_justification) - Specifies
*    the justification of the text within the display field: JUSTIFY_LEFT,
*    JUSTIFY_CENTER, JUSTIFY_RIGHT.
*
*    void setUnderline (Boolean flag) - Specifies whether or not to underline
*    the text.
*
*    char *getValue (char*&) - Gets the string value of the object.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    DrawIcon encapsulated by this object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSDrawIconDefaults () - Initializes data members.
*
*/

#ifndef _CS_DRAW_ICON_H
#define _CS_DRAW_ICON_H

// Includes
#include "ColorMap.h"
#include "CSComponent.h"
#include "CSDrawImage.h"
#include "CSDrawText.h"

#include <sspkg/drawobj.h>

// Class Definition
class CSDrawIcon : public CSComponent {
public:
  CSDrawIcon ();
  ~CSDrawIcon ();

  void setGap (int new_gap)
    {
      gap = new_gap;

      if (owObject)
	xv_set (owObject, DRAWICON_GAP, gap, NULL);
    }

  void setImageColor (Color color)
    {
      if (!colorMap) {
	useDefaultColorMap ();
	if (owObject) initializeColorMap (serverParent);
      }
    
      imageColor = colorMap->findColor (color);
    
      if (owObject)
	xv_set (owObject, DRAWICON_IMAGE_COLOR, imageColor, NULL);
    }

  void setImageColor (const char *color)
    {
      if (!colorMap) {
	useDefaultColorMap ();
	if (owObject) initializeColorMap (serverParent);
      }
    
      imageColor = colorMap->findColor (color);
    
      if (owObject)
	xv_set (owObject, DRAWICON_IMAGE_COLOR, imageColor, NULL);
    }

  void setTextColor (Color color)
    {
      if (!colorMap) {
	useDefaultColorMap ();
	if (owObject) initializeColorMap (serverParent);
      }
    
      textColor = colorMap->findColor (color);
    
      if (owObject)
	xv_set (owObject, DRAWICON_TEXT_COLOR, textColor, NULL);
    }

  void setTextColor (const char *color)
    {
      if (!colorMap) {
	useDefaultColorMap ();
	if (owObject) initializeColorMap (serverParent);
      }
    
      textColor = colorMap->findColor (color);
    
      if (owObject)
	xv_set (owObject, DRAWICON_TEXT_COLOR, textColor, NULL);
    }

  void setLayout (Orientation new_layout)
    {
      layout = new_layout;

      if (owObject)
	xv_set (owObject,
		DRAWICON_LAYOUT_VERTICAL, ((layout == VERTICAL) ?
		                           TRUE : FALSE),
		NULL);
    }

  CSDrawImage *getCSDrawImage ()
    {
      return drawImage;
    }

  CSDrawText *getCSDrawText ()
    {
      return drawText;
    }

  void setSelectedImage (Image &image)
    {
      selectedImage = &image;

      if (owObject) {
	((UIObject *) &image)->createObject ((UIObject *) canvasShellDisplay);
	xv_set (owObject, DRAWIMAGE_IMAGE2, image.getXViewObject (), NULL);
      }
    }

  void setSelectedImageMask (Image &image)
    {
      selectedImageMask = &image;

      if (owObject) {
        ((UIObject *) &image)->createObject ((UIObject *) canvasShellDisplay);
	xv_set (owObject,
		DRAWIMAGE_IMAGE2_MASK, image.getXViewObject (),
		NULL);
      }
    }

  void setNotSelectedImage (Image &image)
    {
      notSelectedImage = &image;

      if (owObject) {
        ((UIObject *) &image)->createObject ((UIObject *) canvasShellDisplay);
	xv_set (owObject, DRAWIMAGE_IMAGE1, image.getXViewObject (), NULL);
      }
    }

  void setNotSelectedImageMask (Image &image)
    {
      notSelectedImageMask = &image;

      if (owObject) {
        ((UIObject *) &image)->createObject ((UIObject *) canvasShellDisplay);
	xv_set (owObject,
		DRAWIMAGE_IMAGE1_MASK, image.getXViewObject (),
		NULL);
      }
    }

  char *getValue (char*&);
  void  setValue (const char *);

  void setDisplayLength (int display_length)
    {
      displayLength = display_length;

      if (owObject)
	xv_set (owObject, DRAWTEXT_LENGTH, displayLength, NULL);
    }

  void setEditable (Boolean flag)
    {
      editable = flag;
      editableSet = TRUE;

      if (owObject)
	xv_set (owObject, DRAWTEXT_EDITABLE, editable, NULL);
    }

  void setJustification (Justification new_justification)
    {
      justification = new_justification;

      if (owObject) {
	if (justification == JUSTIFY_LEFT)
	  xv_set (owObject, DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_LEFT, NULL);
	else if (justification == JUSTIFY_CENTER)
	  xv_set (owObject, DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_CENTER, NULL);
	else if (justification == JUSTIFY_RIGHT)
	  xv_set (owObject, DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_RIGHT, NULL);
      }
    }

  void setUnderline (Boolean flag)
    {
      underline = flag;
      underlineSet = TRUE;

      if (owObject)
	xv_set (owObject, DRAWTEXT_SHOW_UNDERLINE, underline, NULL);
    }

protected:
  int            gap;
  int            imageColor;
  int            textColor;
  Orientation    layout;
  CSDrawImage   *drawImage;
  CSDrawText    *drawText;
  Image         *selectedImage;
  Image         *selectedImageMask;
  Image         *notSelectedImage;
  Image         *notSelectedImageMask;
  int            displayLength;
  Boolean        editable;
  Boolean        editableSet;
  Justification  justification;
  char          *stringValue;
  Boolean        underline;
  Boolean        underlineSet;

  virtual void createObject (UIObject *parent);

private:    
  void setCSDrawIconDefaults ();
};

#endif /* _CS_DRAW_ICON_H */
