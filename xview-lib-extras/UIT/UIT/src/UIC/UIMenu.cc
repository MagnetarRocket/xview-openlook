/*H+ UIMenu.cc
*
*    SCCS ID: @(#)UIMenu.cc 1.3 93/04/09 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the UIMenu class of the User Interface
*    Components.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       05/10/91   Preparation for code review. 
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)UIMenu.cc 1.3 93/04/09 SMI"; 
#endif

// Includes
#include <string.h>

#include "UIMenu.h"
#include "ColorMap.h"

// External Function Declarations

// Local Function Declarations

/*F+ int UIMenu::setNotifyHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Set the menu notify handler.
*
*    PARAMETERS:
*
*    handler - The notify handler.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setNotifyHandler (UICHandlerFunction handler)
{
  if (dynamic && owObject && !notifyHandlerSet)
    setXViewMenuHandler ();

  notifyHandler = handler;
}

/*F+ int UIMenu::addItem (char *label)
*
*    PURPOSE:
*
*    Add an item to the menu.
*
*    PARAMETERS:
*
*    label - The character string label of the menu item.
*
*    RETURN VALUES:
*
*    Returns the item number of the item added.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
int UIMenu::addItem (char *label)
{
  MenuItem *menu_item = new MenuItem;

  menu_item->label = strcpy (new char [strlen (label) + 1], label);
  menuItems.addItem (menu_item);

  if (dynamic && owObject) {
    int num_items = menuItems.getSize ();

    Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					   MENU_RELEASE,
					   XV_KEY_DATA, UIC_OBJECT_KEY, this,
					   XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                num_items,
					   MENU_STRING, menu_item->label,
					   NULL);

    xv_set (owObject, MENU_APPEND_ITEM, xview_menu_item, NULL);
  }

  return menuItems.getSize ();
}

/*F+ int UIMenu::addItem (char *label, UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Add an item to the menu.
*
*    PARAMETERS:
*
*    label   - The character string label of the menu item.
*    handler - The handler function for this menu item.
*
*    RETURN VALUES:
*
*    Returns the item number of the item added.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
int UIMenu::addItem (char *label, UICHandlerFunction handler)
{
  MenuItem *menu_item = new MenuItem;

  menu_item->label = strcpy (new char [strlen (label) + 1], label);
  menu_item->handler = handler;
  menuItems.addItem (menu_item);

  if (dynamic && owObject) {
    int num_items = menuItems.getSize ();

    Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					   MENU_RELEASE,
					   XV_KEY_DATA, UIC_OBJECT_KEY, this,
					   XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                num_items,
					   MENU_STRING, menu_item->label,
					   NULL);

    setXViewMenuItemHandler (xview_menu_item);

    xv_set (owObject, MENU_APPEND_ITEM, xview_menu_item, NULL);
  }

  return menuItems.getSize ();
}

/*F+ int UIMenu::addItem (char *label, UIMenu &menu)
*
*    PURPOSE:
*
*    Add an item to the menu.
*
*    PARAMETERS:
*
*    label - The character string label of the menu item.
*    menu  - The pullright menu for this menu item.
*
*    RETURN VALUES:
*
*    Returns the item number of the item added.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
int UIMenu::addItem (char *label, UIMenu &menu)
{
  MenuItem *menu_item = new MenuItem;

  menu_item->label = strcpy (new char [strlen (label) + 1], label);
  menu_item->menu = &menu;
  menuItems.addItem (menu_item);

  if (dynamic && owObject) {
    int num_items = menuItems.getSize ();

    menu.createObject (this);

    Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					   MENU_RELEASE,
					   XV_KEY_DATA, UIC_OBJECT_KEY, this,
					   XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                num_items,
					   MENU_STRING, menu_item->label,
					   MENU_PULLRIGHT,
					     menu.getXViewObject (),
					   NULL);

    xv_set (owObject, MENU_APPEND_ITEM, xview_menu_item, NULL);
  }

  return menuItems.getSize ();
}

/*F+ int UIMenu::addItem (Image &image)
*
*    PURPOSE:
*
*    Add an item to the menu.
*
*    PARAMETERS:
*
*    image - The menu item image.
*
*    RETURN VALUES:
*
*    Returns the item number of the item added.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
int UIMenu::addItem (Image &image)
{
  MenuItem *menu_item = new MenuItem;

  menu_item->image = &image;
  menuItems.addItem (menu_item);

  if (dynamic && owObject) {
    int num_items = menuItems.getSize ();

    ((UIObject *) &image)->createObject (menuParent);
    Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					   MENU_RELEASE,
					   XV_KEY_DATA, UIC_OBJECT_KEY, this,
					   XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                num_items,
					   MENU_IMAGE,
					     image.getXViewObject (),
					   NULL);

    xv_set (owObject, MENU_APPEND_ITEM, xview_menu_item, NULL);
  }

  return menuItems.getSize ();
}

/*F+ int UIMenu::addItem (Image &image, UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Add an item to the menu.
*
*    PARAMETERS:
*
*    image   - The menu item image.
*    handler - The handler function for this menu item.
*
*    RETURN VALUES:
*
*    Returns the item number of the item added.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
int UIMenu::addItem (Image &image, UICHandlerFunction handler)
{
  MenuItem *menu_item = new MenuItem;

  menu_item->image = &image;
  menu_item->handler = handler;
  menuItems.addItem (menu_item);

  if (dynamic && owObject) {
    int num_items = menuItems.getSize ();

    ((UIObject *) &image)->createObject (menuParent);
    Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					   MENU_RELEASE,
					   XV_KEY_DATA, UIC_OBJECT_KEY, this,
					   XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                num_items,
					   MENU_IMAGE,
					     image.getXViewObject (),
					   NULL);

    setXViewMenuItemHandler (xview_menu_item);

    xv_set (owObject, MENU_APPEND_ITEM, xview_menu_item, NULL);
  }

  return menuItems.getSize ();
}

/*F+ int UIMenu::addItem (Image &image, UIMenu &menu)
*
*    PURPOSE:
*
*    Add an item to the menu.
*
*    PARAMETERS:
*
*    image - The menu item image.
*    menu  - The pullright menu for this menu item.
*
*    RETURN VALUES:
*
*    Returns the item number of the item added.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
int UIMenu::addItem (Image &image, UIMenu &menu)
{
  MenuItem *menu_item = new MenuItem;

  menu_item->image = &image;
  menu_item->menu = &menu;
  menuItems.addItem (menu_item);

  if (dynamic && owObject) {
    int num_items = menuItems.getSize ();

    menu.createObject (this);

    ((UIObject *) &image)->createObject (menuParent);
    Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					   MENU_RELEASE,
					   XV_KEY_DATA, UIC_OBJECT_KEY, this,
					   XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                num_items,
					   MENU_IMAGE,
					     image.getXViewObject (),
					   MENU_PULLRIGHT,
					     menu.getXViewObject (),
					   NULL);

    xv_set (owObject, MENU_APPEND_ITEM, xview_menu_item, NULL);
  }

  return menuItems.getSize ();
}

/*F+ void UIMenu::setItemColor (int item, Color color)
*
*    PURPOSE:
*
*    Set the color of a menu item.
*
*    PARAMETERS:
*
*    item  - The item number.
*    color - The color to set the item to.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemColor (int item, Color color)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    if (!colorMap) useDefaultColorMap ();

    int index;
    index = ((MenuItem *) menuItems [item-1])->colorIndex =
      colorMap->findColor (color);

    if (dynamic && owObject) {
      Menu_item xview_menu_item = (Menu_item)
				  xv_get (owObject,
					  MENU_NTH_ITEM,
					  (pinnable ? item+1 : item));
      initializeColorMap (menuParent, TRUE);
      xv_set (xview_menu_item, MENU_COLOR, index, NULL);
    }
  }
}

/*F+ void UIMenu::setItemColor (int item, char *color)
*
*    PURPOSE:
*
*    Set the color of a menu item.
*
*    PARAMETERS:
*
*    item  - The item number.
*    color - The name of the color to set the item to.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemColor (int item, char *color)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    if (!colorMap) useDefaultColorMap ();

    int index;
    index = ((MenuItem *) menuItems [item-1])->colorIndex =
      colorMap->findColor (color);

    if (dynamic && owObject) {
      Menu_item xview_menu_item = (Menu_item)
				  xv_get (owObject,
					  MENU_NTH_ITEM,
					  (pinnable ? item+1 : item));
      initializeColorMap (menuParent, TRUE);
      xv_set (xview_menu_item, MENU_COLOR, index, NULL);
    }
  }
}

/*F+ void UIMenu::setItemColor (char *label, Color color)
*
*    PURPOSE:
*
*    Set the color of the menu item that has the specified label.
*
*    PARAMETERS:
*
*    label - The label of the item to set the color for.
*    color - The color to set the item to.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemColor (char *label, Color color)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, label)) {
	if (!colorMap) useDefaultColorMap ();

	int index;
	index = ((MenuItem *) menuItems [i])->colorIndex =
	  colorMap->findColor (color);

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = (Menu_item)
				      xv_get (owObject,
					      MENU_NTH_ITEM,
					      (pinnable ? i + 2 : i + 1));
          initializeColorMap (menuParent, TRUE);
	  xv_set (xview_menu_item, MENU_COLOR, index, NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::setItemColor (char *label, char *color)
*
*    PURPOSE:
*
*    Set the color of the menu item that has the specified label.
*
*    PARAMETERS:
*
*    label - The label of the item to set the color for.
*    color - The name of the color to set the item to.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemColor (char *label, char *color)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, label)) {
	if (!colorMap) useDefaultColorMap ();

	int index;
	index = ((MenuItem *) menuItems [i])->colorIndex =
	  colorMap->findColor (color);

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = (Menu_item)
				      xv_get (owObject,
					      MENU_NTH_ITEM,
					      (pinnable ? i + 2 : i + 1));
          initializeColorMap (menuParent, TRUE);
	  xv_set (xview_menu_item, MENU_COLOR, index, NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::setItemColor (Image &image, Color color)
*
*    PURPOSE:
*
*    Set the color of the menu item that has the specified image.
*
*    PARAMETERS:
*
*    image - The image of the item to set the color for.
*    color - The color to set the item to.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemColor (Image &image, Color color)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &image) {
	if (!colorMap) useDefaultColorMap ();

	int index;
	index = ((MenuItem *) menuItems [i])->colorIndex =
	  colorMap->findColor (color);

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = (Menu_item)
				      xv_get (owObject,
					      MENU_NTH_ITEM,
					      (pinnable ? i + 2 : i + 1));
          initializeColorMap (menuParent, TRUE);
	  xv_set (xview_menu_item, MENU_COLOR, index, NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::setItemColor (Image &image, char *color)
*
*    PURPOSE:
*
*    Set the color of the menu item that has the specified image.
*
*    PARAMETERS:
*
*    image - The image of the item to set the color for.
*    color - The name of the color to set the item to.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemColor (Image &image, char *color)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &image) {
	if (!colorMap) useDefaultColorMap ();

	int index;
	index = ((MenuItem *) menuItems [i])->colorIndex =
	  colorMap->findColor (color);

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = (Menu_item)
				      xv_get (owObject,
					      MENU_NTH_ITEM,
					      (pinnable ? i + 2 : i + 1));
          initializeColorMap (menuParent, TRUE);
	  xv_set (xview_menu_item, MENU_COLOR, index, NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::setItemHandler (int item, UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Set the handler of a menu item.
*
*    PARAMETERS:
*
*    item    - The item number.
*    handler - The handler.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemHandler (int item, UICHandlerFunction handler)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    if (dynamic && owObject && !((MenuItem *) menuItems [item-1])->handler) {
      Menu_item xview_menu_item = (Menu_item) xv_get (owObject,
						      MENU_NTH_ITEM,
						      (pinnable ?
						       item+1 : item));

      setXViewMenuItemHandler (xview_menu_item);
    }

    ((MenuItem *) menuItems [item-1])->handler = handler;
  }
}

/*F+ void UIMenu::setItemHandler (char *label, UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Set the handler of the menu item that has the specified label.
*
*    PARAMETERS:
*
*    label   - The label of the item to set the handler for.
*    handler - The handler.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemHandler (char *label, UICHandlerFunction handler)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, label)) {
	if (dynamic &&
	    owObject &&
	    !((MenuItem *) menuItems [i-1])->handler) {
	  Menu_item xview_menu_item = (Menu_item) xv_get (owObject,
							  MENU_NTH_ITEM,
							  (pinnable ?
							   i + 2 : i + 1));


          setXViewMenuItemHandler (xview_menu_item);
	}

	((MenuItem *) menuItems [i])->handler = handler;
	return;
      }
  }
}

/*F+ void UIMenu::setItemHandler (Image &image, UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Set the handler of the menu item that has the specified image.
*
*    PARAMETERS:
*
*    image   - The image of the item to set the handler for.
*    handler - The handler.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemHandler (Image &image, UICHandlerFunction handler)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &image) {
	if (dynamic &&
	    owObject &&
	    !((MenuItem *) menuItems [i-1])->handler) {
	  Menu_item xview_menu_item = (Menu_item) xv_get (owObject,
							  MENU_NTH_ITEM,
							  (pinnable ?
							   i + 2 : i + 1));

          setXViewMenuItemHandler (xview_menu_item);
	}

	((MenuItem *) menuItems [i])->handler = handler;
	return;
      }
  }
}

/*F+ void UIMenu::setItemMenu (int item, UIMenu &menu)
*
*    PURPOSE:
*
*    Set the pullright menu of a menu item.
*
*    PARAMETERS:
*
*    item - The item number.
*    menu - The pullright menu.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemMenu (int item, UIMenu &menu)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    ((MenuItem *) menuItems [item-1])->menu = &menu;

    if (dynamic && owObject) {
      Menu_item xview_menu_item = (Menu_item) xv_get (owObject,
						      MENU_NTH_ITEM,
						      (pinnable ?
						       item+1 : item));

      
      menu.createObject (this);

      xv_set (xview_menu_item, MENU_PULLRIGHT, menu.getXViewObject (), NULL);
    }
  }
}

/*F+ void UIMenu::setItemMenu (char *label, UIMenu &menu)
*
*    PURPOSE:
*
*    Set the pullright menu of the menu item that has the specified label.
*
*    PARAMETERS:
*
*    label - The label of the item to set the menu for.
*    menu  - The pullright menu.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemMenu (char *label, UIMenu &menu)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, label)) {
	((MenuItem *) menuItems [i])->menu = &menu;

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = (Menu_item) xv_get (owObject,
							  MENU_NTH_ITEM,
							  (pinnable ?
							   i + 2 : i + 1));

      
	  menu.createObject (this);
	  
	  xv_set (xview_menu_item,
		  MENU_PULLRIGHT, menu.getXViewObject (),
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::setItemMenu (Image &image, UIMenu &menu)
*
*    PURPOSE:
*
*    Set the pullright menu of the menu item that has the specified image.
*
*    PARAMETERS:
*
*    image - The image of the item to set the menu for.
*    menu  - The pullright menu.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemMenu (Image &image, UIMenu &menu)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &image) {
	((MenuItem *) menuItems [i])->menu = &menu;

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = (Menu_item) xv_get (owObject,
							  MENU_NTH_ITEM,
							  (pinnable ?
							   i + 2 : i + 1));

      
	  menu.createObject (this);
	  
	  xv_set (xview_menu_item,
		  MENU_PULLRIGHT, menu.getXViewObject (),
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::setItemWindow (int item, UIWindow &window)
*
*    PURPOSE:
*
*    Specify a popup window for a menu item.
*
*    PARAMETERS:
*
*    item   - The item number.
*    window - The popup window.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemWindow (int item, UIWindow &window)
{
  if ((item > 0) && (item <= menuItems.getSize ()))
    ((MenuItem *) menuItems [item-1])->window = &window;
}

/*F+ void UIMenu::setItemWindow (char *label, UIWindow &window)
*
*    PURPOSE:
*
*    Specify a popup window for a menu item that has the specified label.
*
*    PARAMETERS:
*
*    label  - The label of the item to set the window for.
*    window - The popup window.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemWindow (char *label, UIWindow &window)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, label)) {
	((MenuItem *) menuItems [i])->window = &window;
	return;
      }
  }
}

/*F+ void UIMenu::setItemWindow (Image &image, UIWindow &window)
*
*    PURPOSE:
*
*    Specify a popup window for a menu item that has the specified image.
*
*    PARAMETERS:
*
*    image  - The image of the item to set the window for.
*    window - The popup window.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemWindow (Image &image, UIWindow &window)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &image) {
	((MenuItem *) menuItems [i])->window = &window;
	return;
      }
  }
}

/*F+ void UIMenu::setDefaultItem (int item)
*
*    PURPOSE:
*
*    Set the specified item as the default menu choice.
*
*    PARAMETERS:
*
*    item - The menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setDefaultItem (int item)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    defaultItem = item;

    if (dynamic && owObject)
      xv_set (owObject, MENU_DEFAULT_ITEM, (pinnable ? item+1 : item), NULL);
  }
}

/*F+ void UIMenu::setDefaultItem (char *label)
*
*    PURPOSE:
*
*    Set the specified item as the default menu choice.
*
*    PARAMETERS:
*
*    label - The label of the menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setDefaultItem (char *label)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, label)) {
	defaultItem = i + 1;

	if (dynamic && owObject)
	  xv_set (owObject, MENU_DEFAULT_ITEM, (pinnable ? i+2 : i+1), NULL);

	return;
      }
  }
}

/*F+ void UIMenu::setDefaultItem (Image &image)
*
*    PURPOSE:
*
*    Set the specified item as the default menu choice.
*
*    PARAMETERS:
*
*    image - The image of the menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setDefaultItem (Image &image)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &image) {
	defaultItem = i + 1;

	if (dynamic && owObject)
	  xv_set (owObject, MENU_DEFAULT_ITEM, (pinnable ? i+2 : i+1), NULL);

	return;
      }
  }
}

/*F+ void UIMenu::deleteItem (int item)
*
*    PURPOSE:
*
*    Delete the specified item of the menu.
*
*    PARAMETERS:
*
*    item - The menu item to delete.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::deleteItem (int item)
{
  MenuItem *menu_item;

  if ((item > 0) && (item <= menuItems.getSize ())) {
    menu_item = (MenuItem *) menuItems.removeItem (item-1);

    if (dynamic && owObject)
      xv_set (owObject, MENU_REMOVE, (pinnable ? item+1 : item), NULL);

    delete menu_item;
    updateSelected (item, -1);
  }
}

/*F+ void UIMenu::deleteItem (char *label)
*
*    PURPOSE:
*
*    Delete the item of the menu correspondoing to the specified label.
*
*    PARAMETERS:
*
*    label - The label of the menu item to delete.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::deleteItem (char *label)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, label)) {
	MenuItem *menu_item = (MenuItem *) menuItems.removeItem (i);

	if (dynamic && owObject)
	  xv_set (owObject, MENU_REMOVE, (pinnable ? i+2 : i+1), NULL);

	delete menu_item;
        updateSelected (i+1, -1);

	return;
      }
  }
}

/*F+ void UIMenu::deleteItem (Image &image)
*
*    PURPOSE:
*
*    Delete the item of the menu correspondoing to the specified image.
*
*    PARAMETERS:
*
*    image - The image of the menu item to delete.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::deleteItem (Image &image)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &image) {
	MenuItem *menu_item = (MenuItem *) menuItems.removeItem (i);

	if (dynamic && owObject)
	  xv_set (owObject, MENU_REMOVE, (pinnable ? i+2 : i+1), NULL);

	delete menu_item;
        updateSelected (i+1, -1);

	return;
      }
  }
}

/*F+ void UIMenu::setItemActive (int item, Boolean flag)
*
*    PURPOSE:
*
*    Set the selectability of the specified item of the menu.
*
*    PARAMETERS:
*
*    item - The menu item to set the selectability of.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemActive (int item, Boolean flag)
{
  MenuItem *menu_item;

  if ((item > 0) && (item <= menuItems.getSize ())) {
    menu_item = (MenuItem *) menuItems [item-1];
    menu_item->active = flag;

    if (dynamic && owObject)
      xv_set (xv_get (owObject,
		      MENU_NTH_ITEM, (pinnable ? item+1 : item),
		      NULL),
	      MENU_INACTIVE, (flag ? FALSE : TRUE),
	      NULL);
  }
}

/*F+ void UIMenu::setItemActive (char *label, Boolean flag)
*
*    PURPOSE:
*
*    Set the selectability of the item of the menu corresponding to the
*    specified label.
*
*    PARAMETERS:
*
*    label - The label of the menu item to set the selectability of.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemActive (char *label, Boolean flag)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, label)) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];
	menu_item->active = flag;

        if (dynamic && owObject)
          xv_set (xv_get (owObject,
		          MENU_NTH_ITEM, (pinnable ? i+2 : i+1),
		          NULL),
	          MENU_INACTIVE, (flag ? FALSE : TRUE),
	          NULL);

	return;
      }
  }
}

/*F+ void UIMenu::setItemActive (Image &image, Boolean flag)
*
*    PURPOSE:
*
*    Set the selectability of the item of the menu corresponding to the
*    specified image.
*
*    PARAMETERS:
*
*    image - The image of the menu item to set the selectability of.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::setItemActive (Image &image, Boolean flag)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &image) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];
	menu_item->active = flag;

        if (dynamic && owObject)
          xv_set (xv_get (owObject,
		          MENU_NTH_ITEM, (pinnable ? i+2 : i+1),
		          NULL),
	          MENU_INACTIVE, (flag ? FALSE : TRUE),
	          NULL);

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (int item, char *label)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item  - The menu item to replace.
*    label - The character string label of the menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (int item, char *label)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    MenuItem *menu_item = (MenuItem *) menuItems [item-1];

    menu_item->clear ();
    menu_item->label = strcpy (new char [strlen (label) + 1], label);

    if (dynamic && owObject) {
      Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					     MENU_RELEASE,
					     XV_KEY_DATA, UIC_OBJECT_KEY, this,
					     XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                  item,
					     MENU_STRING, menu_item->label,
					     NULL);

      xv_set (owObject,
	      MENU_REPLACE, (pinnable ? item+1 : item), xview_menu_item,
	      NULL);
    }
  }
}

/*F+ void UIMenu::replaceItem (int                 item,
*                              char               *label,
*                              UICHandlerFunction  handler)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item    - The menu item to replace.
*    label   - The character string label of the menu item.
*    handler - The handler function for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (int item, char *label, UICHandlerFunction handler)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    MenuItem *menu_item = (MenuItem *) menuItems [item-1];

    menu_item->clear ();
    menu_item->label = strcpy (new char [strlen (label) + 1], label);
    menu_item->handler = handler;

    if (dynamic && owObject) {
      Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					     MENU_RELEASE,
					     XV_KEY_DATA, UIC_OBJECT_KEY, this,
					     XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                  item,
					     MENU_STRING, menu_item->label,
					     NULL);

      setXViewMenuItemHandler (xview_menu_item);

      xv_set (owObject,
	      MENU_REPLACE, (pinnable ? item+1 : item), xview_menu_item,
	      NULL);
    }
  }
}

/*F+ void UIMenu::replaceItem (int item, char *label, UIMenu &menu)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item  - The menu item to replace.
*    label - The character string label of the menu item.
*    menu  - The pullright menu for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (int item, char *label, UIMenu &menu)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    MenuItem *menu_item = (MenuItem *) menuItems [item-1];

    menu_item->clear ();
    menu_item->label = strcpy (new char [strlen (label) + 1], label);
    menu_item->menu = &menu;

    if (dynamic && owObject) {
      menu.createObject (this);

      Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					     MENU_RELEASE,
					     XV_KEY_DATA, UIC_OBJECT_KEY, this,
					     XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                  item,
					     MENU_STRING, menu_item->label,
					     MENU_PULLRIGHT,
					       menu.getXViewObject (),
					     NULL);

      xv_set (owObject,
	      MENU_REPLACE, (pinnable ? item+1 : item), xview_menu_item,
	      NULL);
    }
  }
}

/*F+ void UIMenu::replaceItem (int item, Image &image)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item  - The menu item to replace.
*    image - The menu item image.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (int item, Image &image)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    MenuItem *menu_item = (MenuItem *) menuItems [item-1];

    menu_item->clear ();
    menu_item->image = &image;

    if (dynamic && owObject) {
      ((UIObject *) &image)->createObject (menuParent);
      Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					     MENU_RELEASE,
					     XV_KEY_DATA, UIC_OBJECT_KEY, this,
					     XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                  item,
					     MENU_IMAGE,
					       image.getXViewObject (),
					     NULL);

      xv_set (owObject,
	      MENU_REPLACE, (pinnable ? item+1 : item), xview_menu_item,
	      NULL);
    }
  }
}

/*F+ void UIMenu::replaceItem (int                 item,
*                              Image              &image,
*                              UICHandlerFunction  handler)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item    - The menu item to replace.
*    image   - The menu item image.
*    handler - The handler function for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (int item, Image &image, UICHandlerFunction handler)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    MenuItem *menu_item = (MenuItem *) menuItems [item-1];

    menu_item->clear ();
    menu_item->image = &image;
    menu_item->handler = handler;

    if (dynamic && owObject) {
      ((UIObject *) &image)->createObject (menuParent);
      Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					     MENU_RELEASE,
					     XV_KEY_DATA, UIC_OBJECT_KEY, this,
					     XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                  item,
					     MENU_IMAGE,
					       image.getXViewObject (),
					     NULL);

      setXViewMenuItemHandler (xview_menu_item);

      xv_set (owObject,
	      MENU_REPLACE, (pinnable ? item+1 : item), xview_menu_item,
	      NULL);
    }
  }
}

/*F+ void UIMenu::replaceItem (int item, Image &image, UIMenu &menu)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item  - The menu item to replace.
*    image - The menu item image.
*    menu  - The pullright menu for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (int item, Image &image, UIMenu &menu)
{
  if ((item > 0) && (item <= menuItems.getSize ())) {
    MenuItem *menu_item = (MenuItem *) menuItems [item-1];

    menu_item->clear ();
    menu_item->image = &image;
    menu_item->menu = &menu;

    if (dynamic && owObject) {
      menu.createObject (this);

      ((UIObject *) &image)->createObject (menuParent);
      Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
					     MENU_RELEASE,
					     XV_KEY_DATA, UIC_OBJECT_KEY, this,
					     XV_KEY_DATA, UIMENU_ITEM_NUM_KEY,
					                  item,
					     MENU_IMAGE,
					       image.getXViewObject (),
					     MENU_PULLRIGHT,
					       menu.getXViewObject (),
					     NULL);

      xv_set (owObject,
	      MENU_REPLACE, (pinnable ? item+1 : item), xview_menu_item,
	      NULL);
    }
  }
}

/*F+ int UIMenu::replaceItem (char *item_label, char *label)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_label - The label of the menu item to replace.
*    label      - The character string label of the menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (char *item_label, char *label)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, item_label)) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->label = strcpy (new char [strlen (label) + 1], label);

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_STRING, menu_item->label,
						 NULL);
	  
	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+1 : i), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (char               *item_label,
*                              char               *label,
*                              UICHandlerFunction  handler)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_label - The label of the menu item to replace.
*    label      - The character string label of the menu item.
*    handler    - The handler function for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (char               *item_label,
			  char               *label,
			  UICHandlerFunction  handler)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, item_label)) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->label = strcpy (new char [strlen (label) + 1], label);
	menu_item->handler = handler;

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_STRING, menu_item->label,
						 NULL);
	  
          setXViewMenuItemHandler (xview_menu_item);

	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+1 : i), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (char *item_label, char *label, UIMenu &menu)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_label - The label of the menu item to replace.
*    label      - The character string label of the menu item.
*    menu       - The pullright menu for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (char *item_label, char *label, UIMenu &menu)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, item_label)) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->label = strcpy (new char [strlen (label) + 1], label);
	menu_item->menu = &menu;

	if (dynamic && owObject) {
	  menu.createObject (this);
	  
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_STRING, menu_item->label,
						 MENU_PULLRIGHT,
						   menu.getXViewObject (),
						 NULL);
	  
	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+2 : i+1), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (char *item_label, Image &image)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_label - The label of the menu item to replace.
*    image      - The menu item image.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (char *item_label, Image &image)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, item_label)) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->image = &image;

	if (dynamic && owObject) {
	  ((UIObject *) &image)->createObject (menuParent);
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_IMAGE,
						   image.getXViewObject (),
						 NULL);

	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+2 : i+1), xview_menu_item,
		  NULL);
	}
	
	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (char               *item_label,
*                              Image              &image,
*                              UICHandlerFunction  handler)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_label - The label of the menu item to replace.
*    image      - The menu item image.
*    handler    - The handler function for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (char               *item_label,
			  Image              &image,
			  UICHandlerFunction  handler)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, item_label)) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->image = &image;
	menu_item->handler = handler;

	if (dynamic && owObject) {
	  ((UIObject *) &image)->createObject (menuParent);
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_IMAGE,
						   image.getXViewObject (),
						 NULL);

          setXViewMenuItemHandler (xview_menu_item);

	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+2 : i+1), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (char *item_label, Image &image, UIMenu &menu)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_label - The label of the menu item to replace.
*    image      - The menu item image.
*    menu       - The pullright menu for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (char *item_label, Image &image, UIMenu &menu)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->label)
      if (!strcmp (((MenuItem *) menuItems [i])->label, item_label)) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->image = &image;
	menu_item->menu = &menu;

	if (dynamic && owObject) {
	  menu.createObject (this);

	  ((UIObject *) &image)->createObject (menuParent);
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_IMAGE,
						   image.getXViewObject (),
						 MENU_PULLRIGHT,
						   menu.getXViewObject (),
						 NULL);

	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+2 : i+1), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (Image &item_image, char *label)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_image - The image of the menu item to replace.
*    label      - The character string label of the menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (Image &item_image, char *label)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &item_image) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->label = strcpy (new char [strlen (label) + 1], label);

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_STRING, menu_item->label,
						 NULL);
	  
	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+1 : i), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (Image              &item_image,
*                              char               *label,
*                              UICHandlerFunction  handler)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_image - The image of the menu item to replace.
*    label      - The character string label of the menu item.
*    handler    - The handler function for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (Image              &item_image,
			  char               *label,
			  UICHandlerFunction  handler)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &item_image) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->label = strcpy (new char [strlen (label) + 1], label);
	menu_item->handler = handler;

	if (dynamic && owObject) {
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_STRING, menu_item->label,
						 NULL);

          setXViewMenuItemHandler (xview_menu_item);
	  
	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+1 : i), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (Image &item_image, char *label, UIMenu &menu)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_image - The image of the menu item to replace.
*    label      - The character string label of the menu item.
*    menu       - The pullright menu for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (Image &item_image, char *label, UIMenu &menu)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &item_image) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->label = strcpy (new char [strlen (label) + 1], label);
	menu_item->menu = &menu;

	if (dynamic && owObject) {
	  menu.createObject (this);
	  
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_STRING, menu_item->label,
						 MENU_PULLRIGHT,
						   menu.getXViewObject (),
						 NULL);
	  
	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+2 : i+1), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (Image &item_image, Image &image)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_image - The image of the menu item to replace.
*    image      - The menu item image.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (Image &item_image, Image &image)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &item_image) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->image = &image;

	if (dynamic && owObject) {
	  ((UIObject *) &image)->createObject (menuParent);
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_IMAGE,
						   image.getXViewObject (),
						 NULL);

	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+2 : i+1), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (Image              &item_image,
*                              Image              &image,
*                              UICHandlerFunction  handler)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_image - The image of the menu item to replace.
*    image      - The menu item image.
*    handler    - The handler function for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (Image              &item_image,
			  Image              &image,
			  UICHandlerFunction  handler)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &item_image) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->image = &image;
	menu_item->handler = handler;

	if (dynamic && owObject) {
	  ((UIObject *) &image)->createObject (menuParent);
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_IMAGE,
						   image.getXViewObject (),
						 NULL);

          setXViewMenuItemHandler (xview_menu_item);

	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+2 : i+1), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::replaceItem (Image &item_image, Image &image, UIMenu &menu)
*
*    PURPOSE:
*
*    Replace the specified item of the menu.
*
*    PARAMETERS:
*
*    item_image - The image of the menu item to replace.
*    image      - The menu item image.
*    menu       - The pullright menu for this menu item.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::replaceItem (Image &item_image, Image &image, UIMenu &menu)
{
  int num_items = menuItems.getSize ();

  for (int i=0; i < num_items; i++) {
    if (((MenuItem *) menuItems [i])->image)
      if (((MenuItem *) menuItems [i])->image == &item_image) {
	MenuItem *menu_item = (MenuItem *) menuItems [i];

	menu_item->clear ();
	menu_item->image = &image;
	menu_item->menu = &menu;

	if (dynamic && owObject) {
	  menu.createObject (this);

	  ((UIObject *) &image)->createObject (menuParent);
	  Menu_item xview_menu_item = xv_create (NULL, MENUITEM,
						 MENU_RELEASE,
						 XV_KEY_DATA, UIC_OBJECT_KEY,
						              this,
						 XV_KEY_DATA,
						   UIMENU_ITEM_NUM_KEY, i+1,
						 MENU_IMAGE,
						   image.getXViewObject (),
						 MENU_PULLRIGHT,
						   menu.getXViewObject (),
						 NULL);

	  xv_set (owObject,
		  MENU_REPLACE, (pinnable ? i+2 : i+1), xview_menu_item,
		  NULL);
	}

	return;
      }
  }
}

/*F+ void UIMenu::show (UIObject *event_object)
*
*    PURPOSE:
*
*    Shows a menu from an event handler.  Until "refreshShowMenu" is called,
*    the menu showed will be the same XView object that was created
*    the first time "show" was called.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::show (UIObject *event_object)
{
  if (!showMenu) {
    createObject (event_object);
    showMenu = owObject;
  }

  xv_set (showMenu, MENU_DONE_PROC, &UIMenu::showMenuDoneHandler, NULL);

  menu_show (showMenu,
	     event_object->getXViewObject (),
	     event_object->getEvent ()->getXViewEvent (),
	     NULL);

  showMenuDisplayedCount++;
}

/*F+ void UIMenu::refreshShowMenu ()
*
*    PURPOSE:
*
*    Destroys the XView menu object associated with the last time the
*    menu was shown.  All of the sub-menu XView object are also
*    destroyed.  Only use this if the menu being destroyed is not currently
*    being displayed.
*
*    NOTES:
*
*    This member function is only needed to deal with the showing of
*    dynamically changing menus via event handlers.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/06/90   Original creation.
*
*F-*/
void UIMenu::refreshShowMenu ()
{
  if ((showMenuDisplayedCount == 0) && showMenu) {
    xv_destroy_safe (showMenu);
    showMenu = (Menu) NULL;
  }
}

/*F+ void UIMenu::showMenuDoneHandler (Menu menu)
*
*    PURPOSE:
*
*    This private member function is called by XView when a menu
*    that was shown via the "show" member function is exited.  This
*    member function sets the menu displayed flag and XV_SHOW to FALSE
*    and decrements a counter that keeps track of the number of times
*    the "show" member function has been used on this menu.
*
*    PARAMETER:
*
*    menu - The menu that was exited.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/10/90   Original creation.
*
*F-*/
void UIMenu::showMenuDoneHandler (Menu menu)
{
  UIMenu *object = (UIMenu *) xv_get (menu, XV_KEY_DATA, UIC_OBJECT_KEY);

  xv_set (menu, XV_SHOW, FALSE, NULL);
  object->showMenuDisplayedCount--;
}

/*F+ int UIMenu::addNotifyTarget (UIObject          &object,
*                                 UICTargetFunction  func,
*				  int		     xview_event_action,
*				  int		     xview_event_id)
*
*    PURPOSE:
*
*    Adds a target to the list of notify targets for this object.
*
*    PARAMETERS:
*
*    object             - The target object.
*    func               - The target function.
*    xview_event_action - If specified, this is the XView event action that
*                         must occur for the target function to be called.
*    xview_event_id     - If specified, this is the XView event ID that
*                         must occur for the target function to be called. 
*
*    RETURN VALUES:
*
*    Returns the position in the target list for this object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIMenu::addNotifyTarget (UIObject          &object,
			     UICTargetFunction  func,
			     int		xview_event_action,
			     int		xview_event_id)
{
  UITarget *target = new UITarget;

  target->targetObject = &object;
  target->targetFunction = func;
  target->eventAction = xview_event_action;
  target->eventID = xview_event_id;

  notifyTargets.addItem (target);
  return notifyTargets.getSize ();
}

/*F+ void UIMenu::notifyNotifyTargets ()
*
*    PURPOSE:
*
*    Internal function that loops through all of the notify targets
*    associated with this menu and calls the corresponding target
*    functions.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIMenu::notifyNotifyTargets ()
{
  InputEvent *event = getEvent ();

  int xview_event_action = event->getAction ();
  int xview_event_id = event->getID ();

  for (int i = notifyTargets.getSize () - 1; i > -1; i--) {
    UITarget *target = (UITarget *) notifyTargets [i];

    /* Call the user specified target function if the user specified
       target event occurred */
    if (target->targetFunction)
      if ((((target->eventAction == UIC_NOT_SET) ||
            (target->eventAction == ACTION_NULL_EVENT)) &&
           ((target->eventID == UIC_NOT_SET) ||
            (target->eventID == xview_event_id))) ||
          (target->eventAction == xview_event_action)) 
        (*target->targetFunction) (this, target->targetObject);
  }
}

/*F+ void UIMenu::updateSelected (int item, int delta)
*
*    PURPOSE:
*
*    Internal function for updating choice selection indexes in the selections
*    list due to the addition or deletion of an item from the object.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*    MGS       08/26/91   Original creation.
*
*F-*/
void UIMenu::updateSelected (int item, int delta)
{
  if ((lastSelected == item) && (delta == -1))
    lastSelected = UIC_NOT_SET;
  else if (lastSelected >= item)
    lastSelected += delta;
}
 
/*F+ void UIMenu::setXViewMenuHandler ()
*
*    PURPOSE:
*
*    A protected virtual member function that sets the XView menu
*    handler on the menu.  The derived class version of this function
*    is always used.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/10/90   Original creation.
*
*F-*/
void UIMenu::setXViewMenuHandler ()
{
}

/*F+ void UIMenu::setXViewMenuItemHandler (Menu_item xview_menu_item)
*
*    PURPOSE:
*
*    A protected virtual member function that sets the XView menu
*    handler on a menu item.  The derived class version of this function
*    is always used.
*
*    PARAMETERS:
*
*    xview_menu_item - The menu item to set the handler on.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/10/90   Original creation.
*
*F-*/
void UIMenu::setXViewMenuItemHandler (Menu_item xview_menu_item)
{
}

#ifdef TEST
#endif /* TEST */
