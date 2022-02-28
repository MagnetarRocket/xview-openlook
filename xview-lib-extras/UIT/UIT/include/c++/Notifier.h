/*H+ Notifier.h
*
*    SCCS ID: @(#)Notifier.h 1.2 93/04/09 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE: 
*
*    Defines the Notifier class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       09/10/90   Original creation
*    JDZ       05/01/91   Preparation for code review
*
*
*H-*/

/*C+ Notifier
*
*    DESCRIPTION:
*
*    A Notifier object is used to turn control of the 
*    application over to the windowing system.  When you do this,
*    the application is basically suspended until the user 
*    completes some interaction with the graphical objects 
*    (i.e., selects a Button, enters information into an 
*    AlphaNumericalInput field, attempts to QUIT the application 
*    by chosing the QUIT option from the BaseWindow's menu). 
*    When you turn control over to the windowing system, your 
*    application is then notified  of any events if you have 
*    written any Notify handlers, Event handlers, Done handlers 
*    or Repaint handlers.
*
*    INHERITED CLASSES:
*
*    UIObject - In addition to the normal benefits provided by the 
*    inheritance mechanism, inheriting the UIObject allows this
*    class to be used as UIObject class inside the Handlers.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void       start ()            - Starts running the Notifier.
*    The Notifier object displays the first BaseWindow instantiated.
*
*    void       dispatchEvent ()    - Dispatches the next XView event and 
*    returns control to the caller
*
*    void       stop ()		    - Stop the XView notifier.
*
*C-*/
#ifndef _NOTIFIER_H
#define _NOTIFIER_H

// Includes
#include "UIObject.h"

#include <xview/win_input.h>

// Class Definition
class Notifier : public UIObject {
public:
    			Notifier () {}
			~Notifier () {}

    void		start ()
			{ createXViewObjects ();
			  xv_main_loop (baseWindow->getXViewObject ()); }

    void		dispatchEvent ()
                        { createXViewObjects ();
			  notify_dispatch (); }

    void		stop ()
			{ notify_stop (); }
};

#endif /* _NOTIFIER_H */
