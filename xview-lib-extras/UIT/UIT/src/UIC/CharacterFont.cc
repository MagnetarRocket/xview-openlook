/*H+ CharacterFont.cc
*
*    SCCS ID: @(#)CharacterFont.cc 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CharacterFont class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       05/10/91   Preparation for code review. 
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)CharacterFont.cc 1.1 91/10/24 SMI"; 
#endif

// Includes
#include "CharacterFont.h"

/*F+ char *CharacterFont::getName ()
*
*    PURPOSE:
*
*    Gets the XView name of the font.
*
*    RETURN VALUES:
*
*    Returns a string containing the value.  The string is allocated
*    from the heap and must be deleted by the caller.  If the XView
*    font does not exist, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *CharacterFont::getName ()
{ 
  char *return_value = (char *) NULL;

  if (owObject) {
    char *temp_string = (char *) xv_get (owObject, FONT_NAME);
    return_value = strcpy (new char [strlen (temp_string) + 1], temp_string);
  }

  return (return_value);
}

/*F+ char *CharacterFont::getFamily ()
*
*    PURPOSE:
*
*    Gets the XView family of the font.
*
*    RETURN VALUES:
*
*    Returns a string containing the value.  The string is allocated
*    from the heap and must be deleted by the caller.  If the XView
*    font does not exist, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *CharacterFont::getFamily ()
{
  char *return_value = (char *) NULL;

  if (owObject) {
    char *temp_string = (char *) xv_get (owObject, FONT_FAMILY);
    return_value = strcpy (new char [strlen (temp_string) + 1], temp_string);
  }

  return (return_value);
}

/*F+ char *CharacterFont::getStyle ()
*
*    PURPOSE:
*
*    Gets the XView style of the font.
*
*    RETURN VALUES:
*
*    Returns a string containing the value.  The string is allocated
*    from the heap and must be deleted by the caller.  If the XView
*    font does not exist, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *CharacterFont::getStyle ()
{
  char *return_value = (char *) NULL;

  if (owObject) {
    char *temp_string = (char *) xv_get (owObject, FONT_STYLE);
    return_value = strcpy (new char [strlen (temp_string) + 1], temp_string);
  }

  return (return_value);
}

/*F+ int CharacterFont::getSize ()
*
*    PURPOSE:
*
*    Gets the XView size of the font.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int CharacterFont::getSize ()
{
  if (owObject)
    return (int) xv_get (owObject, FONT_SIZE);

  return 0;
}

/*F+ void CharacterFont::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView font.
*
*    PARAMETERS:
*
*    parent - The parent of the CharacterFont.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CharacterFont::createObject (UIObject *parent)
{
  /* Get the XView frame of the font's parent */
  Frame frame = (Frame) xv_get (parent->getXViewObject (),
				XV_KEY_DATA,
				UIC_FRAME_KEY);

  /* If an XView font name was specified, find the font... */
  if (fontName)
    owObject = xv_find (frame, FONT,
			FONT_NAME, fontName,
			NULL);
  /* Else, create the specified font... */
  else
    owObject = xv_create (frame, FONT,
			  FONT_FAMILY, fontFamily,
			  FONT_STYLE,  fontStyle,
			  FONT_SIZE,   fontSize,
			  NULL);

  UIObject::setUIObjectAttributes (parent);
}

#ifdef TEST
#endif /* TEST */
