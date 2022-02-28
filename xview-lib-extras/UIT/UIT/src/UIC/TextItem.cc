/*H+ TextItem.cc
*
*    SCCS ID: @(#)TextItem.cc 1.1 91/10/24 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the TextItem class.
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
static char sccsid [] = "@(#)TextItem.cc 1.1 91/10/24 SMI"; 
#endif

// Includes
#include "TextItem.h"

/*F+ TextItem::TextItem ()
*
*    PURPOSE:
*
*    TextItem constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TextItem::TextItem ()
{
  boldLabel = FALSE;
}

/*F+ TextItem::TextItem (char *string_label)
*
*    PURPOSE:
*
*    TextItem constructor.
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
TextItem::TextItem (char *string_label) : UIComponent (string_label)
{
  boldLabel = FALSE;
}

/*F+ TextItem::TextItem (Image &image_label)
*
*    PURPOSE:
*
*    TextItem constructor.
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
TextItem::TextItem (Image &image_label) : UIComponent (image_label)
{
  boldLabel = FALSE;
}

/*F+ TextItem::~TextItem ()
*
*    PURPOSE:
*
*    TextItem destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TextItem::~TextItem ()
{
}

/*F+ void TextItem::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the TextItem XView object.
*
*    PARAMETER:
*
*    parent - The parent of the TextItem (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void TextItem::createObject (UIObject *parent)
{
  // Create an XView TextItem taking into account font settings
  createWithFonts (parent, PANEL_MESSAGE, NULL);

  // Set common XView attrbitutes
  setUIComponentAttributes (parent);

  if (boldLabel)
    xv_set (owObject, PANEL_LABEL_BOLD, TRUE, NULL);
}
       
#ifdef TEST
#endif /* TEST */
