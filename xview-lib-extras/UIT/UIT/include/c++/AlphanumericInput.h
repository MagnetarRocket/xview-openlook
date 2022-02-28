/*H+ AlphanumericInput.h
*
*    SCCS ID: @(#)AlphanumericInput.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the AlphanumericInput class.
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

/*C+ AlphanumericInput
*
*    DESCRIPTION:
*
*    An AlphanumericInput object displays a text field and allows the
*    user to input text.  If the object is created as a read only object
*    (via the "setReadOnly" member function in UIInputTextItem), the text
*    is displayed and cannot be modified by the user.
*
*    INHERITED CLASSES:
*
*    UIInputTextItem - All input text items, numeric or alphanumeric,
*                      inherit this.  UIInputTextItem is used to
*                      provide a foundation of functionality specific
*                      to input text items.
*
*    ORDINARY DATA MEMBERS:
*
*    char *value; - Contains the text value of the Alphanumeric object.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    char *getValue (char *&)
*          - Gets the value of the text field of the Alphanumeric object.
*    void  setValue (char *)
*          - Sets the value of the text field of the Alphanumeric object.
*            Default: Not set.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    void  createObject (UIObject *)
*          - Creates the XView object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setAlphanumericInputDefaults ()
*         - Initializes data members.
*
*C-*/
#ifndef _ALPHANUMERIC_INPUT_H
#define _ALPHANUMERIC_INPUT_H

// Includes
#include "UIInputTextItem.h"

// Class Definition
class AlphanumericInput : public UIInputTextItem {
public:
    		  AlphanumericInput ();
    		  AlphanumericInput (char *);
		  AlphanumericInput (Image &);
   		 ~AlphanumericInput ();

    char	 *getValue (char *&);
    void	  setValue (char *);

protected:
    char *value;

    virtual void  createObject (UIObject *);

private:    
    void setAlphanumericInputDefaults ();
};

#endif /* _ALPHANUMERIC_INPUT_H */
