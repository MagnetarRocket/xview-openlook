/*H+ UIInputTextItem.h
*
*    SCCS ID: @(#)UIInputTextItem.h 1.3 92/06/04 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the UIInputTextItem class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       05/10/91   Preparation for code review.
*
*H-*/

/*C+ UIInputTextItem
*
*    DESCRIPTION:
*
*    This class defines the general characteristics of fields that allow
*    user input.  All classes derived from this are classes that provide
*    the ability to display text and allow the user to modify the displayed
*    text (if the object is not create as a read only object).
*
*    INHERITED CLASSES:
*
*    UIComponent - The UIInputTextItem class and all classes derived from it
*                  are components and can be added to a ComponentDisplay.
*
*    ORDINARY DATA MEMBERS:
*
*    int     inputLength;        - The maximum stored length of the text.
*    int     inputDisplayLength; - The length of the text display.
*    Boolean underline;          - Specifies if the text is underlined.
*    Boolean readOnly;           - Specifies if user input is allowed.
*    Boolean echo;               - specifies if user input is to be echoed.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setMaxInputLength (int)
*         - Sets the maximum length of input to this text item.
*           Default: XView default.
*
*    void setInputDisplayLength (int)
*         - Sets the length of the text displayed by this item.
*           Default: XView default.
*
*    void setUnderline (Boolean)
*         - Turns on or off underlines from the text field.  Default: TRUE.
*
*    void setReadOnly (Boolean)
*         - Turns on or off the user's ability to modify the text field.
*           Default: FALSE.
*
*    void setEcho (Boolean)
*         - Specifies whether or not user input should be echoed and displayed.
*           Default: TRUE.
*
*    void setMask (char)
*         - Specifies the mask character to be used when typed in text is being
*           output.  A "setEcho (FALSE)" call takes precedence over setMask
*	    call.
*           Default: No mask.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    static Panel_setting notifyProc (Panel_item, Event *) - Wrapper for the
*                         XView notify proc.  This is called whenever a
*                         notify event occurs on an object of class that
*                         is derived from UIInputTextItem.
*
*    void setUIInputTextItemAttributes (UIObject *)
*       - Sets XView attributes that are common to the UIInputTextItem objects.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void	   setUIInputTextItemDefaults ()
*                  - Initializes data memebers.
*C-*/
#ifndef _UIINPUT_TEXT_ITEM_H
#define _UIINPUT_TEXT_ITEM_H

// Includes
#include "UIComponent.h"

// Class Definition
class UIInputTextItem : public UIComponent {
public:
    		 	 UIInputTextItem ();
    		 	 UIInputTextItem (char *);
		 	 UIInputTextItem (Image &);
			~UIInputTextItem ();

    void	 	 setMaxInputLength (int length)
			 { inputLength = length;
			   if (owObject) {
			     if (resourceName)
			       xv_set (owObject,
				       XV_USE_DB,
				         PANEL_VALUE_STORED_LENGTH,
				           inputLength,
				         NULL,
				       NULL);
			     else
			       xv_set (owObject,
				       PANEL_VALUE_STORED_LENGTH, inputLength,
				       NULL);
			   }
			 }

    void	 	 setInputDisplayLength (int length)
			 { inputDisplayLength = length;
			   if (owObject) {
			     if (resourceName)
			       xv_set (owObject,
				       XV_USE_DB,
				         PANEL_VALUE_DISPLAY_LENGTH,
				           inputDisplayLength,
				         NULL,
				       NULL);
			     else
			       xv_set (owObject,
				       PANEL_VALUE_DISPLAY_LENGTH,
				         inputDisplayLength,
				       NULL);
			   }
			 }

    void 	 	 setUnderline (Boolean value)
			 { underline = value;
			   if (owObject) xv_set (owObject,
			                         PANEL_VALUE_UNDERLINED,
						 underline,
						 NULL); }

    void		 setReadOnly (Boolean flag)
			 { readOnly = flag;
			   if (owObject) xv_set (owObject,
			                         PANEL_READ_ONLY, readOnly,
						 NULL); }

    void		 setEcho (Boolean flag)
			 { echo = flag;
			   if (owObject) {
			     if (echo) {
			       xv_set (owObject, PANEL_MASK_CHAR, '\0', NULL);

			       if (mask)
			         xv_set (owObject, PANEL_MASK_CHAR, mask, NULL);
			     }
			     else	
			       xv_set (owObject, PANEL_MASK_CHAR, ' ', NULL);
			   }
			 }

    void		 setMask (char new_mask)
			 { mask = new_mask;
			   if (owObject && echo)
			     xv_set (owObject, PANEL_MASK_CHAR, mask, NULL);
			 }

protected:
    int		  	 inputLength;
    int		 	 inputDisplayLength;
    Boolean	 	 underline;
    Boolean		 readOnly;
    Boolean		 echo;
    char		 mask;

    void 	         setUIInputTextItemAttributes (UIObject *);

    static Panel_setting notifyProc (Panel_item, Event *);

private:
    void	 	 setUIInputTextItemDefaults ();
};

#endif /* _UIINPUT_TEXT_ITEM_H */
