/*H+ UIInputTextItem.cc
*
*    SCCS ID: @(#)UIInputTextItem.cc 1.5 92/06/04 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the UIInputTextItem class.
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
static char sccsid [] = "@(#)UIInputTextItem.cc 1.5 92/06/04 SMI"; 
#endif

// Includes
#include "UIInputTextItem.h"

/*F+ UIInputTextItem::UIInputTextItem ()
*
*    PURPOSE:
*
*    UIInputTextItem constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIInputTextItem::UIInputTextItem ()
{
  setUIInputTextItemDefaults ();
}

/*F+ UIInputTextItem::UIInputTextItem (char *string_label)
*
*    PURPOSE:
*
*    UIInputTextItem constructor.
*
*    PARAMETERS:
*
*    string_label - Specifies a string for the label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIInputTextItem::UIInputTextItem (char *string_label)
		: UIComponent (string_label)
{
  setUIInputTextItemDefaults ();
}

/*F+ UIInputTextItem::UIInputTextItem (Image &image_label)
*
*    PURPOSE:
*
*    UIInputTextItem constructor.
*
*    PARAMETERS:
*
*    image_label - Specifies an image for the label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIInputTextItem::UIInputTextItem (Image &image_label)
		: UIComponent (image_label)
{
  setUIInputTextItemDefaults ();
}

/*F+ void UIInputTextItem::setUIInputTextItemDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the UIInputTextItem class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIInputTextItem::setUIInputTextItemDefaults ()
{
  inputLength = UIC_NOT_SET;
  inputDisplayLength = UIC_NOT_SET;
  underline = TRUE;
  notifyHandler = (UICHandlerFunction) NULL;
  readOnly = FALSE;
  echo = TRUE;
  mask = '\0';
}

/*F+ UIInputTextItem::~UIInputTextItem ()
*
*    PURPOSE:
*
*    UIInputTextItem destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIInputTextItem::~UIInputTextItem ()
{
}

/*F+ Panel_setting UIInputTextItem::notifyProc (Panel_item item, Event *event)
*
*    PURPOSE:
*
*    Internal wrapper for the XView UIInputTextItem notify proc.  When an input
*    item value changes, XView calls this function.  This function calls
*    the user specified handler.
*
*    PARAMETERS:
*
*    item  - The XView object the notify occurred on.
*    event - The XView event information structure.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Panel_setting UIInputTextItem::notifyProc (Panel_item item, Event *event)
{
  UIInputTextItem *object = (UIInputTextItem *) xv_get (item,
							XV_KEY_DATA,
							UIC_OBJECT_KEY);

  /* In XView V3, some of the components are compund items and the event
     may occur on the child item instead of the parent item.  If this
     happens, the parent (XV_OWNER) must be obtained in order to
     obtain a "this" pointer */
  if (!object) {
    Panel_item compound_item = xv_get (item, PANEL_ITEM_OWNER);
    object = (UIInputTextItem *)
             xv_get (compound_item, XV_KEY_DATA, UIC_OBJECT_KEY);
  }
 
  // Set the internal UIC event information
  object->setEventInformation (event);

  // Notify the user specified notify targets
  object->notifyNotifyTargets ();

  // Call the user specified notify handler
  if (object->useDefaultNotifyHandlerFlag)  
    object->defaultNotifyHandler (); 
  else if (object->notifyHandler)
    (*object->notifyHandler) (object);

  // Erase the internal UIC event information
  object->resetEventInformation ();

  // If the user specified a notify status
  if (object->notifyStatus != UIC_NOT_SET) { 
    // Set the notify status 
    xv_set (item, PANEL_NOTIFY_STATUS, object->notifyStatus, NULL);
  
    // Reset the internal notify status 
    object->notifyStatus = UIC_NOT_SET; 
  }

  // If the user specified a notify result...
  if (object->notifyResult != UIC_NOT_SET) {
    Panel_setting result = (Panel_setting) object->notifyResult;
    object->notifyResult = UIC_NOT_SET;
    
    // Return the user specified result
    return (result);
  }
  
  return (panel_text_notify (item, event));
}

/*F+ void UIInputTextItem::setUIInputTextItemAttributes (UIObject *parent)
*
*    PURPOSE:
*
*    Sets XView attributes common to classes derived from UIInputTextItem.
*
*    PARAMETER:
*
*    parent - The parent of the input text item (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIInputTextItem::setUIInputTextItemAttributes (UIObject *parent)
{
  // Set common XView attributes
  setUIComponentAttributes (parent);

  xv_set (owObject, PANEL_NOTIFY_PROC, &UIInputTextItem::notifyProc, NULL);

  if (labelPosition == ABOVE)
    xv_set (owObject, PANEL_LAYOUT, PANEL_VERTICAL, NULL);
  else
    xv_set (owObject, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);

  xv_set (owObject, PANEL_READ_ONLY, readOnly, NULL);

  if (echo == FALSE)
    xv_set (owObject, PANEL_MASK_CHAR, ' ', NULL);
  else if (mask)
    xv_set (owObject, PANEL_MASK_CHAR, mask, NULL);

  if (inputLength != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject,
	      XV_USE_DB,
	        PANEL_VALUE_STORED_LENGTH, inputLength,
	        NULL,
	      NULL);
    else
      xv_set (owObject, PANEL_VALUE_STORED_LENGTH, inputLength, NULL);
  }

  if (inputDisplayLength != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject,
	      XV_USE_DB,
	        PANEL_VALUE_DISPLAY_LENGTH, inputDisplayLength,
	        NULL,
	      NULL);
    else
      xv_set (owObject, PANEL_VALUE_DISPLAY_LENGTH, inputDisplayLength, NULL);
  }

  xv_set (owObject, PANEL_VALUE_UNDERLINED, underline, NULL);
}

#ifdef TEST
#endif /* TEST */
