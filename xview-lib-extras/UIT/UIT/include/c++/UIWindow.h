/*H+ UIWindow.h
*
*    SCCS ID: @(#)UIWindow.h 1.5 93/04/09 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE: 
*
*    Defines the UIWindow class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       04/30/91   Preparation for code review
*
*H-*/

/*C+ UIWindow
*
*    DESCRIPTION:
*
*    It describes such features as whether the window is resizable, whether 
*    its footer displays, the window's height and width and setting a function 
*    that executes when a user attempts to destroy the window.  The Derived 
*    Classes are specific instances of a window, and define how the window 
*    will look and behave, such as whether it is a base window for the entire 
*    application or whether it pops up when a certain action occurs.
*
*    INHERITED CLASSES:
*
*    UIObject - The base class of all the UI classes in the UIC.
*    
*    ORDINARY DATA MEMBERS:
*
*    GenericList	 displayList   - Lists UIDisplay objects that are
*    contained in this window.
*   
*    Boolean		 resizable     - TRUE if window is resizable.
*
*    Boolean	 	 displayFooter - TRUE if the footer should be displayed.
*
*    UICHandlerFunction	 doneHandler   - The function to execute when the user 
*    attempts to unpin or DISMISS a PopUpWindow or QUIT a BaseWindow.
*
*    char               *leftFooter    - Pointer to the left footer.
*
*    char   		*rightFooter   - Pointer to the right footer.
*
*    Boolean             fitHeight     - TRUE if the window should fit to the 
*    height around the Displays it contains in the specified Dimension.
*
*    Boolean             fitWidth      - TRUE if the window should fit to the
*    width around the Displays it contains in the specified Dimension.
*
*    GenericList         childWindows  - Contains list of all children windows.
*
*    static GenericList *peerWindows   - Pointer to the list of all peers 
*    windows.
*
*    UIWindow		*parentWindow  -  Pointer to the parent window.
*
*    Boolean		 useDefaultDoneHandlerFlag - TRUE if the default done
*    handler should be used.
*
*    XID	         newXParent   - The new XID to reparent this object to.
*
*    int	         newXParentX  - The X coordinate within the new parent.
*
*    int	         newXParentY  - The Y coordinate within the new parent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*	     void    focusNextWindow () - Set the focus to the next
*	     window that can accept the input focus.
*
*	     void    focusPreviousWindow () - Set the focus to the previous
*	     window that can accept the input focus.
*
*            void    setResizable (Boolean flag) - Sets whether or not the
*            window is resizable.  Default: PopUpWindow is resizable.
*
*            void    setDisplayFooter (Boolean footer) - Sets whether or not
*            the window's footer should be displayed.  Default: Displayed.
*
*            void    setLeftFooter (char *footer) - Specifies the string to
*            be displayed as the window's left footer.
*
*            void    setRightFooter (char *footer) - Specifies the string to
*            be displayed as the window's right footer.
*
*            char   *getLeftFooter () - Gets the left footer of the window.
*	     The string returned is newly allocated and must be freed by
*	     the caller.
*
*            char   *getRightFooter () - Gets the right footer of the window.
*	     The string returned is newly allocated and must be freed by
*	     the caller.
*
*            void    setFit (Dimension fit) - Depending on the value of the 
*            Dimension argument (HEIGHT or WIDTH), specifies that this window 
*            should fit to size around the Displays it contains in the 
*            specified Dimension.  Default: Don't fit the window.
*
*    virtual void    setDoneHandler (UICHandlerFunction) - Sets the function to
*    execute when the user attempts to unpin or DISMISS a PopUpWindow or QUIT 
*    a BaseWindow.  Default: Not set.
*
*            void    addChildWindow (UIWindow &window) - Specifies that the 
*            argument is a child window of the window.
*
*            void    addDisplay (UIDisplay &display) - Adds the UIDisplay
*            object to the list of display objects contained in this window. 
*
*            void    removeDisplay (UIDisplay &display) - Removes the UIDisplay
*            object from the list of display objects contained in this window. 
*
*            UIDisplay *traverseDisplays (Boolean reset_flag) - Traverses
*	     the displays that belong to this window.
*
*	     void    reparentXWindow (XID, int x, int y) - Allows this object
*	     to be reparented to another X window.  This will work properly
*	     if the XID is from a BaseWindow or a PopUpWindow, even if
*	     the new parent is being displayed by another process.  The x
*	     and y arguments allow positioning of the UIDisplay in the new
*            parent window.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    void useDefaultDoneHandler (Boolean) - Specifies if the default done
*    handler should be used.
*
*    virtual void defaultDoneHandler () - The default done handler.
*
*    static void eventProc (Xv_window, Event *) - Internal wrapper
*           around the XView event proc for objects of classes derived
*           from UIWindow.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void           setUIWindowDefaults () - Sets all the defaults values that 
*    have been previously defined.  This function is mainly used by the 
*    constructor to initialize a newly created UIWindow object.
*
*C-*/
#ifndef _UIWINDOW_H
#define _UIWINDOW_H

// Includes
#include "GenericList.h"
#include "UIObject.h"
#include "UIDisplay.h"

#include <sys/param.h>
#include <xview/rect.h>
#include <xview/win_input.h>
#include <pixrect/pixrect.h>
#include <pixrect/pixfont.h>
#include <xview/frame.h>

// Class Definition
class UIWindow : public UIObject {
public:
    		 	 UIWindow ();
    		 	 UIWindow (char *);
   			~UIWindow ();

    void		 focusNextWindow ()
			 {
			   if (owObject)
			     xv_set (owObject, FRAME_NEXT_PANE, NULL);
			 }

    void		 focusPreviousWindow ()
			 {
			   if (owObject)
			     xv_set (owObject, FRAME_PREVIOUS_PANE, NULL);
			 }

    void		 setResizable (Boolean flag)
			 { resizable = flag;
			   if (owObject)
			     xv_set (owObject,
				     FRAME_SHOW_RESIZE_CORNER, resizable,
				     NULL); }

    void	 	 setDisplayFooter (Boolean footer)
			 { displayFooter = footer;
			   if (owObject)
			     xv_set (owObject,
				     FRAME_SHOW_FOOTER, displayFooter,
				     NULL); }

    void                 setLeftFooter (char *footer)
			 { leftFooter = (footer ?
					 strcpy (new char [strlen (footer) + 1],
						 footer) :
					 (char *) NULL);
			   if (owObject)
			     xv_set (owObject,
				     FRAME_LEFT_FOOTER, leftFooter,
				     NULL); }

    void                 setRightFooter (char *footer)
			 { rightFooter = (footer ?
					 strcpy (new char [strlen (footer) + 1],
						 footer) :
					 (char *) NULL);
			   if (owObject)
			     xv_set (owObject,
				     FRAME_RIGHT_FOOTER, rightFooter,
				     NULL); }

    char		*getLeftFooter ()
			 { return (leftFooter ?
				   strcpy (new char [strlen (leftFooter) + 1],
					   leftFooter) :
				   (char *) NULL); } 

    char		*getRightFooter ()
			 { return (rightFooter ?
				   strcpy (new char [strlen (rightFooter) + 1],
					   rightFooter) :
				   (char *) NULL); } 

    void                 setFit (Dimension fit)
			 { if (fit == HEIGHT) {
			     fitHeight = TRUE;
			     if (owObject)
			       window_fit_height (owObject);
			   }
			   else {
			     fitWidth = TRUE;
			     if (owObject)
			       window_fit_width (owObject);
			   }
			 }

    virtual void	 setDoneHandler (UICHandlerFunction);

    void                 addChildWindow (UIWindow &window)
      { 
	childWindows.addItem (&window);
	
	int index = peerWindows->findItem (&window);
	if (index > -1)
	  (void) peerWindows->removeItem (index);
	
	window.parentWindow = this;
      }

    void 	 	 addDisplay (UIDisplay &display)
      {
	displayList.addItem (&display);

	if (owObject)
	  ((UIObject *) &display)->createObject (this);
      }

    void                 removeDisplay (UIDisplay &display)
      {
	int index = displayList.findItem (&display);

        if (index > -1) {
          (void) displayList.removeItem (index);
	  display.show (FALSE);
        }
      }

   UIDisplay		*traverseDisplays (Boolean reset_flag)
                         { return (UIDisplay *)
                                  displayList.traverse (reset_flag); }
 
    void                 reparentXWindow (XID xid, int x=0, int y=0)
                         {
                           newXParent = xid;
                           newXParentX = x;
                           newXParentY = y;
       
                           if (owObject)
                             (void)
       		             XReparentWindow ((Display *) xv_get (owObject,
       							          XV_DISPLAY),
                                              (XID) xv_get (owObject,
       						            XV_XID),
                                              xid, x, y);
                         }

protected:
    GenericList		 displayList;
    Boolean		 resizable;
    Boolean	 	 displayFooter;
    UICHandlerFunction	 doneHandler;
    char                *leftFooter;
    char   		*rightFooter;
    Boolean              fitHeight;
    Boolean              fitWidth;
    GenericList          childWindows;
    static GenericList  *peerWindows;
    UIWindow		*parentWindow;
    Boolean		 useDefaultDoneHandlerFlag;
    XID                  newXParent;
    int                  newXParentX;
    int                  newXParentY;

    void setUIWindowAttributes (UIObject *parent)
      {
        if (newXParent != (XID) UIC_NOT_SET) {
          XReparentWindow ((Display *) xv_get (owObject, XV_DISPLAY),
                           (XID) xv_get (owObject, XV_XID),
                           newXParent, newXParentX, newXParentY);
        }

        setUIObjectAttributes (parent);
      }

    void useDefaultDoneHandler (Boolean flag)
      { useDefaultDoneHandlerFlag = flag; }

    virtual void defaultDoneHandler () {}
    
#ifdef __GNUC__
public:
#endif
    static  void eventProc (Xv_window, Event *);

private:
    void setUIWindowDefaults ();
};
 
#endif /* _UIWINDOW_H */
