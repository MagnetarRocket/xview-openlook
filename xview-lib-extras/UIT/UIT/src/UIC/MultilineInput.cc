/*H+ MultilineInput.cc
*
*    SCCS ID: @(#)MultilineInput.cc 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the MultilineInput class.
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
static char sccsid [] = "@(#)MultilineInput.cc 1.1 91/10/24 SMI"; 
#endif

// Includes
#include "MultilineInput.h"

/*F+ MultilineInput::MultilineInput ()
*
*    PURPOSE:
*
*    Constructor for the MultilineInput class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
MultilineInput::MultilineInput ()
{
  setMultilineInputDefaults ();
}

/*F+ MultilineInput::MultilineInput (char *string_label)
*
*    PURPOSE:
*
*    Constructor for the MultilineInput class.
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
MultilineInput::MultilineInput (char *string_label)
	       : UIInputTextItem (string_label)
{
  setMultilineInputDefaults ();
}

/*F+ MultilineInput::MultilineInput (Image &image_label)
*
*    PURPOSE:
*
*    Constructor for the MultilineInput class.
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
MultilineInput::MultilineInput (Image &image_label)
	       : UIInputTextItem (image_label)
{
  setMultilineInputDefaults ();
}

/*F+ void MultilineInput::setMultilineInputDefaults ()
*
*    PURPOSE:
*
*    Constructor for the MultilineInput class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void MultilineInput::setMultilineInputDefaults ()
{
  value = (char *) NULL;
  numRows = UIC_NOT_SET;
}

/*F+ MultilineInput::~MultilineInput ()
*
*    PURPOSE:
*
*    Destructor for the MultilineInput class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
MultilineInput::~MultilineInput ()
{
  if (value != (char *) NULL)
    delete value;
}

/*F+ char *MultilineInput::getValue (char* &return_value)
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
char *MultilineInput::getValue (char* &return_value)
{
  return_value = (char *) NULL;

  char *temp_string = ((owObject) ? (char *) xv_get (owObject, PANEL_VALUE) :
		                    value);
  if (temp_string)
    return_value = strcpy (new char [strlen (temp_string) + 1], temp_string);

  return (return_value);
}

/*F+ void MultilineInput::setValue (char* &new_value)
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
void MultilineInput::setValue (char *new_value)
{
  if (owObject)
    xv_set (owObject, PANEL_VALUE, new_value, NULL);
  else {
    if (value) delete value;

    value = (new_value ? strcpy (new char [strlen (new_value) + 1], new_value) :
			 (char *) NULL);
  }
}

/*F+ void MultilineInput::setDisplayedRows (int num_rows)
*
*    PURPOSE:
*
*    Set the number of rows to display.
*
*    PARAMETERS:
*
*    int num_rows - The number of rows to display.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void MultilineInput::setDisplayedRows (int num_rows)
{
  numRows = num_rows;

  if (owObject)
    xv_set (owObject, PANEL_DISPLAY_ROWS, numRows, NULL);
}
 
/*F+ void MultilineInput::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Instantiates the XView graphics object.
*
*    PARAMETERS:
*
*    UIObject *parent - The parent of the MultilineInput object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void MultilineInput::createObject (UIObject *parent)
{
  // Create the XView object, setting the fonts during the create
  createWithFonts (parent, PANEL_MULTILINE_TEXT, NULL);

  if (value)
    xv_set (owObject, PANEL_VALUE, value, NULL);

  if (numRows != UIC_NOT_SET)
    xv_set (owObject, PANEL_DISPLAY_ROWS, numRows, NULL);
  
  // Set common XView attributes
  setUIInputTextItemAttributes (parent);
}

#ifdef TEST
#endif /* TEST */
