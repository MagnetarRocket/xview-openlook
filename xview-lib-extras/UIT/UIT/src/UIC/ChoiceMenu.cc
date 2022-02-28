/*H+ ChoiceMenu.cc
*
*    SCCS ID: @(#)ChoiceMenu.cc 1.2 91/10/25 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the ChoiceMenu class of the User Interface
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
static char sccsid [] = "@(#)ChoiceMenu.cc 1.2 91/10/25 SMI"; 
#endif

// Includes
#include "BaseWindow.h"
#include "CharacterFont.h"
#include "ChoiceMenu.h"
#include "ColorMap.h"

/*F+ int ChoiceMenu::getChoice (int selection, int &item);
*
*    PURPOSE:
*
*    Get the item number corresponding to the specified selection number.
*
*    INPUT PARAMETERS:
*
*    selection - The number of the selection to get the item of.
*    item      - The item number of the specified selection.
*
*    RETURN VALUES:
*
*    Returns the item number of the specified selection through the return
*    value and the "item" parameter.  If the selection number is illegal,
*    then 0 is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/07/90   Original creation.
*
*F-*/
int ChoiceMenu::getChoice (int selection, int &item)
{
  if (multipleChoice)
    if ((selection > 0) && (selection <= selected.getSize ())) {
      return item = (int) selected [selection-1];
    }

  return item = 0;
}

/*F+ char *ChoiceMenu::getChoice (int selection, char* &label);
*
*    PURPOSE:
*
*    Get the item label corresponding to the specified selection number.
*
*    INPUT PARAMETERS:
*
*    selection - The number of the selection to get the item of.
*    label     - The label of the item of the specified selection.
*
*    RETURN VALUES:
*
*    Returns a new memory buffer containing the item label of the specified
*    selection through the return value and the "label" parameter.  If the
*    selection number is illegal, then NULL is returned.
*
*    SIDE EFFECTS:
*
*    Returns an allocated memoery buffer.  The caller must free up the memory.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/07/90   Original creation.
*
*F-*/
char *ChoiceMenu::getChoice (int selection, char* &label)
{
  if (multipleChoice)
    if ((selection > 0) && (selection <= selected.getSize ())) {
      int item = (int) selected [selection-1];

      char *tmp = ((MenuItem *) menuItems [item-1])->label;
      return label = strcpy (new char [strlen (tmp) + 1], tmp);
    }

  return label = (char *) NULL;
}

/*F+ Image *ChoiceMenu::getChoice (int selection, Image* &image);
*
*    PURPOSE:
*
*    Get the item image corresponding to the specified selection number.
*
*    INPUT PARAMETERS:
*
*    selection - The number of the selection to get the item of.
*    image     - The image of the item of the specified selection.
*
*    RETURN VALUES:
*
*    Returns the item image of the specified selection through the return
*    value and the "image" parameter.  If the selection number is illegal,
*    then NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/07/90   Original creation.
*
*F-*/
Image *ChoiceMenu::getChoice (int selection, Image* &image)
{
  if (multipleChoice)
    if ((selection > 0) && (selection <= selected.getSize ())) {
      int item = (int) selected [selection-1];
      return image = ((MenuItem *) menuItems [item-1])->image;
    }

  return image = (Image *) NULL;
}

/*F+ void ChoiceMenu::setSelectedItem (int item, Boolean flag);
*
*    PURPOSE:
*
*    Specifies the specified item as selected or not selected.
*
*    INPUT PARAMETERS:
*
*    item - The item to set the selection status on.
*    flag - The selection status.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*    MGS       08/22/91   Original creation.
*
*F-*/
void ChoiceMenu::setSelectedItem (int item, Boolean flag)
{
  int num_choices = selected.getSize ();
      
  // If the item is to be set on...
  if (flag) {
    // If not multiple choice...
    if (!multipleChoice && selected.getSize ())
      // Get rid of the old selection
      (void) selected.removeItem (0);
    // Else, multiple choice...
    else {
      /* If the selected item is already in the list, remove it
         and place it at the end of the list */
 
      // Loop through the selected list...
      for (int i=0; i < num_choices; i ++) {
        // If this is the specified item...
        if (((int) selected [i]) == item) {
          // Remove the item from the list
          (void) selected.removeItem (i);
          break;
        }
      }
    }   
        
    // Add the new selection to the selected list
    selected.addItem ((Generic *) item);
    lastSelected = item;
  }
  // Else turn the selection off...
  else {
    // Loop through the selected list...
    for (int i=0; i < num_choices; i ++) {
      // If this is the specified item...
      if (((int) selected [i]) == item) {
        (void) selected.removeItem (i);
        break;
      }
    }
  }   

  // If the XView object exists...
  if (owObject) {
    Menu_item xview_menu_item = xv_get (owObject,
					MENU_NTH_ITEM, 
					pinnable ? item+1 : item);
    xv_set (xview_menu_item, MENU_SELECTED, flag, NULL);
  }
}
 
/*F+ void ChoiceMenu::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the the menu.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       10/07/90   Original creation.
*
*F-*/
void ChoiceMenu::createObject (UIObject *parent)
{
  menuParent = parent;

  int num_items = menuItems.getSize ();

  if (font) {
    ((UIObject *) font)->createObject (parent);
    owObject = xv_create (NULL, (multipleChoice ? MENU_TOGGLE_MENU :
			                          MENU_CHOICE_MENU),
			  MENU_NOTIFY_PROC, &ChoiceMenu::menuNotifyProc,
                          XV_FONT,          font->getXViewObject (),
			  NULL);
  }
  else
    owObject = xv_create (NULL, (multipleChoice ? MENU_TOGGLE_MENU :
			                          MENU_CHOICE_MENU),
			  MENU_NOTIFY_PROC, &ChoiceMenu::menuNotifyProc,
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
	      MENU_NOTIFY_PROC, &ChoiceMenu::menuItemProc,
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

  int num_selected = selected.getSize ();
  for (i=0; i < num_selected; i++) {
    xview_menu_item = xv_get (owObject,
			      MENU_NTH_ITEM, (int) selected [i] +
					     (pinnable ? 1 : 0));
    xv_set (xview_menu_item, MENU_SELECTED, TRUE, NULL);
  }

  UIObject::setUIObjectAttributes (parent);
}

/*F+ void ChoiceMenu::updateSelected (int item, int delta)
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
void ChoiceMenu::updateSelected (int item, int delta)
{
  if ((lastSelected == item) && (delta == -1))
    lastSelected = UIC_NOT_SET;
  else if (lastSelected >= item)
    lastSelected += delta;

  if (delta == -1) {
    int remove_index = selected.findItem ((Generic *) item);
    if (remove_index > -1) (void) selected.removeItem (remove_index);
  }

  int num_selected = selected.getSize ();
  int value;
 
  for (int i=0; i < num_selected; i++)
    if ((value = (int) selected [i]) > item) 
      selected [i] = (Generic *) (value + delta);
}

/*F+ void ChoiceMenu::menuNotifyProc (Menu menu, Menu_item menu_item)
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
void ChoiceMenu::menuNotifyProc (Menu menu, Menu_item menu_item)
{
  int item;
  int selection;

  ChoiceMenu *object = (ChoiceMenu *) xv_get (menu,
					      XV_KEY_DATA,
					      UIC_OBJECT_KEY);
  item = (int) xv_get (menu_item, XV_KEY_DATA, UIMENU_ITEM_NUM_KEY);

  if (object->multipleChoice) {
    if ((selection = object->selected.findItem ((Generic *) item)) == -1)
      object->selected.addItem ((Generic *) item);
    else
      (void) object->selected.removeItem ((Generic *) item);
  }

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

/*F+ void ChoiceMenu::menuItemProc (Menu menu, Menu_item menu_item)
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
void ChoiceMenu::menuItemProc (Menu menu, Menu_item menu_item)
{
  int item;
  int selection;

  ChoiceMenu *object = (ChoiceMenu *) xv_get (menu,
					      XV_KEY_DATA,
					      UIC_OBJECT_KEY);
  item = (int) xv_get (menu_item, XV_KEY_DATA, UIMENU_ITEM_NUM_KEY);

  if (object->multipleChoice) {
    if ((selection = object->selected.findItem ((Generic *) item)) == -1)
      object->selected.addItem ((Generic *) item);
    else
      (void) object->selected.removeItem ((Generic *) item);
  }

  object->lastSelected = item;

  UIWindow *window = ((MenuItem *) object->menuItems [item-1])->window;
  Boolean   show_window = ((MenuItem *) object->menuItems [item-1])->showWindow;
  if (window && show_window) window->show (TRUE);

  object->notifyNotifyTargets ();

  if (((MenuItem *) object->menuItems [item-1])->handler)
    (*((MenuItem *) object->menuItems [item-1])->handler) (object);
}

/*F+ void ChoiceMenu::setXViewMenuHandler ()
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
void ChoiceMenu::setXViewMenuHandler ()
{
  xv_set (owObject, MENU_NOTIFY_PROC, &ChoiceMenu::menuNotifyProc, NULL);
  notifyHandlerSet = TRUE;
}
 
/*F+ void ChoiceMenu::setXViewMenuItemHandler (Menu_item xview_menu_item)
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
void ChoiceMenu::setXViewMenuItemHandler (Menu_item xview_menu_item)
{
  xv_set (xview_menu_item, MENU_NOTIFY_PROC, &ChoiceMenu::menuItemProc, NULL);
}
  
#ifdef TEST
#endif /* TEST */
