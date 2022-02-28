/*H+ PopUpWindow.h
*
*    SCCS ID: @(#)PopUpWindow.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the PopUpWindow class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/06/91   Preparation for code review
*
*H-*/

/*C+ PopUpWindow
*
*    DESCRIPTION:
*
*    A PopUpWindow object is supplemental window which may be created at any 
*    time within application.  It displays information or separates specific 
*    input fields into a logical grouping.  By default, it remains displayed 
*    on the screen and contains a label across the top of the frame.  It is 
*    similar to the BaseWindow in that it can contain a number of UIDisplay 
*    objects.  A PopUpWindow is associated  with the application's BaseWindow.
*
*    INHERITED CLASSES:
*
*    UIWindow - Provides the foundation and basic framework for all GUI 
*    windows.
*
*    ORDINARY DATA MEMBERS:
*
*    Boolean	 pinned - TRUE if window is pinned.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*             void   setPinned (Boolean) - Defines this window as pinned or
*             not pinned.  Default: Not pinned.
*
*    virtual  void   show (Boolean) - Specifies if the PopUpWindow should be 
*             displayed.  Default: FALSE.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual  void   createObject (UIObject *parent = NULL) - Creates the
*             XView object.
*
*    static   void   doneProc (Frame) - Internal wrapper for an XView
*	      frame done proc on this window.  When the window is dismissed,
*             the destroy function is called.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*             void   setPopUpWindowDefaults () - Initializes the data members.
*
*C-*/
#ifndef _POPUPWINDOW_H
#define _POPUPWINDOW_H

// Includes
#include "UIWindow.h"

// Class Definition
class PopUpWindow : public UIWindow {
public:
    		 PopUpWindow ();
    		 PopUpWindow (char *);
   		~PopUpWindow ();

    void 	 setPinned (Boolean);
    virtual void show (Boolean);

protected:
    Boolean	 pinned;

    virtual void createObject (UIObject *parent = NULL);
    static  void doneProc (Frame);

private:
    void	 setPopUpWindowDefaults ();
};

#endif /* _POPUPWINDOW_H */
