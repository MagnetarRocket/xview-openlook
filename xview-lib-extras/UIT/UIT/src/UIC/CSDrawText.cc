/*H+ CSDrawText.cc
*
*    SCCS ID: @(#)CSDrawText.cc 1.2 93/06/01 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSDrawText class.
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
static char sccsid [] = "@(#)CSDrawText.cc 1.2 93/06/01 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSDrawText.h"

/*F+ CSDrawText::CSDrawText ()
*
*    PURPOSE:
*
*    Constructor of the CSDrawText class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawText::CSDrawText ()
{
  setCSDrawTextDefaults ();
}

/*F+ CSDrawText::CSDrawText (const char *string_value)
*
*    PURPOSE:
*
*    Constructor of the CSDrawText class.
*
*    PARAMETERS:
*
*    string_value - The value of the text.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawText::CSDrawText (const char *string_value)
{
  setCSDrawTextDefaults ();

  if (string_value)
    stringValue = strcpy (new char [strlen (string_value)+1], string_value);
}

/*F+ CSDrawText::setCSDrawTextDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSDrawText class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawText::setCSDrawTextDefaults ()
{
  displayLength = UIC_NOT_SET;
  editable = FALSE;
  editableSet = FALSE;
  justification = (Justification) UIC_NOT_SET;
  stringValue = (char *) NULL;
  underline = FALSE;
  underlineSet = FALSE;
  isDrawObject = TRUE;
}

/*F+ CSDrawText::~CSDrawText ()
*
*    PURPOSE:
*
*    Destructor of the CSDrawText class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawText::~CSDrawText ()
{
  if (stringValue) delete stringValue;
}

/*F+ char *CSDrawText::getValue (char* &value)
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
char *CSDrawText::getValue (char* &value)
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

/*F+ void CSDrawText::setValue (const char *value)
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
void CSDrawText::setValue (const char *value)
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

/*F+ void CSDrawText::setFont (CharacterFont &char_font)
*
*    PURPOSE:
*
*    Specifies the font to be used with this object.
*
*    PARAMETERS: 
*
*    char_font - The UIC font to use.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawText::setFont (CharacterFont &char_font)
{
  UIObject::setFont (char_font);
 
  // If the XView object already exists...
  if (owObject) {
    // Create the XView font object and set it
    ((UIObject *) font)->createObject (this);
    xv_set (owObject, DRAWTEXT_FONT, font->getXViewObject (), NULL);
  }
}
 
/*F+ void CSDrawText::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSDrawText object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawText::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), DRAWTEXT, NULL);

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

  if (font) {
    ((UIObject *) font)->createObject (parent);
    xv_set (owObject, DRAWTEXT_FONT, font->getXViewObject (), NULL);
  }

  setCSComponentAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
