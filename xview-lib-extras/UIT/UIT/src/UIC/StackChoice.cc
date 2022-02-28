/*H+ StackChoice.cc
*
*    SCCS ID: @(#)StackChoice.cc 1.2 91/10/25 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the StackChoice class.
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
static char sccsid [] = "@(#)StackChoice.cc 1.2 91/10/25 SMI"; 
#endif

// Includes
#include "StackChoice.h"

/*F+ StackChoice::StackChoice ()
*
*    PURPOSE:
*
*    StackChoice constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
StackChoice::StackChoice ()
{
  setStackChoiceDefaults ();
}

/*F+ StackChoice::StackChoice (char *string_label)
*
*    PURPOSE:
*
*    StackChoice constructor.
*
*    PARAMETERS:
*
*    string_label - Specifies a string for the label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
StackChoice::StackChoice (char *string_label) : UIChoice (string_label)
{
  setStackChoiceDefaults ();
}

/*F+ StackChoice::StackChoice (char *image_label)
*
*    PURPOSE:
*
*    StackChoice constructor.
*
*    PARAMETERS:
*
*    image_label - Specifies an image for the label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
StackChoice::StackChoice (Image &image_label) : UIChoice (image_label)
{
  setStackChoiceDefaults ();
}

/*F+ void StackChoice::setStackChoiceDefaults ()
*
*    PURPOSE:
*
*    Initializes StackChoice data members.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void StackChoice::setStackChoiceDefaults ()
{
  defaultChoice = UIC_NOT_SET;
}

/*F+ StackChoice::~StackChoice ()
*
*    PURPOSE:
*
*    StackChoice destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
StackChoice::~StackChoice ()
{
}

/*F+ void StackChoice::setDefaultChoice (int choice)
*
*    PURPOSE:
*
*    Specifies the default choice.
*
*    PARAMETER:
*
*    choice - The number of the choice.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void StackChoice::setDefaultChoice (int choice)
{
  defaultChoice = choice;

  if (owObject)
    xv_set (owObject, PANEL_DEFAULT_ITEM, defaultChoice, NULL);
}

/*F+ void StackChoice::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the StackChoice XView object.
*
*    PARAMETER:
*
*    parent - The parent of the StackChoice (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void StackChoice::createObject (UIObject *parent)
{
  // Create an XView StackChoice taking into account font settings
  createWithFonts (parent, PANEL_CHOICE_STACK, (void *) NULL);

  // Set common XView attrbitutes
  setUIChoiceAttributes (parent);

  if (defaultChoice != UIC_NOT_SET)
    xv_set (owObject, PANEL_DEFAULT_ITEM, defaultChoice, NULL);
}
       
#ifdef TEST
#endif /* TEST */
