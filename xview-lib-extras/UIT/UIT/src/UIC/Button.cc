/*H+ Button.cc
*
*    SCCS ID: @(#)Button.cc 1.3 92/05/23 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the Button class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       05/10/91   Preparation for code review. 
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)Button.cc 1.3 92/05/23 SMI"; 
#endif

// Includes
#include "Button.h"

/*F+ Button::Button ()
*
*    PURPOSE:
*
*    Button class constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Button::Button ()
{
  setButtonDefaults ();
}

/*F+ Button::Button (char *string_label)
*
*    PURPOSE:
*
*    Button class constructor.
*
*    PARAMETERS:
*
*    string_label - Specifies a string to be used as a label
*                   on the button.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Button::Button (char *string_label) : UIComponent (string_label)
{
  setButtonDefaults ();
}

/*F+ Button::Button (Image &image_label)
*
*    PURPOSE:
*
*    Button class constructor.
*
*    PARAMETERS:
*
*    image_label - Specifies an image to be used as a label
*                  on the button.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Button::Button (Image &image_label) : UIComponent (image_label)
{
  setButtonDefaults ();
}

/*F+ Button::~Button ()
*
*    PURPOSE:
*
*    Button class destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Button::~Button ()
{
}

/*F+ void Button::setButtonDefaults ()
*
*    PURPOSE:
*
*    Sets the default values of the Button data members.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Button::setButtonDefaults ()
{
  abbreviated = FALSE;
  labelWidth = UIC_NOT_SET;
  window = (UIWindow *) NULL;
  showWindow = TRUE;
  buttonMenu = (UIMenu *) NULL;
  notifyHandler = (UICHandlerFunction) NULL;
}

/*F+ void Button::setMenu (UIMenu &menu)
*
*    PURPOSE:
*
*    Specifies the pull-down menu for a button.
*
*    PARAMETERS:
*
*    menu - The pull-down menu.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Button::setMenu (UIMenu &menu)
{
  /* Save the button menu pointer */
  buttonMenu = &menu;

  /* If the button object has already been created... */
  if (owObject) {
    /* Get the current XView button menu and delete it if it exists */
    Menu old_menu = (Menu) xv_get (owObject, PANEL_ITEM_MENU);
    if (old_menu) xv_destroy_safe (old_menu);
    
    /* Create a new XView menu and set the XView button menu */
    ((UIObject *) buttonMenu)->createObject (parentObject);
    xv_set (owObject, PANEL_ITEM_MENU, buttonMenu->getXViewObject (), NULL);
  }
}

/*F+ void Button::notifyProc (Panel_item item, Event *event)
*
*    PURPOSE:
*
*    XView button notify proc.  This member function is protected.
*
*    PARAMETERS:
*
*    item  - The XView panel item that the notify occurred on.
*    event - The XView event info structure.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Button::notifyProc (Panel_item item, Event *event)
{
  Button *object = (Button *) xv_get (item, XV_KEY_DATA, UIC_OBJECT_KEY);
  
  /* Set the internal UIT event information */
  object->setEventInformation (event);

  /* Notify any notify targets */
  object->notifyNotifyTargets ();
  
  /* If a window is attached to the button (via setWindow) and
     the window is supposed to be shown (via setShowWindow)... */
  if (object->window && object->showWindow) object->window->show (TRUE);
  
  /* Call the user specified notify handler */
  if (object->useDefaultNotifyHandlerFlag)
    object->defaultNotifyHandler ();
  else if (object->notifyHandler)
    (*object->notifyHandler) (object);
  
  /* Erase internal UIT event information */
  object->resetEventInformation ();

  // If the user specified a notify status
  if (object->notifyStatus != UIC_NOT_SET) {
    // Set the notify status
    xv_set (item, PANEL_NOTIFY_STATUS, object->notifyStatus, NULL);

    // Reset the internal notify status
    object->notifyStatus = UIC_NOT_SET;
  }
}

/*F+ void Button::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView button object.
*
*    PARAMETERS:
*
*    parent - The parent of the button (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Button::createObject (UIObject *parent)
{
  /* Use the UIComponent::createWithFonts to create a button
     with the ability to set label fonts */
  if (abbreviated)
    createWithFonts (parent, PANEL_ABBREV_MENU_BUTTON, NULL);
  else
    createWithFonts (parent, PANEL_BUTTON, NULL);

  /* Set common XView attributes */
  setUIComponentAttributes (parent);
  
  /* Specify the internal button notify proc */
  xv_set (owObject, PANEL_NOTIFY_PROC, &Button::notifyProc, NULL);
  
  /* If a menu was specified... */
  if (buttonMenu) {
    /* Create the XView menu object and set the XView button menu */
    ((UIObject *) buttonMenu)->createObject (parent);
    xv_set (owObject, PANEL_ITEM_MENU, buttonMenu->getXViewObject (), NULL);
  }
  
  /* Turn the button off, if desired... */
  if (active == FALSE)
    xv_set (owObject, PANEL_INACTIVE, TRUE, NULL);       
  
  /* Set the button label width */
  if (labelWidth != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_LABEL_WIDTH, labelWidth, NULL, NULL);
    else
      xv_set (owObject, PANEL_LABEL_WIDTH, labelWidth, NULL);
  }
}

#ifdef TEST
#endif /* TEST */
