/*H+ MultilineInput.h
*
*    SCCS ID: @(#)MultilineInput.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the MultilineInput class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ MultilineInput
*
*    DESCRIPTION:
*
*    A MultilineInput object displays multiple text field lines	
*    allows the user to input text.  If the object is created as a read
*    only object (via the "setReadOnly" member function in UIInputTextItem),
*    the text is displayed and cannot be modified by the user.
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
*    char *value; - Contains the text value of the MultilineInput object.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    char *getValue (char *&)
*          - Gets the value of the text field of the MultilineInput object.
*    void  setValue (char *)
*          - Sets the value of the text field of the MultilineInput object.
*            Default: Not set.
*    void  setDisplayedRows (int)
*	   - Specifies the number of rows in the MultilineInput object.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *)
*          - Creates the XView object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setMultilineInputDefaults ()
*         - Initializes data members.
*
*C-*/
#ifndef _MULTILINE_INPUT_H
#define _MULTILINE_INPUT_H

// Includes
#include "UIInputTextItem.h"

// Class Definition
class MultilineInput : public UIInputTextItem {
public:
    		  MultilineInput ();
    		  MultilineInput (char *);
		  MultilineInput (Image &);
   		 ~MultilineInput ();

    char	 *getValue (char *&);
    void	  setValue (char *);

    void	  setDisplayedRows (int);

protected:
    char *value;
    int   numRows;

    virtual void  createObject (UIObject *);

private:    
    void setMultilineInputDefaults ();
};

#endif /* _MULTILINE_INPUT_H */
