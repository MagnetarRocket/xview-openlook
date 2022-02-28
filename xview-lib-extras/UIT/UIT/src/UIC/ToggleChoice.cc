/*H+ ToggleChoice.cc
*
*    SCCS ID: @(#)ToggleChoice.cc 1.2 91/10/25 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the ToggleChoice class.
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
static char sccsid [] = "@(#)ToggleChoice.cc 1.2 91/10/25 SMI"; 
#endif

// Includes
#include "ToggleChoice.h"

/*F+ ToggleChoice::ToggleChoice ()
*
*    PURPOSE:
*
*    ToggleChoice constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ToggleChoice::ToggleChoice ()
{
}

/*F+ ToggleChoice::ToggleChoice (char *string_label)
*
*    PURPOSE:
*
*    ToggleChoice constructor.
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
ToggleChoice::ToggleChoice (char *string_label) : UIChoice (string_label)
{
}

/*F+ ToggleChoice::ToggleChoice (char *image_label)
*
*    PURPOSE:
*
*    ToggleChoice constructor.
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
ToggleChoice::ToggleChoice (Image &image_label) : UIChoice (image_label)
{
}

/*F+ ToggleChoice::~ToggleChoice ()
*
*    PURPOSE:
*
*    ToggleChoice destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ToggleChoice::~ToggleChoice ()
{
}

/*F+ void ToggleChoice::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the ToggleChoice XView object.
*
*    PARAMETER:
*
*    parent - The parent of the ToggleChoice (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ToggleChoice::createObject (UIObject *parent)
{
  // Create an XView ToggleChoice taking into account font settings
  createWithFonts (parent, PANEL_CHOICE, (void *) NULL);

  // Set common XView attrbitutes
  setUIChoiceAttributes (parent);
}
       
#ifdef TEST
#endif /* TEST */
