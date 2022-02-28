/*H+ UIComponent.cc
*
*    SCCS ID: @(#)UIComponent.cc 1.5 93/02/08 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the UIComponent class.
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
static char sccsid [] = "@(#)UIComponent.cc 1.5 93/02/08 SMI"; 
#endif

// Includes
#include "CharacterFont.h"
#include "ColorMap.h"
#include "ComponentDisplay.h"
#include "HotRegion.h"
#include "UIComponent.h"

/*F+ UIComponent::UIComponent ()
*
*    PURPOSE:
*
*    Constructor of the UIComponent class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIComponent::UIComponent ()
{
  setUIComponentDefaults ();
}

/*F+ UIComponent::UIComponent (char *string_label)
*
*    PURPOSE:
*
*    Constructor of the UIComponent class.
*
*    PARAMETERS:
*
*    string_label - Specifies a string for the UIComponent label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIComponent::UIComponent (char *string_label) : UIObject (string_label)
{
  setUIComponentDefaults ();
}

/*F+ UIComponent::UIComponent (Image &image_label)
*
*    PURPOSE:
*
*    Constructor of the UIComponent class.
*
*    PARAMETERS:
*
*    image_label - Specifies an image for the UIComponent label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIComponent::UIComponent (Image &image_label)
{
  setUIComponentDefaults ();
  imageLabel = &image_label;
}

/*F+ UIComponent::setUIComponentDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the UIComponent class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setUIComponentDefaults ()
{
  valueX = UIC_NOT_SET;
  valueY = UIC_NOT_SET;
  active = TRUE;
  labelPosition = LEFT;
  notifyHandler = (UICHandlerFunction) NULL;
  notifyResult = UIC_NOT_SET;
  notifyStatus = UIC_NOT_SET;
  imageLabel = (Image *) NULL;
  labelFont = (CharacterFont *) NULL;
  valueFont = (CharacterFont *) NULL;
  isUIComponent = TRUE;
  useDefaultNotifyHandlerFlag = FALSE;
}

/*F+ UIComponent::~UIComponent ()
*
*    PURPOSE:
*
*    Destructor of the UIComponent class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIComponent::~UIComponent ()
{
}

/*F+ void UIComponent::setLabel (char *string_label)
*
*    PURPOSE:
*
*    Specifies the string to use for the object's label.
*
*    PARAMETERS
*
*    string_label - The string to use.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setLabel (char *string_label)
{
  if (label != (char *) NULL)
    delete label;

  if (string_label)
    label = strcpy (new char [strlen (string_label) + 1], string_label);
  else
    label = (char *) NULL;
  
  if (owObject) xv_set (owObject, PANEL_LABEL_STRING, label, NULL);
}

/*F+ void UIComponent::setLabel (Image &image_label)
*
*    PURPOSE:
*
*    Specifies the image to use for the object's label.
*
*    PARAMETERS
*
*    image_label - The image to use.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setLabel (Image &image_label)
{
  imageLabel = &image_label;
  if (owObject) {
    ((UIObject *) imageLabel)->createObject (this);
    xv_set (owObject,
	    PANEL_LABEL_IMAGE, imageLabel->getXViewObject (),
	    NULL);
  }
}

/*F+ void UIComponent::setLabelPosition (Position position)
*
*    PURPOSE:
*
*    Specifies the position of the label relative to the object.
*
*    PARAMETERS
*
*    position - The position of the label (ABOVE or LEFT).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setLabelPosition (Position position)
{
  labelPosition = position;

  if (owObject) {
    if (labelPosition == ABOVE)
      xv_set (owObject, PANEL_LAYOUT, PANEL_VERTICAL, NULL);
    else
      xv_set (owObject, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);
  }
}

/*F+ Image *UIComponent::getLabel (Image* &image_label)
*
*    PURPOSE:
*
*    Gets the UIComponent's image label.
*
*    PARAMETERS
*
*    image_label - The image label.
*
*    RETURN VALUES:
*
*    Returns a pointer to the image label.  Returns NULL if not set.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
Image *UIComponent::getLabel (Image* &image_label)
{
  return (image_label = imageLabel);
}

/*F+ void UIComponent::setActive (Boolean flag)
*
*    PURPOSE:
*
*    Specifies whether or not this UIComponent is active.
*
*    PARAMETERS
*
*    flag - TRUE if the UICompnent is active.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setActive (Boolean flag)
{
  active = flag;

  if (owObject) {
    if (isScroller)
      xv_set (owObject, SCROLLBAR_INACTIVE, (flag ? FALSE : TRUE), NULL);
    else
      xv_set (owObject, PANEL_INACTIVE, (flag ? FALSE : TRUE), NULL);
  }
}

/*F+ void UIComponent::setLabelFont (CharacterFont &font)
*
*    PURPOSE:
*
*    Specifies the font to use for this UIComponent's label.
*
*    PARAMETERS
*
*    font - The font to use.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setLabelFont (CharacterFont &font)
{
  labelFont = &font;

  if (owObject) {
    ((UIObject *) labelFont)->createObject (this);
    xv_set (owObject, PANEL_LABEL_FONT, labelFont->getXViewObject (), NULL);
  }
}

/*F+ void UIComponent::setValueFont (CharacterFont &font)
*
*    PURPOSE:
*
*    Specifies the font to use for this UIComponent's value field.
*
*    PARAMETERS
*
*    font - The font to use.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setValueFont (CharacterFont &font)
{
  valueFont = &font;

  if (owObject) {
    ((UIObject *) labelFont)->createObject (this);
    xv_set (owObject, PANEL_VALUE_FONT, valueFont->getXViewObject (), NULL);
  }
}

/*F+ void UIComponent::setValueX (int x)
*
*    PURPOSE:
*
*    Sets the X coordinate of this component's value.
*
*    PARAMETERS
*
*    x - The X coordinate.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setValueX (int x)
{
  valueX = x;

  if (owObject) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_VALUE_X, valueX, NULL, NULL);
    else
      xv_set (owObject, PANEL_VALUE_X, valueX, NULL);
  }
}

/*F+ void UIComponent::setValueY (int y)
* 
*    PURPOSE: 
* 
*    Sets the Y coordinate of this component's value. 
* 
*    PARAMETERS 
* 
*    y - The Y coordinate. 
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIComponent::setValueY (int y)
{ 
  valueY = y;
 
  if (owObject) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_VALUE_Y, valueY, NULL, NULL);
    else
      xv_set (owObject, PANEL_VALUE_Y, valueY, NULL);
  }
} 

/*F+ int UIComponent::getValueX ()
* 
*    PURPOSE: 
* 
*    Gets the X coordinate of this component's value. 
* 
*    RETURN VALUES: 
* 
*    Returns the X coordinate value.
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
int UIComponent::getValueX ()
{ 
  return (owObject ? xv_get (owObject, PANEL_VALUE_X) : valueX);
} 

/*F+ int UIComponent::getValueY () 
*  
*    PURPOSE: 
*  
*    Gets the Y coordinate of this component's value.  
*  
*    RETURN VALUES:  
*  
*    Returns the Y coordinate value. 
*  
*    REVISION HISTORY:  
*  
*    Who       When       Why  
*    --------- ---------- ------------------------  
*  
*F-*/  
int UIComponent::getValueY () 
{  
  return (owObject ? xv_get (owObject, PANEL_VALUE_Y) : valueY); 
}  
 
/*F+ void UIComponent::setForegroundColor (Color color)
*
*    PURPOSE:
*
*    Sets the foreground color of the object.
*
*    PARAMETERS:
*
*    color - A UIC color structure containing the RGB components of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setForegroundColor (Color color)
{
  if (!colorMap) {
    useDefaultColorMap ();
    if (owObject) initializeColorMap (serverParent);
  }
    
  foregroundColor = colorMap->findColor (color);
    
  if (owObject)
    xv_set (owObject, PANEL_ITEM_COLOR, foregroundColor, NULL);
}

/*F+ void UIComponent::setForegroundColor (char *color)
*
*    PURPOSE:
*
*    Sets the foreground color of the object.
*
*    PARAMETERS:
*
*    color - The name of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setForegroundColor (char *color)
{
  if (!colorMap) {
    useDefaultColorMap ();
    if (owObject) initializeColorMap (serverParent);
  }
    
  foregroundColor = colorMap->findColor (color);
    
  if (owObject)
    xv_set (owObject, PANEL_ITEM_COLOR, foregroundColor, NULL);
}

/*F+ int UIComponent::addNotifyTarget (UIObject          &object,
*                                      UICTargetFunction  func,
*				       int		  xview_event_action,
*				       int		  xview_event_id)
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
int UIComponent::addNotifyTarget (UIObject          &object,
				  UICTargetFunction  func,
				  int		     xview_event_action,
				  int		     xview_event_id)
{
  UITarget *target = new UITarget;

  target->targetObject = &object;
  target->targetFunction = func;
  target->eventAction = xview_event_action;
  target->eventID = xview_event_id;

  notifyTargets.addItem (target);
  return notifyTargets.getSize ();
}

/*F+ void UIComponent::notifyNotifyTargets ()
*
*    PURPOSE:
*
*    Internal function that loops through all of the notify targets
*    associated with this component and calls the corresponding
*    target functions.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::notifyNotifyTargets ()
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

/*F+ void UIComponent::setNotifyHandler ()
*
*    PURPOSE:
*
*    Specifies the notify handler.
*
*    PARAMETERS:
*
*    handler - The user defined notify handler.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setNotifyHandler (UICHandlerFunction handler)
{
  notifyHandler = handler;
}

/*F+ void UIComponent::eventProc (Panel_item item, Event *event)
*
*    PURPOSE:
*
*    Internal XView event proc wrapper.  This is called whenever an
*    event occurs on a component.
*
*    PARAMETERS:
*
*    item  - The item the event occurred on.
*    event - The XView event information structure.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::eventProc (Panel_item item, Event *event)
{
  UIComponent *object = (UIComponent *) xv_get (item,
					        XV_KEY_DATA,
						UIC_OBJECT_KEY);

  /* In XView V3, some of the components are compund items and the event
     may occur on the child item instead of the parent item.  If this
     happens, the parent (XV_OWNER) must be obtained in order to
     obtain a "this" pointer */
  if (!object) {
    Panel_item compound_item = xv_get (item, PANEL_ITEM_OWNER);
    object = (UIComponent *)
             xv_get (compound_item, XV_KEY_DATA, UIC_OBJECT_KEY);
  }

  object->setEventInformation (event);
  
  // Call the user specified event handler
  if (object->useDefaultEventHandlerFlag)  
    object->defaultEventHandler ();  
  else if (object->eventHandler)
    (*object->eventHandler) (object);
  
  object->resetEventInformation ();

  if (object->eventResultSet) {
     object->eventResultSet = FALSE;
     return;
  }

  panel_default_handle_event (item, event);
}

/*F+ void UIComponent::createWithFonts (UIObject *parent,
*                                       Xv_pkg   *package,
*					void     *attribute1,
*					void     *value1,
*					void     *attribute2,
*					void     *value2,
*					void     *attribute3,
*					void     *value3,
*					void     *attribute4,
*					void     *value4,
*                                       void     *terminator)
*
*    PURPOSE:
*
*    Internal function for creating an XView object that sets the values
*    of fonts in the xv_create call.  This function allows the setting of
*    PANEL_LABEL_FONT and PANEL_VALUE_FONT.
*
*    PARAMETERS:
*
*    parent     - The UIC parent of the object.
*    package    - The XView package that is being created.
*    attribute1 - An XView attribute name.
*    value1     - The value of the XView attribute.
*    attribute2 - Another XView attribute name.
*    value2     - The value of the second XView attribute.
*    terminator - Argument list terminator (allows compatibility with
*                 the use of var args in the future).
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::createWithFonts (UIObject *parent,
                                   Xv_pkg   *package,
				   void     *attribute1,
				   void     *value1,
				   void     *attribute2,
			           void     *value2,
				   void     *attribute3,
			           void     *value3,
				   void     *attribute4,
			           void     *value4,
                                   void     *terminator)
{
  if (labelFont)
    ((UIObject *) labelFont)->createObject (parent);
 
  if (valueFont)
    ((UIObject *) valueFont)->createObject (parent);
 
  if (labelFont && valueFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  XV_SHOW,          FALSE,
                          PANEL_LABEL_FONT, labelFont->getXViewObject (),
                          PANEL_VALUE_FONT, valueFont->getXViewObject (),
			  attribute1,       value1,
			  attribute2,       value2,
			  attribute3,       value3,
			  attribute4,       value4,
                          NULL);
  else if (labelFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  XV_SHOW,          FALSE,
                          PANEL_LABEL_FONT, labelFont->getXViewObject (),
			  attribute1,       value1,
			  attribute2,       value2,
			  attribute3,       value3,
			  attribute4,       value4,
                          NULL);
  else if (valueFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  XV_SHOW,          FALSE,
                          PANEL_VALUE_FONT, valueFont->getXViewObject (),
			  attribute1,       value1,
			  attribute2,       value2,
			  attribute3,       value3,
			  attribute4,       value4,
                          NULL);
  else
    owObject = xv_create (parent->getXViewObject (), package,
			  XV_SHOW,    FALSE,
			  attribute1, value1,
			  attribute2, value2,
			  attribute3, value3,
			  attribute4, value4,
			  NULL);
}    
 
/*F+ UIComponent::setUIComponentAttributes (UIObject *parent)
*
*    PURPOSE:
*
*    Sets common UIComponent XView attributes that are required for creating
*    the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the UIComponent object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::setUIComponentAttributes (UIObject *parent)
{
  if (resourceName) xv_set (owObject, XV_INSTANCE_NAME, resourceName, NULL);

  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  setHelpData ();

  if (xLoc != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_X, xLoc, NULL, NULL);
    else
      xv_set (owObject, XV_X, xLoc, NULL);
  }
  
  if (yLoc != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_Y, yLoc, NULL, NULL);
    else
      xv_set (owObject, XV_Y, yLoc, NULL);
  }
  
  if (width != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_WIDTH, width, NULL, NULL);
    else
      xv_set (owObject, XV_WIDTH, width, NULL);
  }

  if (height != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_HEIGHT, height, NULL, NULL);
    else
      xv_set (owObject, XV_HEIGHT, height, NULL);
  }
  
  if (valueX != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_VALUE_X, valueX, NULL, NULL);
    else
      xv_set (owObject, PANEL_VALUE_X, valueX, NULL);
  }

  if (valueY != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_VALUE_Y, valueY, NULL, NULL);
    else
      xv_set (owObject, PANEL_VALUE_Y, valueY, NULL);
  }

  initializeColorMap (parent);

  if (foregroundColor != UIC_NOT_SET)
    xv_set (owObject, PANEL_ITEM_COLOR, foregroundColor, NULL);
  
  if (label != (char *) NULL)
    xv_set (owObject, PANEL_LABEL_STRING, label, NULL);
  else if (imageLabel) {
    ((UIObject *) imageLabel)->createObject (parent);
    xv_set (owObject,
	    PANEL_LABEL_IMAGE, imageLabel->getXViewObject (),
	    NULL);
  }
  
  // If the user specified an event handler...
  if (eventHandler || useDefaultEventHandlerFlag || eventTargets.getSize () ||
      (draggable && !isDropTarget) || hotRegions.getSize ()) {
    xv_set (owObject, PANEL_EVENT_PROC, &UIComponent::eventProc, NULL);
    int noEvents = events.getSize ();
    for (int i = 0; i < noEvents; i++)
      xv_set (owObject, WIN_CONSUME_EVENT, (int) events [i], NULL); 
  }
  
  if (!active) xv_set (owObject, PANEL_INACTIVE, TRUE, NULL);

  UIObject::setUIObjectAttributes (parent);

  xv_set (owObject, XV_SHOW, showFlag, NULL);
}

/*F+ Boolean UIComponent::repaintComponent ()
*
*    PURPOSE:
*
*    This function is called to repaint the component if this is a
*    user defined component.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::repaintComponent (ComponentDisplay *cd)
{
}

/*F+ Boolean UIComponent::componentHotRegionEventProc (ComponentDisplay *cd)
*
*    PURPOSE:
*
*    This function is called if an event occurs in the hot region associated
*    with this component if this is a user defined component.
*
*    PARAMETERS:
*
*    cd - A pointer to the parent ComponentDisplay.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::componentHotRegionEventProc (ComponentDisplay *cd)
{
}

/*F+ Boolean UIComponent::componentHotRegionEventProc (ComponentDisplay *cd)
*
*    PURPOSE:
*
*    This function is called if an event occurs outside of the hot region
*    associated with this component, but inside of the components parent
*    ComponentDisplay object, if this is a user defined component.
*
*    PARAMETERS:
*
*    cd - A pointer to the parent ComponentDisplay.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::componentBackgroundEventProc (ComponentDisplay *cd)
{
}

/*F+ void UIComponent::createDragSite ()
*
*    PURPOSE:
*
*    Creates the site for sourcing drags from this object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::createDragSite ()
{
  if (!owObject) 
    createUserComponentDragSite (((ComponentDisplay *)
                                  parentObject)->getPaintWindow (),
				 *dragSiteHotRegion); 
  else if (!dragSite)
    createXViewDragSite (((ComponentDisplay *)
			  parentObject)->getPaintWindow ());
}

/*F+ void UIComponent::createDropSite ()
*
*    PURPOSE:
*
*    Creates the site for accepting drops on this object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::createDropSite ()
{
  if (!owObject) 
    createUserComponentDropSite (((ComponentDisplay *)
				  parentObject)->getPaintWindow (),
				 *dropSiteHotRegion);
  else if (!dropSite) {
    Rect *rectangle;

    if ((dropRectangle.r_width > 0) && (dropRectangle.r_height > 0))
      rectangle = &dropRectangle;
    else
      rectangle = (Rect *) xv_get (owObject, XV_RECT);

    createXViewDropSite (((ComponentDisplay *) parentObject)->getPaintWindow (),
			 rectangle);
  }
}

/*F+ void UIComponent::createHotRegionDragSite (HotRegion &hot_region)
*
*    PURPOSE:
*
*    Creates the site for sourcing drags on this object using the
*    specified hot region.
*
*    PARAMETERS:
*
*    hot_region - The hot region that specifies the size and location
*                 of the site.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::createHotRegionDragSite (HotRegion &hot_region)
{
  if (!hot_region.dragSite)
    hot_region.createXViewDragSite (((ComponentDisplay *)
				     parentObject)->getPaintWindow ());
}
 
/*F+ void UIComponent::createHotRegionDropSite (HotRegion &hot_region)
*
*    PURPOSE:
*
*    Creates the site for accepting drops on this object using the
*    specified hot region.
*
*    PARAMETERS:
*
*    hot_region - The hot region that specifies the size and location
*                 of the site.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::createHotRegionDropSite (HotRegion &hot_region)
{
  Rect rectangle;

  if (!hot_region.dropSite) {
    if ((hot_region.dropRectangle.r_width > 0) &&
	(hot_region.dropRectangle.r_height > 0)) 
      rectangle = hot_region.dropRectangle;
    else {
      int rect_x, rect_y, rect_w, rect_h;

      hot_region.getDefaultRectangle (rect_x, rect_y, rect_w, rect_h);
      rect_construct (&rectangle, rect_x, rect_y, rect_w, rect_h);
    }

    hot_region.createXViewDropSite (((ComponentDisplay *)
				     parentObject)->getPaintWindow (),
				    &rectangle);
  }
}

/*F+ void UIComponent::createUserComponentDragSite (HotRegion  &hot_Region,
*						    UICGeneric  xview_object)
*
*    PURPOSE:
*
*    Creates the XView drag site for the specified XView object.
*
*    PARAMETERS:
*
*    hot_region   - The hor region used to detect the drag dite event.
*    xview_object - The XView object to create the drag site for.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::createUserComponentDragSite (UICGeneric  xview_object,
				               HotRegion  &hot_region)
{
  Xv_server server = XV_SERVER_FROM_WINDOW (xview_object);

  hot_region.draggable = TRUE;
  parentObject->addHotRegion (hot_region);

  dragSite = xv_create (xview_object, DRAGDROP,
                        SEL_CONVERT_PROC, &UIObject::dragSelectionConvertProc,
                        NULL);

  xv_set (dragSite, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  xv_set (xview_object,
          WIN_CONSUME_EVENTS,
            LOC_DRAG,
            WIN_MOUSE_BUTTONS,
            NULL,
          NULL);
 
  if (dragCursorImage) {
    ((UIObject *) dragCursorImage)->createObject (serverParent);
    dragCursor = xv_create (serverParent->getXViewObject (), CURSOR,
                            CURSOR_IMAGE, dragCursorImage->getXViewObject (),
                            CURSOR_OP,    PIX_SRC^PIX_DST,
                            CURSOR_XHOT,  dragCursorX,
                            CURSOR_YHOT,  dragCursorY,
                            NULL);
 
    xv_set (dragSite, DND_CURSOR, dragCursor, NULL);
  }
 
  if (acceptDropCursorImage) {
    ((UIObject *) acceptDropCursorImage)->createObject (serverParent);
    acceptDropCursor = xv_create (serverParent->getXViewObject (), CURSOR,
                                  CURSOR_IMAGE,
                                    acceptDropCursorImage->getXViewObject (),
                                  CURSOR_OP,   PIX_SRC^PIX_DST,
                                  CURSOR_XHOT, acceptDropCursorX,
                                  CURSOR_YHOT, acceptDropCursorY,
                                  NULL);
 
    xv_set (dragSite, DND_ACCEPT_CURSOR, acceptDropCursor, NULL);
  }
 
  dragMessageSelection = xv_create (dragSite, SELECTION_ITEM,
                                    SEL_DATA, dragMessage,
                                    NULL);
 
  dragFileNameSelection = xv_create (dragSite, SELECTION_ITEM,
                    		     SEL_TYPE, (Atom) xv_get (server,
						              SERVER_ATOM,
							      "FILE_NAME"),
                                     SEL_DATA, dragFileName,
                                     NULL);
 
  (void) xv_create (dragSite, SELECTION_ITEM,
                    SEL_TYPE, (Atom) xv_get (server, SERVER_ATOM, "HOST_NAME"),
                    SEL_DATA, applicationHostName,
                    NULL);
 
  dragObjectSelection =
    xv_create (dragSite, SELECTION_ITEM,
               SEL_TYPE,   (Atom) xv_get (server, SERVER_ATOM, "OBJECT"),
               SEL_FORMAT, 32,
               SEL_LENGTH, 1,
               SEL_DATA,   &dragObject,
               NULL);

  hot_region.dragSite = dragSite;
  hot_region.dragFileNameSelection = dragFileNameSelection;
  hot_region.dragMessageSelection = dragMessageSelection;
  hot_region.dragObjectSelection = dragObjectSelection;
}                 

/*F+ void UIComponent::createUserComponentDropSite (UICGeneric  xview_object,
*					            HotRegion  &hot_region)
*
*    PURPOSE:
*
*    Creates the XView drop site for the specified XView object and region.
*
*    PARAMETERS:
*
*    xview_object - The XView object to create the drop site for.
*    hot_region   - The hot region defining hte drop site size.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIComponent::createUserComponentDropSite (UICGeneric  xview_object,
				               HotRegion  &hot_region)
{
  Rect rectangle;

  if ((dropRectangle.r_width > 0) && (dropRectangle.r_height > 0))
    rectangle = dropRectangle;
  else if ((hot_region.dropRectangle.r_width > 0) &&
      (hot_region.dropRectangle.r_height > 0))
    rectangle = hot_region.dropRectangle;
  else {
    int rect_x, rect_y, rect_w, rect_h;
         
    hot_region.getDefaultRectangle (rect_x, rect_y, rect_w, rect_h);
    rect_construct (&rectangle, rect_x, rect_y, rect_w, rect_h);
  }   
 
  dropSite = xv_create (xview_object, DROP_SITE_ITEM,
                        DROP_SITE_ID,         uiObjectID,
                        DROP_SITE_REGION,     &rectangle,
                        DROP_SITE_EVENT_MASK, DND_ENTERLEAVE |
					      DND_MOTION,
                        DROP_SITE_DEFAULT,    defaultDropSite,
                        NULL);

  xv_set (dropSite, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  dropSiteList->addItem ((Generic *) dropSite);
}

#ifdef TEST
#endif /* TEST */
