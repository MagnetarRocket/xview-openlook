/*H+ AlphanumericInput.cc
*
*    SCCS ID: @(#)AlphanumericInput.cc 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the AlphanumericInput class.
*
*    AUTHORS: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       05/10/91   Preparation for code review.
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)AlphanumericInput.cc 1.1 91/10/24 SMI"; 
#endif

// Includes
#include "AlphanumericInput.h"

/*F+ AlphanumericInput::AlphanumericInput ()
*
*    PURPOSE:
*
*    Constructor for the AlphanumericInput class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
AlphanumericInput::AlphanumericInput ()
{
  setAlphanumericInputDefaults ();
}

/*F+ AlphanumericInput::AlphanumericInput (char *string_label)
*
*    PURPOSE:
*
*    Constructor for the AlphanumericInput class.
*
*    PARAMETERS:
*
*    char *string_label - String label for the input field.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
AlphanumericInput::AlphanumericInput (char *string_label)
		  : UIInputTextItem (string_label)
{
  setAlphanumericInputDefaults ();
}

/*F+ AlphanumericInput::AlphanumericInput (Image &image_label)
*
*    PURPOSE:
*
*    Constructor for the AlphanumericInput class.
*
*    PARAMETERS:
*
*    Image &image_label - Image label for the input field.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
AlphanumericInput::AlphanumericInput (Image &image_label)
		  : UIInputTextItem (image_label)
{
  setAlphanumericInputDefaults ();
}

/*F+ void AlphanumericInput::setAlphanumericInputDefaults ()
*
*    PURPOSE:
*
*    Constructor for the AlphanumericInput class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void AlphanumericInput::setAlphanumericInputDefaults ()
{
  value = (char *) NULL;
}

/*F+ AlphanumericInput::~AlphanumericInput ()
*
*    PURPOSE:
*
*    Destructor for the AlphanumericInput class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
AlphanumericInput::~AlphanumericInput ()
{
  if (value != (char *) NULL)
    delete value;
}

/*F+ char *AlphanumericInput::getValue (char* &return_value)
*
*    PURPOSE:
*
*    Get the current value of the input field.
*
*    PARAMETERS:
*
*    char* &return_value - The value of the input field.
*
*    RETURN VALUES:
*
*    Returns a pointer to a newly allocated string as both the function
*    return value and the through parameter return_value.  The caller
*    must delete the allocated memory.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *AlphanumericInput::getValue (char* &return_value)
{
  return_value = (char *) NULL;

  char *temp_string = (owObject ? (char *) xv_get (owObject, PANEL_VALUE) :
		                  value);
  if (temp_string)
    return_value = strcpy (new char [strlen (temp_string) + 1], temp_string);

  return (return_value);
}

/*F+ void AlphanumericInput::setValue (char* &new_value)
*
*    PURPOSE:
*
*    Set the value of the input field.
*
*    PARAMETERS:
*
*    char* &new_value - The value of the input field.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void AlphanumericInput::setValue (char *new_value)
{
  if (owObject)
    xv_set (owObject, PANEL_VALUE, new_value, NULL);
  else {
    if (value) delete value;

    value = (new_value ? strcpy (new char [strlen (new_value) + 1], new_value) :
			 (char *) NULL);
  }
}

/*F+ void AlphanumericInput::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Instantiates the XView graphics object.
*
*    PARAMETERS:
*
*    UIObject *parent - The parent of the AlphanumericInput object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void AlphanumericInput::createObject (UIObject *parent)
{
  // Create the XView object, setting the fonts during the create
  createWithFonts (parent, PANEL_TEXT, NULL);

  if (value)
    xv_set (owObject, PANEL_VALUE, value, NULL);
  
  // Set common XView attributes
  setUIInputTextItemAttributes (parent);
}

#ifdef TEST
#endif /* TEST */
