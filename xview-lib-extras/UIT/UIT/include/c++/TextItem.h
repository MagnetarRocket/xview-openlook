/*H+ TextItem.h
*
*    SCCS ID: @(#)TextItem.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the TextItem class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/09/91   Preparation for code review.
*
*H-*/

/*C+ TextItem
*
*    DESCRIPTION:
*
*    A TextItem object displays any block of text on a
*    ComponentDisplay object.
*
*    INHERITED CLASSES:
*
*    UIComponent - Defines the general characteristics of
*    components in the UIC.
*
*    ORDINARY DATA MEMBERS:
*
*    Boolean	 boldLabel - TRUE if the text should be 
*    displayed in a bold font.  Default: FALSE.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*            void   setLabelBold (Boolean) - Sets whether or not 
*            the text should be displayed in a bold font.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void   createObject (UIObject *) - Create XView object.
*
*C-*/
#ifndef _TEXT_ITEM_H
#define _TEXT_ITEM_H

// Includes
#include "UIComponent.h"

// Class Definition
class TextItem : public UIComponent {
public:
    		 TextItem ();
    		 TextItem (char *);
		 TextItem (Image &);
		~TextItem ();

    void	 setLabelBold (Boolean bold)
		 { boldLabel = bold;
		   if (owObject) xv_set (owObject,
					 PANEL_LABEL_BOLD, boldLabel,
					 NULL); }
    
protected:
    Boolean	 boldLabel;

    virtual void createObject (UIObject *);
};

#endif /* _TEXT_ITEM_H */
