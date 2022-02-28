/*H+ NumericInput.cc
*
*    SCCS ID: @(#)NumericInput.cc 1.3 92/06/09 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the NumericInput class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       05/10/91   Preparation for code review. 
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)NumericInput.cc 1.3 92/06/09 SMI"; 
#endif

// Includes
#include "NumericInput.h"

/*F+ NumericInput::NumericInput ()
*
*    PURPOSE:
*
*    NumericInput constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
NumericInput::NumericInput ()
{
  setNumericInputDefaults ();
}

/*F+ NumericInput::NumericInput (char *string_label)
*
*    PURPOSE:
*
*    NumericInput constructor.
*
*    PARAMETERS:
*
*    string_label - Specifies a string to be used as a label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
NumericInput::NumericInput (char *string_label) : UIInputTextItem (string_label)
{
  setNumericInputDefaults ();
}

/*F+ NumericInput::NumericInput (char *image_label)
*
*    PURPOSE:
*
*    NumericInput constructor.
*
*    PARAMETERS:
*
*    image_label - Specifies an image to be used as a label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
NumericInput::NumericInput (Image &image_label) : UIInputTextItem (image_label)
{
  setNumericInputDefaults ();
}

/*F+ void NumericInput::setNumericInputDefaults ()
*
*    PURPOSE:
*
*    Sets the initial values of the data members.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void NumericInput::setNumericInputDefaults ()
{
  value = UIC_NOT_SET;
  valueSet = FALSE;
  minValue = UIC_NOT_SET;
  minValueSet = FALSE;
  maxValue = UIC_NOT_SET;
  maxValueSet = FALSE;
}

/*F+ NumericInput::~NumericInput ()
*
*    PURPOSE:
*
*    NumericInput destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
NumericInput::~NumericInput ()
{
}

/*F+ int NumericInput::getValue (int &return_value)
*
*    PURPOSE:
*
*    Returns the current value of the NumericInput object.
*
*    PARAMETERS:
*
*    return_value - The value of the object.
*
*    RETURN VALUES:
*
*    Returns the current value of the NumericInput object.  If the
*    XView object has not been created and no value has been specified
*    previously, then UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int NumericInput::getValue (int &return_value)
{
  return_value = value;

  if (owObject)
    return_value = (int) xv_get (owObject, PANEL_VALUE);

  return (return_value);
}

/*F+ void NumericInput::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the NumericInput object (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void NumericInput::createObject (UIObject *parent)
{
  // Create the XView object with any fonts that may have been specified
  createWithFonts (parent, PANEL_NUMERIC_TEXT, NULL);

  // Set common XView attributes
  setUIInputTextItemAttributes (parent);
  
  if (minValueSet) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_MIN_VALUE, minValue, NULL, NULL);
    else
      xv_set (owObject, PANEL_MIN_VALUE, minValue, NULL);
  }

  if (maxValueSet) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_MAX_VALUE, maxValue, NULL, NULL);
    else
      xv_set (owObject, PANEL_MAX_VALUE, maxValue, NULL);
  }

  if (valueSet)
    xv_set (owObject, PANEL_VALUE, value, NULL);
}

#ifdef TEST
#endif /* TEST */
