/*H+ ToggleChoice.h
*
*    SCCS ID: @(#)ToggleChoice.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the ToggleChoice class.
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

/*C+ ToggleChoice
*
*    DESCRIPTION:
*
*    A ToggleChoice object displays one or more choices horizontally
*    and/or vertically.  The user may select one or more of the choices
*    depending on whether or not the object has been specified as a multiple
*    object.
*
*    INHERITED CLASSES:
*
*    UIChoice - All choice classes are derived from UIChoice.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *parent) - After being supplied
*    with the parent of the ToggleChoice object (a ComponentDisplay),
*    this function creates the XView object.
*
*C-*/
#ifndef _TOGGLE_CHOICE_H
#define _TOGGLE_CHOICE_H

// Includes
#include "UIChoice.h"

// Class Definition
class ToggleChoice : public UIChoice {
public:
    	        ToggleChoice ();
    	        ToggleChoice (char *);
	        ToggleChoice (Image &);
	       ~ToggleChoice ();

protected:
  virtual void	createObject (UIObject *);
};

#endif /* _TOGGLE_CHOICE_H */
