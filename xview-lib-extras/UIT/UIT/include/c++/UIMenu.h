/*H+ UIMenu.h
*
*    SCCS ID: @(#)UIMenu.h 1.3 92/05/23 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the UIMenu class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       10/06/90   Original creation.
*
*H-*/

/*C+ UIMenu
*
*    DESCRIPTION:
*
*    Defines an abstract data type that forms the base class for the menu
*    classes in the User Interface Components.
*
*    INHERITED CLASSES:
*
*    UIObject - The base class of all UI classes in the UIC.
*
*    LOCAL CLASSES:
*
*    MenuItem - Defines the contents of a menu item, including the handler
*               function.
*
*               ORDINARY DATA MEMBERS:
*
*               char               *label; - The string label of the item.
*               Image              *image; - The image label of the item.
*               UIMenu             *menu;  - The pullright menu of the item.
*               UIWindow           *window; - The window to show when the
*                                             item is chosen.
*               Boolean	            showWindow; - Specifies whether or not the
*                                                 item's window should be shown
*                                                 when the item is chosen.
*               UICHandlerFunction  handler; - The notify handler for the item.
*               int                 colorIndex; - The color of the item.
*               Boolean             active; - Specifies Whether or not the
*                                             item is active.
*
*               PUBLIC MEMBER FUNCTIONS:
*
*               void clear () - Cleans up and clears the data members of
*                               the item.
*               void initialize () - Initializes the the data members of
*                                    the item.
*
*    ORDINARY DATA MEMBERS:
*
*    int                 defaultItem   - The default menu item.
*    Boolean             dynamic       - If set, the last XView menu
*					 instantiated can be changed
*					 dynamically.
*    int                 lastSelected  - The last choice selected.
*    GenericList         menuItems     - The menu items in the menu.
*    UIObject           *menuParent    - The parent UIC object of the menu.
*    UICHandlerFunction  notifyHandler - The user specified menu notify handler.
*    int                 numColumns    - The number of columns to be displayed.
*    int                 numRows       - The number of rows to be displayed.
*    Menu                showMenu      - The XView menu that is shown
*					 when the show member function is used.
*    int                 showMenuDisplayedCount
*				       - The counter used to keep track of the
*				         number of menus being shown.
*    GenericList         notifyTargets - The list of targets to be notified
*    					 when a notify occurs on this menu.
*    Boolean		 notifyHandlerSet - TRUE if the XView notify handler
*    has been set.
*    Boolean		 useDefaultNotifyHandlerFlag - TRUE if the default
*    notify handler should be used.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setDisplayedColumns (int) - Sets ths number of columns to display.
*                                     Default: Not set.
*
*    void setDisplayedRows (int)    - Sets the number of rows to display.
*                                     Default: Not set.
*
*    virtual int    getChoice (int &)
*                                   - Returns the number of the last choice
*                                     selected.  Returns 0 if nothing has
*                                     been selected.
*    virtual char  *getChoice (char* &)
*                                   - Returns the label of the last choice
*                                     selected.  Returns NULL if no label
*                                     exists.
*    virtual Image *getChoice (Image* &)
*                                   - Returns the pointer to the image of the
*                                     last choice selected.  Returns NULL if
*                                     no label exists.
*
*    void setNotifyHandler (UICHandlerFunction)
*                                   - Sets the menu notify handler.
*                                     Default: Not set.
*
*    void setDefaultItem (int)
*    void setDefaultItem (char *)
*    void setDefaultItem (Image &)  - Sets the default menu item.
*                                     Default: Not set.
*
*    int addItem (char *)
*    int addItem (char *, UICHandlerFunction)
*    int addItem (char *, UIMenu &)
*    int addItem (Image &)
*    int addItem (Image &, UICHandlerFunction)
*    int addItem (Image &, UIMenu&) - Adds an item to the menu.
*
*    void setItemColor (int, Color)
*    void setItemColor (int, char *)
*    void setItemColor (char *, Color)
*    void setItemColor (char *, char *)
*    void setItemColor (Image &, Color)
*    void setItemColor (Image , char *)
*                                   - Sets the color of an item.
*
*    void setItemHandler (int, UICHandlerFunction)
*    void setItemHandler (char *, UICHandlerFunction)
*    void setItemHandler (Image&, UICHandlerFunction)
*                                   - Sets an item handler proc.
*
*    void setItemMenu (int)
*    void setItemMenu (char *)
*    void setItemMenu (Image &)     - Sets an item pullright menu.
*
*    void setItemWindow (int, UIWindow &)
*    void setItemWindow (char *, UIWindow &)
*    void setItemWindow (Image &, UIWindow &)
*				    - Specifies a window to popup when the
*                                     item is selected.
*
*    void setItemShowWindow (int, Boolean)
*			            - Specifies whether or not item's window
*				      should be shown when the item is chosen.
*
*    void deleteItem (int)
*    void deleteItem (char *)
*    void deleteItem (Image &)      - Deletes the specified item from the menu.
*
*    void setItemActive (int)
*    void setItemActive (char *)
*    void setItemActive (Image &)   - Turns on or off the selectability of the
*                                     specified item.
*
*    void replaceItem (int, char *)
*    void replaceItem (int, char *, UICHandlerFunction)
*    void replaceItem (int, char *, UIMenu &)
*    void replaceItem (int, Image &)
*    void replaceItem (int, Image &, UICHandlerFunction)
*    void replaceItem (int, Image &, UIMenu &)
*    void replaceItem (char *, char *)
*    void replaceItem (char *, char *, UICHandlerFunction)
*    void replaceItem (char *, char *, UIMenu &)
*    void replaceItem (char *, Image &)
*    void replaceItem (char *, Image &, UICHandlerFunction)
*    void replaceItem (char *, Image &, UIMenu &)
*    void replaceItem (Image&, char *)
*    void replaceItem (Image&, char *, UICHandlerFunction)
*    void replaceItem (Image&, char *, UIMenu &)
*    void replaceItem (Image&, Image &)
*    void replaceItem (Image&, Image &, UICHandlerFunction)
*    void replaceItem (Image&, Image &, UIMenu &)
*                          - Replaces the specified item with a new
*                                     label or image.
*
*    void setPinnable (Boolean)     - Sets a flag indicating whether or not
*                                     the menu is pinnable.
*                                     Default: FALSE.
*
*    void setDynamic (Boolean)      - Sets a flag indicating whether or not
*				      the last XView instantiation can be
*				      changed dynamically.
*                                     Default: FALSE.
*
*    void show (UIObject *)         - Shows the menu.  Once called, the same
*                                     XView menu object will be used for each
*                                     "show" call until "refreshShowMenu" is
*                                     called.
*
*    void refreshShowMenu ()        - Creates a new XView menu object to be
*                                     used by the "show" member function.  This
*                                     should only be called if the menu is
*                                     not currently displayed via a "show"
*                                     call.  This member function is only
*                                     needed to deal with the showing of
*                                     dynamically changing menus via event
*                                     handlers.
*
*    Boolean isShowMenuDisplayed () - Indicated whether or not the menu
*                                     is currently displayed via a "show"
*                                     call.  This can be used to decide whether
*                                     or not a call "refreshShowMenu" is safe.
*
*    int addNotifyTarget (UIObject &,
*			  UICTargetFunction,
*			  int xview_event_action = UIC_NOT_SET,
*			  int xview_event_id = UIC_NOT_SET)
*                                   - Adds the specified object as a target to
*                                     be notified when the UIMenu is notified.
*				      The target function is called with the
*				      pointer to the UIMenu and the target
*				      object.  If an event action or ID is
*				      specified, then the target is notified
*				      only when an event with that action or
*				      ID occurs.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    void notifyNotifyTargets ()    - Notifies all of the targets associated
*                                     with this component.
*
*    virtual void setXViewMenuHandler ()
*				    - Sets the handler on the XView menu.
*    virtual void setXViewMenuItemHandler (Menu_item)
*                                   - Sets the handler on the XView menu item.
*
*    void useDefaultNotifyHandler (Boolean) - Spcifies if the default
*    notify handler should be used.
*
*    virtual void defaultNotifyHandler () - The default notify handler.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void showMenuDoneHandler (Menu)
*                                   - Turns of the show menu and helps keep
*				      track of how many times it is actually
*				      being shown.
*
*    virtual void updateSelected (int, int)
*				    - Used to updates the list of selected
*				      items after an item has been deleted
*				      from the menu.
*
*C-*/
#ifndef _UIMENU_H
#define _UIMENU_H

// Includes
#include "Image.h"
#include "UIObject.h"
#include "UIWindow.h"

// Macro Definitions
#define UIMENU_ITEM_NUM_KEY    (UIC_OBJECT_KEY+1)

// Local Class Definitions
class UIMenu;
class MenuItem : public Generic {
 public:
  MenuItem () { initialize (); }
  ~MenuItem () { if (label) delete label; }
  void clear () { if (label) delete label;
		  initialize (); }
  void initialize () { label = (char *) NULL;
		       image = (Image *) NULL;
		       menu = (UIMenu *) NULL;
		       window = (UIWindow *) NULL;
		       showWindow = TRUE;
		       handler = (UICHandlerFunction) NULL;
		       colorIndex = UIC_NOT_SET;
		       active = TRUE; 
		     }
  char               *label;
  Image              *image;
  UIMenu             *menu;
  UIWindow           *window;
  Boolean	      showWindow;
  UICHandlerFunction  handler;
  int                 colorIndex;
  Boolean             active;
};

// Class Definition
class UIMenu : public UIObject {
 public:
  UIMenu () { defaultItem = UIC_NOT_SET;
              lastSelected = UIC_NOT_SET;
	      numColumns = UIC_NOT_SET;
	      numRows = UIC_NOT_SET;
	      notifyHandler = (UICHandlerFunction) NULL;
	      pinnable = FALSE;
	      dynamic = FALSE;
	      showMenuDisplayedCount = 0;
	      notifyHandlerSet = FALSE;
	      isUIMenu = TRUE;
	      useDefaultNotifyHandlerFlag = FALSE;
	    }
  ~UIMenu () {};

  void           setDisplayedColumns (int columns)
    { numColumns = columns;
      if (dynamic && owObject)
	xv_set (owObject, MENU_NCOLS, numColumns, NULL); }

  void           setDisplayedRows (int rows)
    { numRows = rows;
      if (dynamic && owObject)
	xv_set (owObject, MENU_NROWS, numRows, NULL); }

  virtual int    getChoice (int &choice)
    { return choice = lastSelected; }

  virtual char  *getChoice (char* &label)
    {
      if (lastSelected) {
	char *tmp = ((MenuItem *) menuItems [lastSelected-1])->label;
        label = strcpy (new char [strlen (tmp) + 1], tmp);
      }
      else
	label = (char *) NULL;
      return label;
    }

  virtual Image *getChoice (Image* &image)
    {
      if (lastSelected)
        image = ((MenuItem *) menuItems [lastSelected-1])->image;
      else
	image = (Image *) NULL;
      return image;
    }

  void           setNotifyHandler (UICHandlerFunction);

  int            addItem (char *);
  int            addItem (char *, UICHandlerFunction);
  int            addItem (char *, UIMenu &);
  int            addItem (Image &);
  int            addItem (Image &, UICHandlerFunction);
  int            addItem (Image &, UIMenu &);

  void           setItemColor (int, Color);
  void           setItemColor (int, char *);
  void           setItemColor (char *, Color);
  void           setItemColor (char *, char *);
  void           setItemColor (Image &, Color);
  void           setItemColor (Image &, char *);

  void           setItemHandler (int, UICHandlerFunction);
  void           setItemHandler (char *, UICHandlerFunction);
  void           setItemHandler (Image &, UICHandlerFunction);

  void           setItemMenu (int, UIMenu &);
  void           setItemMenu (char *, UIMenu &);
  void           setItemMenu (Image &, UIMenu &);

  void           setItemWindow (int, UIWindow &);
  void           setItemWindow (char *, UIWindow &);
  void           setItemWindow (Image &, UIWindow &);

  void           setItemShowWindow (int item, Boolean flag)
    { 
      if ((item > 0) && (item <= menuItems.getSize ()))
        ((MenuItem *) menuItems [item-1])->showWindow = flag;
    }

  void           setDefaultItem (int);
  void           setDefaultItem (char *);
  void           setDefaultItem (Image &);

  void           deleteItem (int);
  void           deleteItem (char *);
  void           deleteItem (Image &);

  void           setItemActive (int, Boolean);
  void           setItemActive (char *, Boolean);
  void           setItemActive (Image &, Boolean);

  void           replaceItem (int, char *);
  void           replaceItem (int, char *, UICHandlerFunction);
  void           replaceItem (int, char *, UIMenu &);
  void           replaceItem (int, Image &);
  void           replaceItem (int, Image &, UICHandlerFunction);
  void           replaceItem (int, Image &, UIMenu &);
  void           replaceItem (char *, char *);
  void           replaceItem (char *, char *, UICHandlerFunction);
  void           replaceItem (char *, char *, UIMenu &);
  void           replaceItem (char *, Image &);
  void           replaceItem (char *, Image &, UICHandlerFunction);
  void           replaceItem (char *, Image &, UIMenu &);
  void           replaceItem (Image &, char *);
  void           replaceItem (Image &, char *, UICHandlerFunction);
  void           replaceItem (Image &, char *, UIMenu &);
  void           replaceItem (Image &, Image &);
  void           replaceItem (Image &, Image &, UICHandlerFunction);
  void           replaceItem (Image &, Image &, UIMenu &);

  void           setPinnable (Boolean flag)
    {
      pinnable = flag;

      if (owObject) {
	if (pinnable) {
	  if (label)
	    xv_set (owObject,
		    MENU_GEN_PIN_WINDOW, baseWindow->getXViewObject (), label,
		    NULL);
	  else
	    xv_set (owObject,
		    MENU_GEN_PIN_WINDOW, baseWindow->getXViewObject (), "",
		    NULL);
	}
	else {
	  xv_set (owObject,
		  MENU_GEN_PIN_WINDOW, NULL, NULL,
		  NULL);
	}
      }
    }

  void           setDynamic (Boolean flag) { dynamic = flag; }

  void 	         show (UIObject *);
  void           refreshShowMenu ();
  Boolean        isShowMenuDisplayed () { return ((showMenuDisplayedCount > 0) ?
						TRUE : FALSE); }
  int	         addNotifyTarget (UIObject&,
				  UICTargetFunction,
				  int xview_event_action = UIC_NOT_SET,
				  int xview_event_id = UIC_NOT_SET);

protected:
  int                 defaultItem;
  Boolean             dynamic;
  int                 lastSelected;
  GenericList         menuItems;
  UIObject	     *menuParent;
  UICHandlerFunction  notifyHandler;
  int                 numColumns;
  int                 numRows;
  Boolean             pinnable;
  Menu                showMenu;
  int                 showMenuDisplayedCount;
  GenericList	      notifyTargets;
  Boolean	      notifyHandlerSet;
  Boolean	      useDefaultNotifyHandlerFlag;

  void                notifyNotifyTargets ();

  virtual void        setXViewMenuHandler ();
  virtual void        setXViewMenuItemHandler (Menu_item);

  void		      useDefaultNotifyHandler (Boolean flag)
		      { useDefaultNotifyHandlerFlag = flag; }

  virtual void defaultNotifyHandler () {}

private:
  static  void        showMenuDoneHandler (Menu);
  virtual void	      updateSelected (int, int);
};

#endif /* _UIMENU_H */
