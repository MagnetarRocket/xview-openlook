/*H+ CheckBoxChoice.cc
*
*    SCCS ID: @(#)CheckBoxChoice.cc 1.2 91/10/25 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CheckBoxChoice class.
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
static char sccsid [] = "@(#)CheckBoxChoice.cc 1.2 91/10/25 SMI"; 
#endif

// Includes
#include "CheckBoxChoice.h"

/*F+ CheckBoxChoice::CheckBoxChoice ()
*
*    PURPOSE:
*
*    CheckBoxChoice constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CheckBoxChoice::CheckBoxChoice ()
{
  multipleChoice = TRUE;
}

/*F+ CheckBoxChoice::CheckBoxChoice (char *string_label)
*
*    PURPOSE:
*
*    CheckBoxChoice constructor.
*
*    PARAMETERS:
*
*    string_label - Specifies a string for the label of the choice object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CheckBoxChoice::CheckBoxChoice (char *string_label) : UIChoice (string_label)
{
  multipleChoice = TRUE;
}

/*F+ CheckBoxChoice::CheckBoxChoice (Image &image_label)
*
*    PURPOSE:
*
*    CheckBoxChoice constructor.
*
*    PARAMETERS:
*
*    image_label - Specifies an image for the label of the choice object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CheckBoxChoice::CheckBoxChoice (Image &image_label) : UIChoice (image_label)
{
  multipleChoice = TRUE;
}

/*F+ CheckBoxChoice::~CheckBoxChoice ()
*
*    PURPOSE:
*
*    CheckBoxChoice destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CheckBoxChoice::~CheckBoxChoice ()
{
}

/*F+ CheckBoxChoice::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The CheckBoxChoice parent (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CheckBoxChoice::createObject (UIObject *parent)
{
  /* Create an XView PANEL_CHECK_BOX with the specified fonts */
  createWithFonts (parent, PANEL_CHECK_BOX, (void *) NULL);

  /* Set common XView attributes */
  setUIChoiceAttributes (parent);
}

#ifdef TEST
#endif /* TEST */
