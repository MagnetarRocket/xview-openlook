/*H+ StackChoice.h
*
*    SCCS ID: @(#)StackChoice.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the StackChoice class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       05/10/91   Prepare for code review.
*
*H-*/

/*C+ StackChoice
*
*    DESCRIPTION:
*
*    A StackChoice object provides a pull-down menu that displays a stack
*    of choices the user may selected.  Along with the menu selection arrow,
*    the StackChoice displays the value of the last selected choice.  A
*    StackChoice object cannot be multiple choice.
*
*    INHERITED CLASSES:
*
*    UIChoice - All choice classes are derived from UIChoice.
*
*    ORDINARY DATA MEMBERS:
*
*    defaultChoice - The default choice selected when the pulldown arrow
*		     is clicked.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setDefaultChoice (int) - Specifies the default choice if the
*    user clicks the pulldown arrow.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *parent) - After being supplied
*    with the parent of the StackChoice object (a ComponentDisplay),
*    this function creates the XView object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setStackChoiceDefaults () - Initializes data members.
*
*C-*/
#ifndef _STACK_CHOICE_H
#define _STACK_CHOICE_H

// Includes
#include "UIChoice.h"

// Class Definition
class StackChoice : public UIChoice {
public:
    		StackChoice ();
    		StackChoice (char *);
		StackChoice (Image &);
	       ~StackChoice ();

	  void	setDefaultChoice (int);

protected:
  int		defaultChoice;
  virtual void  createObject (UIObject *);

private:
  	  void	setStackChoiceDefaults ();
};

#endif /* _STACK_CHOICE_H */
