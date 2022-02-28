/*H+ TermDisplay.h
*
*    SCCS ID: @(#)TermDisplay.h 1.2 92/05/23 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the TermDisplay class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/07/91   Preparation for code review
*
*H-*/

/*C+ TermDisplay
*
*    DESCRIPTION:
*
*    The class provides such terminal emulation capability as a TTY 
*    subwindow, which is like having a shell tool available. An application 
*    can be run or an interactive shell can be started.  The window has the 
*    standard XView default pop-up menu with associated pull-right menus 
*    that provide the user with basic functions like HISTORY, EDIT, FIND, etc.
*
*    INHERITED CLASSES:
*
*    UIDisplay - Supplies basic display capabilities.
*
*    ORDINARY DATA MEMBERS:
*
*    char	**ttyArgs - Pointer to the parameters that define the terminal.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView object.
*
*C-*/
#ifndef _TERM_DISPLAY_H
#define _TERM_DISPLAY_H

// Includes
#include "UIDisplay.h"

#include <xview/win_input.h>
#include <xview/termsw.h>
#include <xview/tty.h>

// Class Definition
class TermDisplay : public UIDisplay {
public:
    		  TermDisplay ();
		  TermDisplay (char *, ...);
		 ~TermDisplay ();

protected:
    char	**ttyArgs;

    virtual void  createObject (UIObject *);
};

#endif /* _TERM_DISPLAY_H */
