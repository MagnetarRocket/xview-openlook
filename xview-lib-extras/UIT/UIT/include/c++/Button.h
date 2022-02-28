/*H+ Button.h
*
*    SCCS ID: @(#)Button.h 1.3 93/02/08 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*    
*    Defines the Button class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/09/91   Preparation for code review
*
*H-*/

/*C+ Button
*
*    DESCRIPTION:
*
*    A Button object lets the user to invoke a function or a menu.
*    It may contain a label or a graphic image.  Menu buttons contain a
*    triangle or mark pointing in the direction in which the menu
*    will be displayed.  A menu could provide alternative values 
*    or functions that the button may invoke.
*
*    INHERITED CLASSES:
*
*    UIComponent - Defines the generic characteristics of 
*    components in all GUI applications.
*
*    ORDINARY DATA MEMBERS:
*
*    UIMenu 	 *buttonMenu - Pointer to the menu that is to 
*    appear when the user selects this button.
*
*    int          labelWidth - width of the label.
*
*    Boolean      abbreviated - TRUE if this button is an 
*    abbreviated one.
*
*    UIWindow    *window - Pointer to the window where this 
*    button is to be displayed.
*
*    Boolean	  showWindow - TRUE if the window is open.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*            void	 setMenu (UIMenu &) - Associates the UIMenu 
*            with the BUtton object. When the user selects this 
*            Button with the right mouse button, the Menu will appear.  
*            Default: No associated menu.
*
*            void        setAbbreviated (Boolean) - Sets whether or not 
*            this Button is an abbreviated one (one which has a pull 
*            down menu associated with it).  Default: Not abbreviated.
*
*            void	 setLabelWidth (int) - Sets the width of the 
*            area where the label on the Button is to be displayed.  
*            Default: XView default.
*
*	     int         getLabelWidth () - Gets the button's label width.
*
*            void        setWindow (UIWindow &) - Associates the UIWindow 
*            with the Button object.  When the user selects this Button 
*            with the left mouse button, the UIWindow will appear.  
*            Default: No associated window.
*
*            void        setShowWindow (Boolean) - Specifies whether or not
*            the associated window should be shown when the Button is pressed.
*            Default: TRUE.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void 	 createObject (UIObject *) - Create XView object.
*
*    static  void        notifyProc (Panel_item, Event *) -  User
*            defined function that is to be executed when an event
*            is noted in this Button object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*            void	 setButtonDefaults () - Initializes data members.
*
*C-*/
#ifndef _BUTTON_H
#define _BUTTON_H

// Includes
#include "NoticePrompt.h"
#include "UIComponent.h"
#include "UIMenu.h"
#include "UIWindow.h"

// Class Definition
class Button : public UIComponent {
public:
    		 	 Button ();
    		 	 Button (char *);
		 	 Button (Image &);
			~Button ();

    void		 setMenu (UIMenu &);

    void		 setAbbreviated (Boolean flag)
			 { abbreviated = flag; }

    void		 setLabelWidth (int width)
			 { labelWidth = width;
			   if (owObject) {
			     if (resourceName)
			       xv_set (owObject,
					 XV_USE_DB,
				         PANEL_LABEL_WIDTH, labelWidth,
					 NULL,
				       NULL);
			     else
			       xv_set (owObject,
				       PANEL_LABEL_WIDTH, labelWidth,
				       NULL);
			   }
			 }

    int 	         getLabelWidth ()
			 {
  			   return (owObject ?
			           (int) xv_get (owObject, PANEL_LABEL_WIDTH) :
			           labelWidth);
  			 }

    void                 setWindow (UIWindow &button_window)
			 { window = &button_window; }
    void		 setShowWindow (Boolean flag)
			 { showWindow = flag; }

protected:
    UIMenu 	 	*buttonMenu;
    int		 	 labelWidth;
    Boolean		 abbreviated;
    UIWindow            *window;
    Boolean		 showWindow;
    
    virtual void 	 createObject (UIObject *);
    static  void	 notifyProc (Panel_item, Event *);

private:
    void		 setButtonDefaults ();
};

#endif /* _BUTTON_H */
