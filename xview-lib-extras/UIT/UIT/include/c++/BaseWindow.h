/*H+  BaseWindow.h
*
*    SCCS ID: @(#)BaseWindow.h 1.3 92/05/23 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE: 
*    
*    Defines the BaseWindow class.
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

/*C+ BaseWindow
*
*    DESCRIPTION:
*
*    A BaseWindow class further specifies the UIWindow class to by used by GUI
*    applications.  It can contain any UIDisplay object or PopUpWindow.
*
*    INHERITED CLASSES:
*
*    UIWindow - Provides the foundation and basic framework for all window 
*    based applications.
*
*    ORDINARY DATA MEMBERS:
*
*    Image              *icon - Pointer to the Image object that represents the
*    window when it is closed.
*
*    Boolean		 newIcon - TRUE if the icon image was created
*    internally in this object.
*
*    Image              *iconMask - Pointer to the icon mask that allows to
*    mask the parts of the icon user wants to see.  The parts not in the mask 
*    are seen as the workstation background.
*
*    Boolean		 newIcon - TRUE if the icon mask image was created
*    internally in this object.
*
*    int                 iconX - The X coordinate of the Icon's image location.
*
*    int                 iconY- The Y coordinate of the Icon's image location.
*
*    char		*iconLabel - The icon label.
*
*    Boolean             busyFlag - TRUE if the BaseWindow is busy.
*
*    Boolean             noConfirmFlag - TRUE if there is to be no confirmation
*    when the user chooses to quit the window.
*
*    NoticePrompt       *destroyPrompt - A notice prompt to be displayed when
*    the user has chosen to quit the window.
*
*    DestroyStatus       destroyStatus - The status associated with quitting 
*    a BaseWindow.  It contains the stage of destruction the BaseWindow is in 
*    after the user selected QUIT.
*
*    Boolean		 noDestroy - TRUE if the next destroy attempts
*    that provide a DESTROY_CHECKING status are to be aborted.
*
*    Boolean		 doneHandlerSet - TRUE if the XView destroy
*    function has been interposed.
*
*    UICHandlerFunction  saveHandler - Function that is executed when the
*    application is asked to save itself (via the save layout choice in
*    the OpenWindow menu).
*
*    UICHandlerFunction  iconEventHandler - Function that is executed when
*    an event occurs on the iconized form of this window.
*
*    Boolean		 iconEventHandlerSet - TRUE if the icon event handler
*    has been set.
*
*    Boolean		 useDefaultSaveHandlerFlag - TRUE if the default
*    save handler is to be used.
*
*    Boolean		 useDefaultIconEventHandlerFlag - TRUE if the default
*    icon event handler is to be used.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    virtual  void    show (Boolean) - Specifies whether or not this BaseWindow
*             is to be displayed.  Default: If main BaseWindow, display the
*             window, otherwise don't display the window.
*
*             void    setClosed (Boolean) - Sets BaseWindow to be iconified.
*             Default: Not iconified.
*
*             void    setIcon (Image&) - Associates an Image to this 
*             BaseWindow.  This is the icon that will appear when the 
*             window is closed.  Default: No icon associated with window.
*
*             void    setIcon (char*) - Associates an image to this 
*             BaseWindow.  The argument is the file name which contains the 
*             image specification.  This is the icon that appears when the 
*             window is closed.  Default: No icon associated with window.
*             
*             void    setIconMask (Image&) - Sets the icon's mask.  This, in 
*             combination with the icon that is set (via setIcon), determines 
*             how the icon will appear.  The argument is an Image object. 
*             that has previously been created.  The icon mask allows the user 
*             to mask the parts of the icon user wants to see.  The parts not 
*             in the mask are seen as the workstation background.
*             Default: No icon mask associated with window.
*
*             void    setIconMask (char *) -  Sets the icon's mask. The
*             argument in this case is the file name which contains the image 
*             specification.  Default: No icon mask associated with window.
*
*             void    setIconX (int) - Sets the X coordinate of the Icon's
*             image location.  Default: Not set.
*		     
*             void    setIconY (int) - Sets the Y coordinate of the Icon's
*             image location.  Default: Not set.
*		     
*             void    setIconLabel (char *) - Sets the label on the icon.
*
*             void    setBusy (Boolean) - TRUE if the BaseWindow is busy. If
*             so, then the cursor appears as an hourglass, and the input events
*             are allowed.  Default: FALSE.
*
*             void    setNoConfirm (Boolean) - Specifies whether or not
*             exiting the BaseWindow is to be confirmed.  Default: TRUE.
*
*             void    setDestroyPrompt (NoticePrompt&) - If the user selected 
*             the QUIT option from the window's menu, the destroy prompt is 
*             displayed.  If the destroy prompt returns NOTICE_NO, the QUIT 
*             is aborted.  Note that the NOTICE_NO implies that the developer
*             used the member function setYesAndNoLabels for the NoticePrompt
*             or used the default NoticePrompt behavior.  Default: Not set.
*
*             DestroyStatus getDestroyStatus () - Provides the destroy status 
*             associated with with quitting a BaseWindow.  It returns the 
*             stage of destruction the BaseWindow is in after the user 
*             selected QUIT.  For more information see the XView Programming 
*             manual, Section 19.9.5.
*
*             void    setNoDestroy (Boolean) - If set to TRUE, then
*	      destroy attempts that provide a DESTROY_CHECKING status are
*             aborted.
*
*             void    setSaveHandler (UICHandlerFunction) - Specifies 
*             a user defined handler to be executed when a save layout
*             request is made.  Default: Not set.
*
*             void    setIconEventHandler (UICHandlerFunction) - Specifies
*             a user defined handler to be executed when an event occurs
*             on the BaseWindow icon.  Default: Not set.
*
*    virtual  void    setDoneHandler (UICHandlerFunction) - Specifies a
*             function to be called if the BaseWindow is exited.
*             Default: Not set.
*
*             int     getIconX () - Gets the X coordinate of BaseWindow icon's
*             location.
*		     
*             int     getIconY () - Gets the Y coordinate of BaseWindow icon's
*             location.
*
*    virtual  Boolean isClosed () - TRUE if the BaseWindow is currently
*             iconized.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual  void    createObject (UIObject *parent=NULL) -
*    Create XView object.
*
*    void             setDestroyStatus (DestroyStatus) - Sets the destroyStatus
*    to a specified value.
*
*    void             refreshIcon () - Associates the newly specified icon
*    (via setIcon) with a BaseWindow that is already being displayed.
*
*    void useDefaultSaveHandler (Boolean) - Specifies whether or not to
*    use the default save handler.
*
*    void useDefaultIconEventHandler (Boolean) - Specifies whether or not to
*    use the default icon event handler.
*
*    virtual void defaultSaveHandler () - The default save handler.
*
*    virtual void defaultIconEventHandler () - The default icon event handler.
*
*    static Notify_value doneProc (Notify_client, Destroy_status) -
*    User defined function that is executed when the user selected the QUIT
*    option from the window's menu.  It allows user to control the order in
*    which the application destruction is taking place.
*
*    static Notify_value iconEventProc (Xv_window, Event *, Notify_arg, 
*                                 Notify_event_type) - Internal wrapper around
*    the XView event proc for events occuring on the iconified version
*    of the BaseWindow.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setBaseWindowDefaults () - Initializes data members.
*
*C-*/
#ifndef _BASEWINDOW_H
#define _BASEWINDOW_H

// Includes
#include "Image.h"
#include "NoticePrompt.h"
#include "UIWindow.h"

// Class Definition
class BaseWindow : public UIWindow {
public:
    		 BaseWindow ();
    		 BaseWindow (char *);
   		~BaseWindow ();

    virtual void show (Boolean);

    void	 setClosed (Boolean flag)
		 { closed = flag;
		   if (owObject)
		     xv_set (owObject, FRAME_CLOSED, closed, NULL); }

    void	 setIcon (Image &icon_image)
		 { if (newIcon) delete icon;
		   newIcon = FALSE;
		   icon = &icon_image;
		   refreshIcon (); }

    void  	 setIcon (char *icon_file)
		 { if (newIcon) delete icon;
		   newIcon = TRUE;
		   icon = new Image (icon_file);
		   refreshIcon (); }

    void	 setIconMask (Image &icon_image)
		 { if (newIconMask) delete icon;
		   newIcon = FALSE;
		   iconMask = &icon_image;
		   refreshIcon (); }

    void  	 setIconMask (char *icon_file)
		 { if (newIconMask) delete icon;
		   newIcon = TRUE;
		   iconMask = new Image (icon_file);
		   refreshIcon (); }

    void         setIconX (int x)
		 { iconX = x;
		   if (owObject) {
		     Icon icon = (Icon) xv_get (owObject, FRAME_ICON);
		     if (icon) xv_set (icon, XV_X, iconX, NULL);
                   }
		 }
		     
    void         setIconY (int y)
		 { iconY = y;
		   if (owObject) {
		     Icon icon = (Icon) xv_get (owObject, FRAME_ICON);
		     if (icon) xv_set (icon, XV_Y, iconY, NULL);
                   }
		 }

    void	 setIconLabel (char *icon_label)
		 { if (icon_label)
		     iconLabel = strcpy (new char [strlen (icon_label) +1],
					 icon_label);
		   else
		     iconLabel = (char *) NULL;
		   if (owObject) {
		     Icon icon = (Icon) xv_get (owObject, FRAME_ICON);
		     if (icon) xv_set (icon, XV_LABEL, iconLabel, NULL);
		   }
		 }
		     
    void         setBusy (Boolean flag)
		 { busyFlag = flag;
		   if (owObject)
		     xv_set (owObject, FRAME_BUSY, busyFlag, NULL); }

    void         setNoConfirm (Boolean flag)
		 { noConfirmFlag = flag;
		   if (owObject)
		     xv_set (owObject,
			     FRAME_NO_CONFIRM, noConfirmFlag,
			     NULL); }

    void         setDestroyPrompt (NoticePrompt &);

    DestroyStatus getDestroyStatus () { return destroyStatus; }

    void	 setNoDestroy (Boolean flag) { noDestroy = flag; }

    void         setSaveHandler (UICHandlerFunction);
    void         setIconEventHandler (UICHandlerFunction);

    virtual void setDoneHandler (UICHandlerFunction);

    int          getIconX ()
		 { if (owObject) {
		     Icon icon = (Icon) xv_get (owObject, FRAME_ICON);
		     if (icon) iconX = (int) xv_get (icon, XV_X);
		   }
                   return iconX;
		 }
		     
    int          getIconY ()
		 { if (owObject) {
		     Icon icon = (Icon) xv_get (owObject, FRAME_ICON);
		     if (icon) iconY = (int) xv_get (icon, XV_Y);
		   }
                   return iconY;
		 }

    virtual Boolean isClosed ()
		 { if (owObject)
		     closed = (Boolean) xv_get (owObject, FRAME_CLOSED);
		   return closed; }

protected:
    Image	       *icon;
    Boolean		newIcon;
    Image	       *iconMask;
    Boolean		newIconMask;
    int                 iconX;
    int                 iconY;
    char	       *iconLabel;
    Boolean             busyFlag;
    Boolean		noConfirmFlag;
    NoticePrompt       *destroyPrompt;
    DestroyStatus       destroyStatus;
    Boolean		noDestroy;
    Boolean		doneHandlerSet;
    UICHandlerFunction  saveHandler;
    UICHandlerFunction  iconEventHandler;
    Boolean		iconEventHandlerSet;
    Boolean		useDefaultSaveHandlerFlag;
    Boolean		useDefaultIconEventHandlerFlag;

    virtual void    createObject (UIObject *parent = NULL);
    
    void         setDestroyStatus (DestroyStatus status) 
		 { destroyStatus = status; }

    void         refreshIcon ()
		 { if (owObject && icon) {
		     Icon tempIcon;

		     ((UIObject *) icon)->createObject (this);

		     if (iconMask) {
		       ((UIObject *) iconMask)->createObject (this);

		       tempIcon = xv_create (owObject, ICON,
                                             ICON_IMAGE,
					     icon->getXViewObject (),
                                             ICON_MASK_IMAGE,
					     iconMask->getXViewObject (),
					     NULL);
		     }
		     else
		       tempIcon = xv_create (owObject, ICON,
                                             ICON_IMAGE,
					     icon->getXViewObject (),
				             NULL);

		     xv_set (owObject, FRAME_ICON, tempIcon, NULL);
		   }
                 }

    void	 useDefaultSaveHandler (Boolean flag)
		 { useDefaultSaveHandlerFlag = flag; }

    void	 useDefaultIconEventHandler (Boolean flag)
		 { useDefaultIconEventHandlerFlag = flag; }

   virtual void defaultSaveHandler () {}
   virtual void defaultIconEventHandler () {}

   static Notify_value doneProc (Notify_client, Destroy_status);

   static Notify_value iconEventProc (Xv_window,
				      Event *,
				      Notify_arg,
				      Notify_event_type);

private:
   void setBaseWindowDefaults ();
};

#endif /* _BASEWINDOW_H */
