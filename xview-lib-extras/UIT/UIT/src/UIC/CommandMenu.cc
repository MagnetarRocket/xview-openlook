/*H+ CommandMenu.cc
*
*    SCCS ID: @(#)CommandMenu.cc 1.2 91/10/25 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CommandMenu class of the User
*    Interface Components.
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
static char sccsid [] = "@(#)CommandMenu.cc 1.2 91/10/25 SMI"; 
#endif

// Includes
#include "BaseWindow.h"
#include "CharacterFont.h"
#include "ColorMap.h"
#include "CommandMenu.h"

/*F+ void CommandMenu::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the menu.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/07/90   Original creation.
*
*F-*/
void CommandMenu::createObject (UIObject *parent)
{
  menuParent = parent;

  int num_items = menuItems.getSize ();

  if (font) {
    ((UIObject *) font)->createObject (parent);
    owObject = xv_create (NULL, MENU_COMMAND_MENU,
			  MENU_NOTIFY_PROC, &CommandMenu::menuNotifyProc,
  			  XV_FONT,          font->getXViewObject (),
			  NULL);
  }
  else
    owObject = xv_create (NULL, MENU_COMMAND_MENU,
			  MENU_NOTIFY_PROC, &CommandMenu::menuNotifyProc,
			  NULL);

  UICGeneric parent_frame = xv_get (parent->getXViewObject (),
                                    XV_KEY_DATA,
                                    UIC_FRAME_KEY);

  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  xv_set (owObject, XV_KEY_DATA, UIC_FRAME_KEY, parent_frame, NULL);
			   
  setHelpData ();

  if (pinnable) {
    if (label)
      xv_set (owObject, MENU_GEN_PIN_WINDOW, parent_frame, label, NULL);
    else
      xv_set (owObject, MENU_GEN_PIN_WINDOW, parent_frame, "", NULL);
  }

  if (label)
    xv_set (owObject, MENU_TITLE_ITEM, label, NULL);

  if (xLoc != UIC_NOT_SET)
    xv_set (owObject, XV_X, xLoc, NULL);

  if (yLoc != UIC_NOT_SET)
    xv_set (owObject, XV_Y, yLoc, NULL);

  if (numColumns != UIC_NOT_SET)
    xv_set (owObject, MENU_NCOLS, numColumns, NULL);

  if (numRows != UIC_NOT_SET)
    xv_set (owObject, MENU_NROWS, numRows, NULL);

  MenuItem  *menu_item;
  Menu_item  xview_menu_item;
  for (int i=0; i < num_items; i++) {
    menu_item = (MenuItem *) menuItems [i];

    xview_menu_item = xv_create (NULL, MENUITEM,
				 MENU_RELEASE,
				 XV_KEY_DATA, UIC_OBJECT_KEY, this,
				 XV_KEY_DATA, UIMENU_ITEM_NUM_KEY, i+1,
				 NULL);

    if (menu_item->label) {
      xv_set (xview_menu_item, MENU_STRING, menu_item->label, NULL);
    }
    else if (menu_item->image) {
      ((UIObject *) menu_item->image)->createObject (parent);
      xv_set (xview_menu_item,
	      MENU_IMAGE, menu_item->image->getXViewObject (),
	      NULL);
    }
    else {
      xv_set (xview_menu_item, MENU_STRING, "", NULL);
    }

    if (menu_item->handler)
      xv_set (xview_menu_item,
	      MENU_NOTIFY_PROC, &CommandMenu::menuItemProc,
	      NULL);

    if (menu_item->menu) {
      ((UIObject *) (menu_item->menu))->createObject (parent);

      xv_set (xview_menu_item,
	      MENU_PULLRIGHT, menu_item->menu->getXViewObject (),
	      NULL);
    }

    if (menu_item->colorIndex != UIC_NOT_SET) {
      initializeColorMap (parent, TRUE);
      xv_set (xview_menu_item, MENU_COLOR, menu_item->colorIndex, NULL);
    }

    if (!menu_item->active)
      xv_set (xview_menu_item, MENU_INACTIVE, TRUE);

    xv_set (owObject, MENU_APPEND_ITEM, xview_menu_item, NULL);
  }

  if ((defaultItem > 0) && (defaultItem <= menuItems.getSize ()))
    xv_set (owObject, MENU_DEFAULT, defaultItem + (pinnable ? 1 : 0), NULL);

  UIObject::setUIObjectAttributes (parent);
}

/*F+ void CommandMenu::menuNotifyProc (Menu menu, Menu_item menu_item)
*
*    PURPOSE:
*
*    XView menu notify handler.  This member function is protected.
*
*    PARAMETERS:
*
*    menu      - The the notify event occurred on.
*    menu_item - The menu item that was selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/07/90   Original creation.
*
*F-*/
void CommandMenu::menuNotifyProc (Menu menu, Menu_item menu_item)
{
  int item;

  CommandMenu *object = (CommandMenu *) xv_get (menu,
						XV_KEY_DATA,
						UIC_OBJECT_KEY);
  item = (int) xv_get (menu_item, XV_KEY_DATA, UIMENU_ITEM_NUM_KEY);

  object->lastSelected = item;

  UIWindow *window = ((MenuItem *) object->menuItems [item-1])->window;
  Boolean   show_window = ((MenuItem *) object->menuItems [item-1])->showWindow;
  if (window && show_window) window->show (TRUE);

  object->notifyNotifyTargets ();

  if (object->useDefaultNotifyHandlerFlag)   
    object->defaultNotifyHandler ();    
  else if (object->notifyHandler)
    (*object->notifyHandler) (object);
}

/*F+ void CommandMenu::menuItemProc (Menu menu, Menu_item menu_item)
*
*    PURPOSE:
*
*    XView menu item notify handler.  This member function is protected.
*
*    PARAMETERS:
*
*    menu      - The the notify event occurred on.
*    menu_item - The menu item that was selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/07/90   Original creation.
*
*F-*/
void CommandMenu::menuItemProc (Menu menu, Menu_item menu_item)
{
  int item;

  CommandMenu *object = (CommandMenu *) xv_get (menu,
					        XV_KEY_DATA,
					        UIC_OBJECT_KEY);
  item = (int) xv_get (menu_item, XV_KEY_DATA, UIMENU_ITEM_NUM_KEY);

  object->lastSelected = item;

  UIWindow *window = ((MenuItem *) object->menuItems [item-1])->window;
  Boolean   show_window = ((MenuItem *) object->menuItems [item-1])->showWindow;
  if (window && show_window) window->show (TRUE);

  object->notifyNotifyTargets ();

  if (((MenuItem *) object->menuItems [item-1])->handler)
    (*((MenuItem *) object->menuItems [item-1])->handler) (object);
}

/*F+ void CommandMenu::setXViewMenuHandler () 
* 
*    PURPOSE: 
* 
*    Sets the XView menu handler on the menu.  This member function is
*    protected.
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
*    MGS       10/10/90   Original creation. 
* 
*F-*/ 
void CommandMenu::setXViewMenuHandler () 
{ 
  xv_set (owObject, MENU_NOTIFY_PROC, &CommandMenu::menuNotifyProc, NULL); 
  notifyHandlerSet = TRUE;
} 
    
/*F+ void CommandMenu::setXViewMenuItemHandler (Menu_item xview_menu_item) 
* 
*    PURPOSE: 
* 
*    Sets the XView menu handler on a menu item.  This member function
*    is protected.
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
void CommandMenu::setXViewMenuItemHandler (Menu_item xview_menu_item)
{
  xv_set (xview_menu_item, MENU_NOTIFY_PROC, &CommandMenu::menuItemProc, NULL);
}
       
#ifdef TEST
#endif /* TEST */
