/*H+ CSDrawText.h
*
*    SCCS ID: @(#)CSDrawText.h 1.2 93/06/01 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSDrawText class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSDrawText
*
*    DESCRIPTION:
*
*    The CSDrawText component provides the ability to draw text on
*    a CanvasShellDisplay.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSDrawText object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
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
*    void setValue (const char *) - Set the string value of the object.
*
*    void setFont (CharacterFont&) - Set the fint of the text for this object.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    DRAWTEXT encapsulated by this object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSDrawTextDefaults () - Initializes data members.
*
*/

#ifndef _CS_DRAW_TEXT_H
#define _CS_DRAW_TEXT_H

// Includes
#include "CharacterFont.h"
#include "CSComponent.h"

#include <sspkg/drawobj.h>

// Class Definition
class CSDrawText : public CSComponent {
public:
  CSDrawText ();
  CSDrawText (const char *);
  ~CSDrawText ();

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

  void setFont (CharacterFont&);

protected:
  int            displayLength;
  Boolean        editable;
  Boolean        editableSet;
  Justification  justification;
  char          *stringValue;
  Boolean        underline;
  Boolean        underlineSet;

  virtual void createObject (UIObject *parent);

private:    
  void setCSDrawTextDefaults ();

friend class CSDrawIcon;
};

#endif /* _CS_DRAW_TEXT_H */
