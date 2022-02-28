/*H+ ComponentDisplay.cc
*
*    SCCS ID: @(#)ComponentDisplay.cc 1.7 93/04/09 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines member function of the ComponentDisplay class.
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
static char sccsid [] = "@(#)ComponentDisplay.cc 1.7 93/04/09 SMI"; 
#endif

// Includes
#include "CharacterFont.h"
#include "ComponentDisplay.h"

/*F+ ComponentDisplay::ComponentDisplay ()
*
*    PURPOSE:
*
*    ComponentDisplay constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ComponentDisplay::ComponentDisplay ()
{
  /* This component display is not an XView panel until components are added
     (it is currently an XView canvas) */
  isPanel = FALSE;

  setComponentDisplayDefaults ();
}

/*F+ ComponentDisplay::ComponentDisplay (Boolean force_panel)
*
*    PURPOSE:
*
*    ComponentDisplay constructor.
*
*    PARAMETERS:
*
*    force_panel - If TRUE, the component display is forced to be
*                  a panel whether or not components have been added.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ComponentDisplay::ComponentDisplay (Boolean force_panel)
{
  /* The force_panel Boolean determines whether or not this component
     display is an XView panel or canvas */
  isPanel = force_panel;

  setComponentDisplayDefaults ();
}

/*F+ void ComponentDisplay::setComponentDisplayDefaults ()
*
*    PURPOSE:
*
*    Initializes various ComponentDisplay data members.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::setComponentDisplayDefaults ()
{
  focusComponent = (UIComponent *) NULL;
  componentHotRegion = FALSE;
  dropComponents = FALSE;
  numScrollers = 0;
  numCanvasComponents = 0;
  model = XVIEW;
  defaultButton = (Button *) NULL;
  dynamic = FALSE;
  bitGravity = NorthWestGravity;
  displayMenu = (UIMenu *) NULL;
  fitHeight = FALSE;
  fitWidth = FALSE;
  paintHeight = UIC_NOT_SET;
  paintWidth = UIC_NOT_SET;
  autoExpand = UIC_NOT_SET;
  layoutDirection = HORIZONTAL;
  eventHandlerSet = FALSE;
  repaintHandlerSet = FALSE;
  repaintHandler = (UICHandlerFunction) NULL;
  resizeHandler = (UICHandlerFunction) NULL;
  paintColorMap = (ColorMap *) NULL;
  paintWindow = (Xv_window) NULL;
  canvasXDisplay = (Display *) NULL;
  canvasXWindow = (Window) NULL;
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
  canvasPSWindow = (NeWStoken) UIC_NOT_SET;
#endif
#endif
  canvasXRectlist = (Xv_xrectlist *) NULL;
  canvasRectlist = (Rectlist *) NULL;
  useDefaultRepaintHandlerFlag = FALSE;
  useDefaultResizeHandlerFlag = FALSE;
}

/*F+ ComponentDisplay::~ComponentDisplay ()
*
*    PURPOSE:
*
*    ComponentDisplay destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ComponentDisplay::~ComponentDisplay ()
{
  /* Clear out the repaint components list without actually deleting
     the components */
  repaintComponents.dropItems ();

  if (!destroyChildren) {
    componentList.dropItems ();
  }
  else { 
    UIObject *child; 
 
    Boolean reset_flag = TRUE;
    while (child = (UIObject *) componentList.traverse (reset_flag)) {
      reset_flag = FALSE;

      if (!((UIComponent *) child)->isComponentGroup)
        child->setDestroyChildren (TRUE);
    }
    componentList.clear ();
  } 
}

/*F+ void ComponentDisplay::setDefaultButton (Button &button)
*
*    PURPOSE:
*
*    Specifies the default Button for this ComponentDisplay.
*
*    PARAMETERS:
*
*    button - The default button.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::setDefaultButton (Button &button)
{
  defaultButton = &button;

  if (owObject) {
    if (!button.getXViewObject ())
      addComponent (button);

    xv_set (owObject, PANEL_DEFAULT_ITEM, button.getXViewObject (), NULL);
  }
}

/*F+ void ComponentDisplay::setFocusComponent (UIComponent &component)
*
*    PURPOSE:
*
*    Specifies the input focus for this ComponentDisplay.
*
*    PARAMETERS:
*
*    component - The component that has the input focus.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::setFocusComponent (UIComponent &component)
{
  focusComponent = &component;
 
  if (owObject) {
    if (!component.getXViewObject ())
      addComponent (component);
 
    xv_set (owObject, PANEL_CARET_ITEM, component.getXViewObject (), NULL);
  }
}

/*F+ UIComponent *ComponentDisplay::getFocusComponent ()
*
*    PURPOSE: 
* 
*    Gets the input focus for this ComponentDisplay. 
*
*    RETURN VALUES:
*
*    Returns the component that has the input focus.  Returns NULL
*    if no component currently has the focus of this ComponentDisplay.
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
UIComponent *ComponentDisplay::getFocusComponent () 
{ 
  if (owObject) { 
    UICGeneric object = xv_get (owObject, PANEL_CARET_ITEM); 
    focusComponent = (object ?
		      (UIComponent *) xv_get (object,
					      XV_KEY_DATA,
					      UIC_OBJECT_KEY) :
		      (UIComponent *) NULL);
  } 

  return focusComponent;
} 
  
/*F+ void ComponentDisplay::setForegroundColor (Color color)
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
void ComponentDisplay::setForegroundColor (Color color)
{
  // If no paint color map was specified, use the application color map
  if (!paintColorMap)
    if (colorMap) setPaintColorMap (*colorMap);

  // If no color map has been specified...
  if (!colorMap) {
    // Use the default UIC color map
    useDefaultColorMap ();
    setPaintColorMap (*colorMap);
  }
  
  // Find the foreground color index
  foregroundColor = paintColorMap->findColor (color);
  
  if (owObject) {
    xv_set (owObject, WIN_FOREGROUND_COLOR, foregroundColor, NULL);

    if (owObject != paintWindow)
      xv_set (paintWindow, WIN_FOREGROUND_COLOR, foregroundColor, NULL);
  }
}

/*F+ void ComponentDisplay::setForegroundColor (char *color)
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
void ComponentDisplay::setForegroundColor (char *color)
{
  // If no paint color map was specified, use the application color map
  if (!paintColorMap)
    if (colorMap) setPaintColorMap (*colorMap);

  // If no color map has been specified...
  if (!colorMap) {
    // Use the default UIC color map
    useDefaultColorMap ();
    setPaintColorMap (*colorMap);
  }
  
  // Find the foreground color index
  foregroundColor = paintColorMap->findColor (color);
  
  if (owObject) {
    xv_set (owObject, WIN_FOREGROUND_COLOR, foregroundColor, NULL);

    if (owObject != paintWindow)
      xv_set (paintWindow, WIN_FOREGROUND_COLOR, foregroundColor, NULL);
  }
}

/*F+ void ComponentDisplay::setBackgroundColor (Color color)
*
*    PURPOSE:
*
*    Sets the background color of the object.
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
void ComponentDisplay::setBackgroundColor (Color color)
{
  // If no paint color map was specified, use the application color map
  if (!paintColorMap)
    if (colorMap) setPaintColorMap (*colorMap);

  // If no color map has been specified...
  if (!colorMap) {
    // Use the default UIC color map
    useDefaultColorMap ();
    setPaintColorMap (*colorMap);
  }
  
  // Find the background color index
  backgroundColor = paintColorMap->findColor (color);
  
  if (owObject) {
    xv_set (owObject, WIN_BACKGROUND_COLOR, backgroundColor, NULL);

    if (owObject != paintWindow)
      xv_set (paintWindow, WIN_BACKGROUND_COLOR, backgroundColor, NULL);
  }
}

/*F+ void ComponentDisplay::setBackgroundColor (char *color)
*
*    PURPOSE:
*
*    Sets the background color of the object.
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
void ComponentDisplay::setBackgroundColor (char *color)
{
  // If no paint color map was specified, use the application color map
  if (!paintColorMap)
    if (colorMap) setPaintColorMap (*colorMap);

  // If no color map has been specified...
  if (!colorMap) {
    // Use the default UIC color map
    useDefaultColorMap ();
    setPaintColorMap (*colorMap);
  }
  
  // Find the background color index
  backgroundColor = paintColorMap->findColor (color);
  
  if (owObject) {
    xv_set (owObject, WIN_BACKGROUND_COLOR, backgroundColor, NULL);

    if (owObject != paintWindow)
      xv_set (paintWindow, WIN_BACKGROUND_COLOR, backgroundColor, NULL);
  }
}

/*F+ void ComponentDisplay::setMenu (UIMenu &menu)
*
*    PURPOSE:
*
*    Specifies the ComponentDisplay menu.
*
*    PARAMETERS:
*
*    menu - The menu to show.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::setMenu (UIMenu &menu)
{
  // Save the menu pointer
  displayMenu = &menu;

  // If the XView object has been created...
  if (owObject) {
    // Create the new XView menu
    ((UIObject *) displayMenu)->createObject (this);

    // If this is an XView panel...
    if (isPanel) {
      // If an old menu exists, delete it
      Menu old_menu = (Menu) xv_get (owObject, WIN_MENU);
      if (old_menu) xv_destroy_safe (old_menu);

      /* If no old menu existed, an XView panel background proc
	 needs to be created to catch the menu button presses */
      if (!old_menu)
	xv_set (owObject,
	        PANEL_BACKGROUND_PROC, &ComponentDisplay::backgroundEventProc,
		NULL);

      // Specify the new XView menu as the panel menu
      xv_set (owObject, WIN_MENU, displayMenu->getXViewObject (), NULL);
    }
    // Else, this is an XView canvas
    else {
      // If an old menu exists, delete it
      Menu old_menu = (Menu) xv_get (paintWindow, WIN_MENU);
      if (old_menu) xv_destroy_safe (old_menu);

      // Specify the new XView menu as the panel menu
      xv_set (paintWindow, WIN_MENU, displayMenu->getXViewObject (), NULL);

      // If no event handler has been set, set one to catch the menu
      // button presses
      if (!eventHandlerSet) {
        xv_set (owObject, WIN_EVENT_PROC, &ComponentDisplay::eventProc, NULL);
        xv_set (paintWindow,
		WIN_EVENT_PROC, &ComponentDisplay::eventProc,
		NULL);
      }
    }
  }
}

/*F+ UIMenu *ComponentDisplay::getMenu ()
*
*    PURPOSE:
*
*    Returns a pointer to the current ComponentDisplay menu.
*
*    RETURN VALUES:
*
*    Returns the pointer to the last set component display menu.
*    Returns NULL if no menu is set.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIMenu *ComponentDisplay::getMenu ()
{
  return displayMenu;
}

/*F+ void ComponentDisplay::setRepaintHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Specifies the ComponentDisplay repaint handler.
*
*    PARAMETERS:
*
*    handler - The function to call to repaint the component display.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::setRepaintHandler (UICHandlerFunction handler)
{
  repaintHandler = handler;

  /* If the XView object already exists and the repaint handler was
     never set before... */
  if (owObject && !repaintHandlerSet) {
    // If using the X11 drawing model...
    if (model == X11) {
      // If this is an XView panel...
      if (isPanel)
	xv_set (owObject,
		PANEL_REPAINT_PROC, &ComponentDisplay::repaintXProc,
		NULL);
      // Else, this is an XView canvas
      else
	xv_set (owObject,
		CANVAS_REPAINT_PROC, &ComponentDisplay::repaintXProc,
		NULL);
    }
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
    // Else, if using the PostScript drawing model...
    else if (model == POSTSCRIPT) {
      // If this is an XView panel...
      if (isPanel)
	xv_set (owObject,
		PANEL_REPAINT_PROC, &ComponentDisplay::repaintProc,
		NULL);
      // Else, this is an XView canvas
      else
	xv_set (owObject,
		PSCANVAS_REPAINT_PROC, &ComponentDisplay::repaintPSProc,
		NULL);
    }
#endif
#endif
    // Else, this is the XView drawing model...
    else {
      // If this is an XView panel...
      if (isPanel)
	xv_set (owObject,
		PANEL_REPAINT_PROC, &ComponentDisplay::repaintProc,
		NULL);
      // Else, this is an XView canvas
      else
	xv_set (owObject,
		CANVAS_REPAINT_PROC, &ComponentDisplay::repaintProc,
		NULL);
    }
    
    repaintHandlerSet = TRUE;
  }
}

/*F+ void ComponentDisplay::setResizeHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Specifies the ComponentDisplay resize handler.
*
*    PARAMETERS:
*
*    handler - The function to call to handle the resize.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::setResizeHandler (UICHandlerFunction handler)
{
  resizeHandler = handler;
}
 
/*F+ void ComponentDisplay::repaintProc (Canvas     canvas,
*                                        Xv_window  xview_window,
*					 Rectlist  *rectlist)
*
*    PURPOSE:
*
*    The XView drawing model repaint proc for the ComponentDisplay.
*    This is called by XView and is used to call the user specified
*    repaint handler.  This member function is protected.
*
*    PARAMETERS:
*
*    canvas       - The XView canvas or panel to repaint
*    xview_window - The paint window of the canvas or panel.
*    rectlist     - The clipping rectangles of the area that needs repainting.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::repaintProc (Canvas     canvas,
				    Xv_window  xview_window,
				    Rectlist  *rectlist)
{
  ComponentDisplay *object = (ComponentDisplay *) xv_get (canvas,
					                  XV_KEY_DATA,
							  UIC_OBJECT_KEY);
  
  // Save the clipping rectangle list
  object->canvasRectlist = rectlist;
  
  // Call the user specified handler
  if (object->useDefaultRepaintHandlerFlag)
    object->defaultRepaintHandler ();
  else if (object->repaintHandler)
    (*object->repaintHandler) (object);
  
  // Repaint user defined components
  object->repaintComponentsProc ();
  
  // If this is an XView panel...
  if (object->isPanel) {
    // ...and it is scrollable... use the paint window...
      
    /* If scrollable, use the paint window, else use the XView object...
       Specify not to clear the panel before repainting... */
    panel_paint (((object->numScrollers > 0) ? xview_window : canvas),
		 PANEL_NO_CLEAR);
  }
}

/*F+ void ComponentDisplay::repaintXProc (Canvas       canvas,
*                                         Xv_window    xview_window,
*                                         Display     *display,
*                                         Window       window,
*					  Xv_rectlist *rectlist)
*
*    PURPOSE:
*
*    The X11 drawing model repaint proc for the ComponentDisplay.
*    This is called by XView and is used to call the user specified
*    repaint handler.  This member function is protected.
*
*    PARAMETERS:
*
*    canvas       - The XView canvas or panel to repaint
*    xview_window - The paint window of the canvas or panel.
*    display      - The display pointer for the component display.
*    window       - The XID or Window ID of the component display.
*    rectlist     - The clipping rectangles of the area that needs repainting.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::repaintXProc (Canvas        canvas,
				     Xv_window     xview_window,
				     Display      *display,
				     Window        window,
				     Xv_xrectlist *rectlist)
{
  ComponentDisplay *object = (ComponentDisplay *) xv_get (canvas,
							  XV_KEY_DATA,
							  UIC_OBJECT_KEY);
  
  // Save the clipping rectangle list
  object->canvasXRectlist = rectlist;
  
  // Call the user specified handler
  if (object->useDefaultRepaintHandlerFlag)
    object->defaultRepaintHandler ();
  else if (object->repaintHandler)
    (*object->repaintHandler) (object);
  
  // Repaint user defined components
  object->repaintComponentsProc ();
  
  // If this is an XView panel...
  if (object->isPanel) {
    // ...and it is scrollable... use the paint window...
      
    /* If scrollable, use the paint window, else use the XView object...
       Specify not to clear the panel before repainting... */
    panel_paint (((object->numScrollers > 0) ? xview_window : canvas),
		 PANEL_NO_CLEAR);
  }
}

#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
/*F+ void ComponentDisplay::repaintPSProc (Canvas        canvas,
*                                          NeWStoken     ps_window,
*                                          Display      *display,
*                                          Window        window,
*					   Xv_xrectlist *rectlist)
*
*    PURPOSE:
*
*    The PostScript drawing model repaint proc for the ComponentDisplay.
*    This is called by XView and is used to call the user specified
*    repaint handler.  This member function is protected.
*
*    PARAMETERS:
*
*    canvas    - The XView canvas or panel to repaint
*    ps_window - The NeWS token of the canvas or panel.
*    display   - The display pointer for the component display.
*    window    - The XID or Window ID of the component display.
*    rectlist  - The clipping rectangles of the area that needs repainting.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::repaintPSProc (PScanvas      canvas,
				      NeWStoken     ps_window,
				      Display      *display,
				      Window        window,
				      Xv_xrectlist *rectlist)
{
  ComponentDisplay *object = (ComponentDisplay *) xv_get (canvas,
							  XV_KEY_DATA,
							  UIC_OBJECT_KEY);
  
  // Save the clipping rectangle list
  object->canvasXRectlist = rectlist;
  
  // Call the user specified handler
  if (object->useDefaultRepaintHandlerFlag)
    object->defaultRepaintHandler ();
  else if (object->repaintHandler)
    (*object->repaintHandler) (object);
  
  // Repaint the user defined components
  object->repaintComponentsProc ();
  
  // If this is an XView panel...
  if (object->isPanel)
    /* Specify not to clear the panel before repainting... */
    panel_paint (canvas, PANEL_NO_CLEAR);
}
#endif
#endif /* NO_XVPS */

/*F+ void ComponentDisplay::resizeProc (Canvas canvas, int width, int height)
*
*    PURPOSE:
*
*    Provides an internal wrapper for XView canvas resize procs.
*
*    PARAMETERS:
*
*    canvas - The XView canvas or panel that was resized.
*    width  - The new width.
*    height - The new height.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::resizeProc (Canvas canvas, int width, int height)
{
  ComponentDisplay *object = (ComponentDisplay *) xv_get (canvas,
                                                          XV_KEY_DATA,
                                                          UIC_OBJECT_KEY);

  // If there is a drop site, resize it...
  if (object->dropSite)
    object->changeDropSiteSize ((Rect *) xv_get (object->paintWindow, XV_RECT));
 
  // Call the user specified handler
  if (object->useDefaultResizeHandlerFlag)
    object->defaultResizeHandler ();
  else if (object->resizeHandler)
    (*object->resizeHandler) (object);
}

/*F+ void ComponentDisplay::backgroundEventProc (Panel  panel,
*                                                Event *event)
*
*    PURPOSE:
*
*    The XView panel background event proc.  This is called whenever an
*    event occurs on a panel that is not on top of an XView component.
*    If the user specified a panel menu, then the menu will be shown
*    if the menu mouse button was pressed and the event did not occur
*    in a user defined component.
*
*    PARAMETERS:
*
*    panel - The panel the event occurred on.
*    event - The XView event info structure.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::backgroundEventProc (Panel panel, Event *event)
{
  ComponentDisplay *object = (ComponentDisplay *) xv_get (panel,
							  XV_KEY_DATA,
							  UIC_OBJECT_KEY);

  // Set the UIC event information
  object->setEventInformation (event);

  /* If the event was not on a user defined component and the mouse
     menu button is down... */
  if (!object->componentHotRegion &&
      (event_action (event) == ACTION_MENU) && event_is_down (event)) {
    // If a menu is specified, show it...
    Menu menu = (Menu) xv_get (panel, WIN_MENU);
    if (menu) menu_show (menu, panel, event, NULL);
  }

  // Erase the UIC event information
  object->resetEventInformation ();
}

/*F+ void ComponentDisplay::eventProc (Xv_window win, Event *event)
*
*    PURPOSE:
*
*    The XView canvas event proc.  This was set by WIN_EVENT_PROC and
*    is called whenever an event occurs on a canvas.
*
*    PARAMETERS:
*
*    win   - The XView object the event occurred on.
*    event - The XView event info structure.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::eventProc (Xv_window win, Event *event)
{
  ComponentDisplay *object = (ComponentDisplay *) xv_get (win,
							  XV_KEY_DATA,
							  UIC_OBJECT_KEY);
  
  // Set the UIC event information
  object->setEventInformation (event);

  // Call the user specified event handler
  if (object->useDefaultEventHandlerFlag)
    object->defaultEventHandler ();
  else if (object->eventHandler)
    (*object->eventHandler) (object);

  object->componentHotRegion = FALSE;

  // Loop through all of the user defined component regions
  for (int i = object->componentHotRegions.getSize () - 1; i > -1; i--) {
    HotRegion   *hot_region = ((ComponentHotRegion *)
		               object->componentHotRegions [i])->hotRegion;
    UIComponent *component = ((ComponentHotRegion *)
		              object->componentHotRegions [i])->component;
    Boolean background =
	    ((ComponentHotRegion *)
	     object->componentHotRegions [i])->trapBackgroundEvents;

    // If this event is in a component hot region...
    if (hot_region->detectHotRegionEvent (object, event)) {
      object->componentHotRegion = TRUE;

      // Call the user defined component hot region event proc
      component->componentHotRegionEventProc (object);

      // If this component is not event transparent...
      if (!hot_region->isTransparent ())
	break;
    }
    /* Else, if background events (any event not on the component,
       but on the panel) events are to be detected for the user defined
       component... */
    else if (background)
      component->componentBackgroundEventProc (object);
  }

  /* If this is an XView canvas and not on a user defined component
     and the mouse menu button is down... */ 
  if (!object->isPanel && !object->componentHotRegion &&
      (event_action (event) == ACTION_MENU) && event_is_down (event)) {
    // If a menu is specified, show it...
    Menu menu = (Menu) xv_get (win, WIN_MENU);
    if (menu) menu_show (menu, win, event, NULL);
  }

  // Erase the UIC event information
  object->resetEventInformation ();
}

/*F+ Notify_value ComponentDisplay::panelEventProc (Xv_window          win,
*					            Event             *event,
*					            Notify_arg         arg,
*					            Notify_event_type  type)
*
*    PURPOSE:
*
*    The XView panel event proc.  This was set with a notify interpose and
*    is called whenever an event occurs on a panel.
*
*    PARAMETERS:
*
*    win   - The XView object the event occurred on.
*    event - The XView event info structure.
*    arg   - XView client data.
*    type  - The XView notifier event type: SAFE or IMMEDIATE.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
Notify_value ComponentDisplay::panelEventProc (Xv_window          win,
				               Event             *event,
				               Notify_arg         arg,
				               Notify_event_type  type)
{
  ComponentDisplay *object = (ComponentDisplay *) xv_get (win,
                                                          XV_KEY_DATA,
                                                          UIC_OBJECT_KEY);
 
  static int last_action;

  // Set the UIC event information
  object->setEventInformation (event);
 
  /*
   * Because resize procs are not called for panels, detect if a resize
   * event occurred and do any resize related stuff that kay be required...
   */
  if ((event_action (event) == WIN_REPAINT) && (last_action == WIN_RESIZE)) {
    // If there is a drop site, resize it...
    if (object->dropSite && object->owObject)
      object->changeDropSiteSize((Rect *) xv_get (object->owObject, XV_RECT));

    // Call the user specified handler
    if (object->useDefaultResizeHandlerFlag)
      object->defaultResizeHandler ();
    else if (object->resizeHandler)
      (*object->resizeHandler) (object);
  }

  // Call the user specified event handler
  if (object->useDefaultEventHandlerFlag)
    object->defaultEventHandler ();
  else if (object->eventHandler)
    (*object->eventHandler) (object);
 
  object->componentHotRegion = FALSE;
 
  // Loop through all of the user defined component regions
  for (int i = object->componentHotRegions.getSize () - 1; i > -1; i--) {
    HotRegion   *hot_region = ((ComponentHotRegion *)
                               object->componentHotRegions [i])->hotRegion;
    UIComponent *component = ((ComponentHotRegion *)
                              object->componentHotRegions [i])->component;
    Boolean background =
            ((ComponentHotRegion *)
             object->componentHotRegions [i])->trapBackgroundEvents;
 
    // If this event is in a component hot region...
    if (hot_region->detectHotRegionEvent (object, event)) {
      object->componentHotRegion = TRUE;
 
      // Call the user defined component hot region event proc
      component->componentHotRegionEventProc (object);
 
      // If this component is not event transparent...
      if (!hot_region->isTransparent ())
        break;
    }
    /* Else, if background events (any event not on the component,
       but on the panel) events are to be detected for the user defined
       component... */
    else if (background)
      component->componentBackgroundEventProc (object);
  }

  /* If this is an XView canvas and not on a user defined component
     and the mouse menu button is down... */
  if (!object->isPanel && !object->componentHotRegion &&
      (event_action (event) == ACTION_MENU) && event_is_down (event)) {
    // If a menu is specified, show it...
    Menu menu = (Menu) xv_get (win, WIN_MENU);
    if (menu) menu_show (menu, win, event, NULL);
  }
 
  // Save the event action for use by the next event on this object
  last_action = event_action (event);

  // Erase the UIC event information
  object->resetEventInformation ();

  if (object->eventResultSet) { 
    object->eventResultSet = FALSE; 
    return (object->eventResult); 
  } 

  return (notify_next_event_func (win, (Notify_event) event, arg, type));
}

/*F+ void ComponentDisplay::addComponent (UIComponent &component)
*
*    PURPOSE:
*
*    Adds the specified component to the component display.
*
*    PARAMETERS:
*
*    component - The component to add.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::addComponent (UIComponent &component)
{
  // Add the component to the list of components
  componentList.addItem ((Generic *) (&component));
  
  // If the component is a scroller, increment the scroller count
  if (component.isScroller)
    numScrollers++;

  /* If the component is drawable on a canvas, increment the canvas
     component count */
  if (component.isDrawableOnCanvas)
    numCanvasComponents++;
  
  // See if this component accepts drops
  if (component.acceptDrops && !component.isDropTarget)
    dropComponents = TRUE;

  // If the XView component display object has already been created...
  if (owObject) {
    // Create the XView component object
    ((UIObject *) &component)->createObject (this);
    
    // Get the XView pointer to the object
    UICGeneric object = ((UIObject *) &component)->getXViewObject ();

    // If the XView object was created...
    if (object)
      // Store the XView frame pointer of the parent window as key data
      xv_set (object,
	      XV_KEY_DATA, UIC_FRAME_KEY, (Frame) xv_get (owObject,
							  XV_KEY_DATA,
							  UIC_FRAME_KEY),
	      NULL);

    /* If an event handler has not been set and a user defined component
       or a component that accepts drops has been added... */
    if (!eventHandlerSet &&
	(dropComponents || (componentHotRegions.getSize () > 0))) {
      // Set an XView event handler proc
      if (!isPanel) {
        xv_set (owObject, WIN_EVENT_PROC, &ComponentDisplay::eventProc, NULL);
        xv_set (paintWindow,
	        WIN_EVENT_PROC, &ComponentDisplay::eventProc,
	        NULL);
      }
      else {
        notify_interpose_event_func (owObject,
                                     (Notify_func)
				     &ComponentDisplay::panelEventProc,
                                     NOTIFY_SAFE);
        notify_interpose_event_func (owObject,
                                     (Notify_func)
				     &ComponentDisplay::panelEventProc,
                                     NOTIFY_IMMEDIATE);
      }
      
      // Set the user specified events to be consumed
      int num_events = events.getSize (); 
      for (int i = 0; i < num_events; i++) {
        xv_set (paintWindow, WIN_CONSUME_EVENT, (int) events [i], NULL);

	if (!isPanel)
          xv_set (owObject, WIN_CONSUME_EVENT, (int) events [i], NULL);
      }
      
      eventHandlerSet = TRUE;
    }
    
    /* If a repaint handler has not been set and a user defined component
       has been added... */
    if (!repaintHandlerSet && (repaintComponents.getSize () > 0)) {
      // If using the X11 drawing model...
      if (model == X11) {
	// If this is an XView panel...
	if (isPanel)
	  xv_set (owObject,
		  PANEL_REPAINT_PROC, &ComponentDisplay::repaintXProc,
		  NULL);
	// Else, this is an XView canvas
	else
	  xv_set (owObject,
		  CANVAS_REPAINT_PROC, &ComponentDisplay::repaintXProc,
		  NULL);
      }
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
      // Else, if using the PostScript drawing model...
      else if (model == POSTSCRIPT) {
	// If this is an XView panel...
	if (isPanel)
	  xv_set (owObject,
		  PANEL_REPAINT_PROC, &ComponentDisplay::repaintProc,
		  NULL);
	// Else, this is an XView canvas
	else
	  xv_set (owObject,
		  PSCANVAS_REPAINT_PROC, &ComponentDisplay::repaintPSProc,
		  NULL);
      }
#endif
#endif
      // Else, this is the XView drawing model...
      else {
	// If this is an XView panel...
	if (isPanel)
	  xv_set (owObject,
		  PANEL_REPAINT_PROC, &ComponentDisplay::repaintProc,
		  NULL);
	// Else, this is an XView canvas
	else
	  xv_set (owObject,
		  CANVAS_REPAINT_PROC, &ComponentDisplay::repaintProc,
		  NULL);
      }
    
      repaintHandlerSet = TRUE;
    }
  }
}

/*F+ void ComponentDisplay::removeComponent (UIComponent &component)
*
*    PURPOSE:
*
*    Removes the specified component from the component display.  The
*    object must still be deleted to reclaim the memory it uses and to
*    destroy the XView object.
*
*    PARAMETERS:
*
*    component - The component to add.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::removeComponent (UIComponent &component)
{ 
  Boolean found = FALSE;
  int     index;
 
  if ((index = componentList.findItem (&component)) > -1) {
    (void) componentList.removeItem (index);
    found = TRUE;
  }

  if ((index = repaintComponents.findItem (&component)) > -1) {
    (void) repaintComponents.removeItem (index); 
    found = TRUE;
  }
 
  int num_hot_regions = componentHotRegions.getSize ();
  for (int i=0; i < num_hot_regions; i++) { 
    if (((ComponentHotRegion *)
         componentHotRegions [i])->component == &component) {
      (void) componentHotRegions.removeItem (i); 
      found = TRUE;
      break; 
    } 
  } 

  if (found) component.show (FALSE);
}
 
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
/*F+ void ComponentDisplay::storePSDrawImageProc (char    *name,
*                                                 Image   &image,
*                                                 Boolean  transparent,
*                                                 Boolean  invert)
*
*    PURPOSE:
*
*    Creates and names a PostScript procedure that draws the specified image
*    using moveto and rlineto.
*
*    PARAMETERS:
*
*    name        - Specifies the name of the PostScript procedure.
*    image       - The image the procedure will draw.
*    transparent - If TRUE, 0 valued pixels of color images are transparent.
*    invert      - If TRUE, inverts black and white images.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::storePSDrawImageProc (char    *name,
					     Image   &image,
					     Boolean  transparent,
					     Boolean  invert)
{
  ((UIObject *) &image)->createObject ();

  int         d = image.getDepth ();
  int         w = image.getWidth ();
  int         h = image.getHeight ();
  
  UICGeneric  xview_image = image.getXViewObject ();
  Display    *display = (Display *) xv_get (xview_image, XV_DISPLAY);
  XID         xid = (XID) xv_get (xview_image, XV_XID);
  
  // Get the X image corresponding to the UIC image object
  XImage *x_image = XGetImage (display, xid, 0, 0, w, h, 1, XYPixmap);
  
  char temp_string [128];
  
  // PostScript that specifies the procedure name
  POSTSCRIPT_STRING ("/");
  POSTSCRIPT_STRING (name);
  POSTSCRIPT_STRING (" { ");
  POSTSCRIPT_STRING ("/y exch def ");
  POSTSCRIPT_STRING ("/x exch def ");

  unsigned long pixel;
  
  // Loop though all of the X image pixels
  for (int j=0; j < h; j++) {
    ps_flush_PostScript ();
    for (int i=0; i < w; i++) {
      // If the image depth is 1 (black and white)
      if (d == 1) {
	// Get the pixel value
	pixel = XGetPixel (x_image, i, j);

	/* If the pixel is on and the image is not inverted or
	   the pixel is off and the image is inverted... */
	if ((pixel && !invert) || (!pixel && invert)) { 
	  // PostScript to move to the x-y location and draw a point
	  POSTSCRIPT_STRING ("x y moveto ");
	  sprintf (temp_string, "%d %d rmoveto ", i, j);
	  POSTSCRIPT_STRING (temp_string);
	  POSTSCRIPT_STRING ("0 0 rlineto "); 
	}
      }
      // Else the image is color
      else {
	XColor   color;
	Colormap cmap = DefaultColormap (getXDisplay (),
					 DefaultScreen (getXDisplay ()));

	// If 0 pixel values are transparent...
	if (transparent) {
	  // If the pixel is non-zero... */
	  if (color.pixel = XGetPixel (x_image, i, j)) { 
	    // Get the pixel RGB components
	    XQueryColor (getXDisplay (), cmap, &color);

	    // PostScript to move to the x-y location and draw a color point
	    sprintf (temp_string,
		     "%f %f %f setrgbcolor ",
		     ((float) color.red) / 255.0,
		     ((float) color.green) / 255.0,
		     ((float) color.blue) / 255.0);
	    POSTSCRIPT_STRING ("x y moveto ");
	    sprintf (temp_string, "%d %d rmoveto ", i, j);
	    POSTSCRIPT_STRING (temp_string);
	    POSTSCRIPT_STRING ("0 0 rlineto "); 
	  }
	}
	else {
	  // Get the pixel RGB components
	  color.pixel = XGetPixel (x_image, i, j);
	  XQueryColor (getXDisplay (), cmap, &color);

	  // PostScript to move to the x-y location and draw a color point
	  sprintf (temp_string,
		   "%f %f %f setrgbcolor ",
		   ((float) color.red) / 255.0,
		   ((float) color.green) / 255.0,
		   ((float) color.blue) / 255.0);
	  POSTSCRIPT_STRING ("x y moveto ");
	  sprintf (temp_string, "%d %d rmoveto ", i, j);
	  POSTSCRIPT_STRING (temp_string);
	  POSTSCRIPT_STRING ("0 0 rlineto "); 
	}
      }
    }
  }

  // PostScript to "stroke" the image and end the procedure definition
  POSTSCRIPT_STRING ("stroke ");
  POSTSCRIPT_STRING ("} def ");
  ps_flush_PostScript ();
}

/*F+ void ComponentDisplay::storePSImageProc (char  *name,
*                                             Image &image)
*
*    PURPOSE:
*
*    Creates and names a PostScript procedure that draws the specified image
*    using the PostScript "image" command.
*
*    PARAMETERS:
*
*    name        - Specifies the name of the PostScript procedure.
*    image       - The image the procedure will draw.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
-*/
void ComponentDisplay::storePSImageProc (char *name, Image &image)
{
  ((UIObject *) &image)->createObject ();

  int         d = image.getDepth ();
  int         w = image.getWidth ();
  int         h = image.getHeight ();
  
  UICGeneric  xview_image = image.getXViewObject ();
  Display    *display = (Display *) xv_get (xview_image, XV_DISPLAY);
  XID         xid = (XID) xv_get (xview_image, XV_XID);

  // Create an X plane mask that corresponds to the image depth
  unsigned long plane_mask = (1 << d) - 1;

  // Get the X image corresponding to the UIC image
  XImage *x_image = XGetImage (display, xid, 0, 0, w, h, plane_mask, XYPixmap);
  
  char temp_string [128];
  
  // PostScript to define the procedure name
  POSTSCRIPT_STRING ("/");
  POSTSCRIPT_STRING (name);
  POSTSCRIPT_STRING (" { ");
  POSTSCRIPT_STRING ("/y exch def ");
  POSTSCRIPT_STRING ("/x exch def ");

  // Set up a transform to be used by the "image" command
  POSTSCRIPT_STRING ("[0 0 0 0 0 0] currentmatrix ");
  POSTSCRIPT_STRING ("x y translate ");
  sprintf (temp_string, "%d %d scale ", w, h);
  POSTSCRIPT_STRING (temp_string);

  // Specify the height and width of the image data
  sprintf (temp_string, "%d %d 8 ", w, h);
  POSTSCRIPT_STRING (temp_string);

  // Specify an image translation array
  sprintf (temp_string, "[%d 0 0 %d 0 %d] ", w, -h, h);
  POSTSCRIPT_STRING (temp_string);
  POSTSCRIPT_STRING ("{<");

  // Determine the factor to scale X colors to 0-255 range
  float scale_factor = 255.0 / plane_mask;

  unsigned long pixel;

  // Loop through the X image pixels
  for (int j=0; j < h; j++)
    for (int i=0; i < w; i++) {
      // Get the X pixel
      pixel = XGetPixel (x_image, i, j);
      
      // Scale the X pixel to the PostScript pixel value (0-255)
      sprintf (temp_string, "%x", pixel * scale_factor);

      // write the pixel as PostScript with 2 characters
      if (strlen (temp_string) == 1)
	POSTSCRIPT_STRING ("0");
      POSTSCRIPT_STRING (temp_string);
    }

  // Specify the "image" command and end the procedure definition
  POSTSCRIPT_STRING (">} ");
  POSTSCRIPT_STRING ("image ");
  POSTSCRIPT_STRING ("setmatrix ");
  POSTSCRIPT_STRING ("} def ");
  ps_flush_PostScript ();
}

/*F+ void ComponentDisplay::storePSImageMaskProc (char    *name,
*                                                 Image   &image,
*                                                 Boolean  invert)
*
*    PURPOSE:
*
*    Creates and names a PostScript procedure that draws the specified image
*    using the PostScript "imagemask" command.
*
*    PARAMETERS:
*
*    name   - Specifies the name of the PostScript procedure.
*    image  - The image the procedure will draw.
*    invert - Tell the imagemask command to invert the image. 
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
-*/
void ComponentDisplay::storePSImageMaskProc (char    *name,
					     Image   &image,
					     Boolean  invert)
{
  ((UIObject *) &image)->createObject ();

  int         w = image.getWidth ();
  int         h = image.getHeight ();
  
  UICGeneric  xview_image = image.getXViewObject ();
  Display    *display = (Display *) xv_get (xview_image, XV_DISPLAY);
  XID         xid = (XID) xv_get (xview_image, XV_XID);

  // Get the X image corresponding to the UIC image
  XImage *x_image = XGetImage (display, xid, 0, 0, w, h, 1, XYPixmap);
  
  char temp_string [128];
  
  // PostScript to define the procedure name
  POSTSCRIPT_STRING ("/");
  POSTSCRIPT_STRING (name);
  POSTSCRIPT_STRING (" { ");
  POSTSCRIPT_STRING ("/y exch def ");
  POSTSCRIPT_STRING ("/x exch def ");

  // Set up a transform to be used by the "imagemask" command
  POSTSCRIPT_STRING ("[0 0 0 0 0 0] currentmatrix ");
  POSTSCRIPT_STRING ("x y translate ");
  sprintf (temp_string, "%d %d scale ", w, h);
  POSTSCRIPT_STRING (temp_string);

  // Specifiy the width and height of the imagemask data
  sprintf (temp_string, "%d %d ", w, h);
  POSTSCRIPT_STRING (temp_string);

  // Specify whether or not to invert the imagemask
  if (invert) {
    POSTSCRIPT_STRING ("true ");
  }
  else {
    POSTSCRIPT_STRING ("false ");
  }

  // Specify an image translation array
  sprintf (temp_string, "[%d 0 0 %d 0 0] ", w, h);
  POSTSCRIPT_STRING (temp_string);
  POSTSCRIPT_STRING ("{<");

  int byte = 0;

  // Loop through the X image pixel bits and convert to byte data
  for (int j=0; j < h; j++)
    for (int i=0; i < w; i++) {
      // Add the on pixels into the current byte
      if (XGetPixel (x_image, i, j))
	byte += (1 << (7 - (i % 8)));

      // If we have processed 8 pixels, we have a byte...
      if (((i % 8) == 7) || (i == w-1)) {
	// Write the byte as PostScript with 2 characters
	sprintf (temp_string, "%x", byte);

	if (strlen (temp_string) == 1)
	  POSTSCRIPT_STRING ("0");
	POSTSCRIPT_STRING (temp_string);

	byte = 0;
      }
    }

  // Specifiy the "imagemask" command and end the procedure definition
  POSTSCRIPT_STRING (">} ");
  POSTSCRIPT_STRING ("imagemask ");
  POSTSCRIPT_STRING ("setmatrix ");
  POSTSCRIPT_STRING ("} def ");
  ps_flush_PostScript ();
}

/*F+ void ComponentDisplay::removePSProc (char *name)
*
*    PURPOSE:
*
*    Removes the named PostScript procedure from the the NeWS server.
*
*    PARAMETERS:
*
*    name - Name of the PostScript procedure to remove.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::removePSProc (char *name)
{
  char *temp_string = new char [strlen (name) + 11];

  // Specify the named procedure as an empty definition
  sprintf (temp_string, " /%s {} def ", name);
  POSTSCRIPT_STRING (temp_string);
  ps_flush_PostScript ();

  delete temp_string;
}
#endif
#endif /* NO_XVPS */

/*F+ void ComponentDisplay::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Create the XView ComponentDisplay object.
*
*    PARAMETERS:
*
*    parent - The ComponentDisplay parent (a class derived from UIWindow)
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentDisplay::createObject (UIObject *parent)
{
  // If the drawing model is X11...
  if (model == X11) {
    // If this is an XView panel...
    if ((componentList.getSize () - numCanvasComponents > numScrollers) ||
        isPanel) {
      // ...and scrollable...
      if (numScrollers > 0)
	owObject = xv_create (parent->getXViewObject (), SCROLLABLE_PANEL,
			      CANVAS_X_PAINT_WINDOW,     TRUE,
			      PANEL_PAINT,		 PANEL_CLEAR,
			      WIN_DYNAMIC_VISUAL,	 dynamic,
			      WIN_BIT_GRAVITY,		 bitGravity,
			      NULL);
      // ...and not scrollable...
      else
	owObject = xv_create (parent->getXViewObject (), PANEL,
			      CANVAS_X_PAINT_WINDOW,     TRUE,
			      PANEL_PAINT,		 PANEL_CLEAR,
			      WIN_DYNAMIC_VISUAL,	 dynamic,
			      WIN_BIT_GRAVITY,		 bitGravity,
			      NULL);

      isPanel = TRUE;
    }
    // Else, this is an XView canvas...
    else
      owObject = xv_create (parent->getXViewObject (), CANVAS,
			    CANVAS_X_PAINT_WINDOW,     TRUE,
			    WIN_DYNAMIC_VISUAL,	       dynamic,
			    WIN_BIT_GRAVITY,	       bitGravity,
			    NULL);
  }
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
  // Else, if the drawing model is PostScript...
  else if (model == POSTSCRIPT) {
    // If this is an XView panel...
    if ((componentList.getSize () - numCanvasComponents > numScrollers) ||
        isPanel) {
      // ...and scrollable...
      if (numScrollers > 0)
	owObject = xv_create (parent->getXViewObject (), SCROLLABLE_PANEL,
			      XV_VISUAL_CLASS,           StaticColor,
			      PANEL_PAINT,		 PANEL_CLEAR,
			      WIN_BIT_GRAVITY,	         bitGravity,
			      NULL);
      // ...and not scrollable...
      else
	owObject = xv_create (parent->getXViewObject (), PANEL,
			      XV_VISUAL_CLASS,           StaticColor,
			      PANEL_PAINT,		 PANEL_CLEAR,
			      WIN_BIT_GRAVITY,	         bitGravity,
			      NULL);

      isPanel = TRUE;
    }
    // Else, this is an XView canvas...
    else
      owObject = xv_create (parent->getXViewObject (), PSCANVAS,
			    XV_VISUAL_CLASS,           StaticColor,
			    NULL);
  }
#endif
#endif
  // Else the drawing model is XView
  else {
    // If this is an XView panel...
    if ((componentList.getSize () - numCanvasComponents > numScrollers) ||
 	isPanel) {
      // ...and scrollable...
      if (numScrollers > 0)
	owObject = xv_create (parent->getXViewObject (), SCROLLABLE_PANEL,
			      PANEL_PAINT,		 PANEL_CLEAR,
			      WIN_DYNAMIC_VISUAL,	 dynamic,
			      NULL);
      // ...and not scrollable...
      else
	owObject = xv_create (parent->getXViewObject (), PANEL,
			      PANEL_PAINT,		 PANEL_CLEAR,
			      WIN_DYNAMIC_VISUAL,	 dynamic,
			      NULL);

      isPanel = TRUE;
    }
    // Else, this is an XView canvas...
    else
      owObject = xv_create (parent->getXViewObject (), CANVAS,
			    WIN_DYNAMIC_VISUAL,	dynamic,
			    NULL);
  } 

  if (resourceName) xv_set (owObject, XV_INSTANCE_NAME, resourceName, NULL);

  // Store the "this" pointer and the XView pointer of the parent
  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  xv_set (owObject,
	  XV_KEY_DATA, UIC_FRAME_KEY, parent->getXViewObject (),
	  NULL);

  /* If this is a panel and not scrollable or it is using the PostScript
     drawing model, then there is no canvas paint window... */
  if ((isPanel && (numScrollers == 0)) || (model == POSTSCRIPT))
    paintWindow = owObject;
  /* If it is a scrollable panel or a canvas using the X11 or XView
     drawing models, it has a canvas paint window... */
  else {
    paintWindow = canvas_paint_window (owObject);

    // Store the "this" pointer in the XView paint window
    xv_set (paintWindow, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  }

  // Get the X display pointer and the XID (Window)
  canvasXDisplay = (Display *) xv_get (paintWindow, XV_DISPLAY);
  canvasXWindow = (Window) xv_get (paintWindow, XV_XID);

#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
  // If the drawing model is PostScript...
  if (model == POSTSCRIPT)
    // Get the PSCANVAS NeWStoken...
    canvasPSWindow = (NeWStoken) xv_get (owObject, PSCANVAS_NEWSTOKEN);
#endif
#endif

  // If a paint color map was specified...
  if (paintColorMap) {
    // Create the XView color map and set the component display color map
    ((UIObject *) paintColorMap)->createObject (this);

    xv_set (owObject, WIN_CMS, paintColorMap->getXViewObject (), NULL);

    if (owObject != paintWindow)
      xv_set (paintWindow, WIN_CMS, paintColorMap->getXViewObject (), NULL);
  }

  if (foregroundColor != UIC_NOT_SET)
    xv_set (owObject, WIN_FOREGROUND_COLOR, foregroundColor, NULL);
  if (backgroundColor != UIC_NOT_SET)
    xv_set (owObject, WIN_BACKGROUND_COLOR, backgroundColor, NULL);

  if (owObject != paintWindow) {
    if (foregroundColor != UIC_NOT_SET)
      xv_set (paintWindow, WIN_FOREGROUND_COLOR, foregroundColor, NULL);
    if (backgroundColor != UIC_NOT_SET)
      xv_set (paintWindow, WIN_BACKGROUND_COLOR, backgroundColor, NULL);
  }

  // Set the XView help data
  setHelpData ();
  
  // If this is a canvas or a scrollable panel...
  if (!isPanel || (numScrollers > 0)) {
    // If paint window width and height are no set...
    if ((paintWidth != UIC_NOT_SET) || (paintHeight != UIC_NOT_SET))
      /* Do not let the canvas or panel automatically shrink or expand
	 by default... */
      xv_set (owObject,
	      CANVAS_AUTO_SHRINK, FALSE,
	      CANVAS_AUTO_EXPAND, FALSE,
	      NULL);
    
    if (paintWidth != UIC_NOT_SET) {
      if (resourceName)
        xv_set (owObject, XV_USE_DB, CANVAS_WIDTH, paintWidth, NULL, NULL);
      else
        xv_set (owObject, CANVAS_WIDTH, paintWidth, NULL);
    }
    
    if (paintHeight != UIC_NOT_SET) {
      if (resourceName)
        xv_set (owObject, XV_USE_DB, CANVAS_HEIGHT, paintHeight, NULL, NULL);
      else
        xv_set (owObject, CANVAS_HEIGHT, paintHeight, NULL);
    }
    
    if (autoExpand != UIC_NOT_SET)
      xv_set (owObject, CANVAS_AUTO_EXPAND, autoExpand, NULL);
  }
  
  // If this is a regular panel...
  if (isPanel && (numScrollers == 0))
    xv_set (owObject, WIN_BORDER, border, NULL);
  
  // If a font was specified...
  if (font) {
    // Create the XView font and set the font
    ((UIObject *) font)->createObject (parent);
    xv_set (paintWindow, XV_FONT, font->getXViewObject (), NULL);
  }
  
  // If a menu was specified...
  if (displayMenu) {
    // Create the XView menu
    ((UIObject *) displayMenu)->createObject (this);
    
    // If this is a panel...
    if (isPanel) {
      // Install a background event proc
      xv_set (owObject,
	      PANEL_BACKGROUND_PROC, &ComponentDisplay::backgroundEventProc,
	      NULL);

      // Set the menu on the panel object
      xv_set (owObject, WIN_MENU, displayMenu->getXViewObject (), NULL);
    }
    // Else this is a canvas...
    else
      // Set the menu on the paint window
      xv_set (paintWindow, WIN_MENU, displayMenu->getXViewObject (), NULL);
  }
  
  // If this is a panel, set the layout direction...
  if (isPanel) {
    if (layoutDirection == HORIZONTAL)
      xv_set (owObject, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);
    else if (layoutDirection == VERTICAL)
      xv_set (owObject, PANEL_LAYOUT, PANEL_VERTICAL, NULL);
  }
  
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
  
  /* If fit width has not been specified, allow extend to edge or
     exact specification of width... */
  if (!fitWidth) {
    if (extendWidth)
      xv_set (owObject, XV_WIDTH, WIN_EXTEND_TO_EDGE, NULL);
    else if (width != UIC_NOT_SET) {
      if (resourceName)
        xv_set (owObject, XV_USE_DB, XV_WIDTH, width, NULL, NULL);
      else
        xv_set (owObject, XV_WIDTH, width, NULL);
    }
  }
  
  /* If fit height has not been specified, allow extend to edge or
     exact specification of height... */
  if (!fitHeight) {
    if (extendHeight)
      xv_set (owObject, XV_HEIGHT, WIN_EXTEND_TO_EDGE, NULL);
    else if (height != UIC_NOT_SET) {
      if (resourceName)
        xv_set (owObject, XV_USE_DB, XV_HEIGHT, height, NULL, NULL);
      else
        xv_set (owObject, XV_HEIGHT, height, NULL);
    }
  }

  // XView prefers to have the show set before placing components on the panel
  if (!showFlag) xv_set (owObject, XV_SHOW, FALSE, NULL);

  int num_components = componentList.getSize ();

  // Loop through the list of components that have been added to the display...
  for (int i = 0; i < num_components; i++) {
    // Create the XView component object
    ((UIObject *) componentList [i])->createObject (this);
    
    // Get the XView pointer
    UICGeneric object = ((UIObject *) componentList [i])->getXViewObject ();
    
    // If the XView object was created...
    if (object)
      // Store the XView frame pointer of the parent window as key data
      xv_set (object,
	      XV_KEY_DATA, UIC_FRAME_KEY, parent->getXViewObject (),
	      NULL);
  }

  if (defaultButton) {
    if (!defaultButton->getXViewObject ()) {
      ((UIObject *) defaultButton)->createObject (this);

      UICGeneric object = defaultButton->getXViewObject ();

      if (object)
        xv_set (object,
                XV_KEY_DATA, UIC_FRAME_KEY, parent->getXViewObject (),
                NULL);

      componentList.addItem ((Generic *) defaultButton);
    }

    xv_set (owObject,
	    PANEL_DEFAULT_ITEM, defaultButton->getXViewObject (),
            NULL);
  }

  if (focusComponent) {
    if (!focusComponent->getXViewObject ()) {
      focusComponent->createObject (this);

      UICGeneric object = focusComponent->getXViewObject ();

      if (object)
        xv_set (object,
                XV_KEY_DATA, UIC_FRAME_KEY, parent->getXViewObject (),
                NULL);

      componentList.addItem ((Generic *) focusComponent);
    } 

    xv_set (owObject,
            PANEL_CARET_ITEM, focusComponent->getXViewObject (),
            NULL);
  }
  
  if (fitWidth)
    window_fit_width (owObject);
  
  if (fitHeight)
    window_fit_height (owObject);
  
  /* If an event handler was specified or there are user defined components
     and the handler was not already set (through a ComponentGroup being
     added to the ComponentDisplay)... */
  if (!eventHandlerSet &&
      (eventHandler ||
       useDefaultEventHandlerFlag ||
       eventTargets.getSize () ||
       (componentHotRegions.getSize () > 0) ||
       (acceptDrops && (dropHandler || useDefaultDropHandlerFlag)) ||
       (displayMenu && !isPanel) ||
       draggable || dropComponents || hotRegions.getSize ())) {
    // Set the XView event proc
    if (!isPanel) {
      xv_set (owObject, WIN_EVENT_PROC, &ComponentDisplay::eventProc, NULL);
      xv_set (paintWindow, WIN_EVENT_PROC, &ComponentDisplay::eventProc, NULL);
    }
    else {
      notify_interpose_event_func (owObject,
                                   (Notify_func)
				   &ComponentDisplay::panelEventProc,
                                   NOTIFY_SAFE);
      notify_interpose_event_func (owObject,
                                   (Notify_func)
				   &ComponentDisplay::panelEventProc,
                                   NOTIFY_IMMEDIATE);
    }
    
    // Set user specified events to be consumed
    int num_events = events.getSize (); 
    for (int i = 0; i < num_events; i++) {
      xv_set (paintWindow, WIN_CONSUME_EVENT, (int) events [i], NULL);

      if (!isPanel)
	xv_set (owObject, WIN_CONSUME_EVENT, (int) events [i], NULL);
    }
    
    eventHandlerSet = TRUE;
  }
  
  /* If a repaint handler was specified or there are user defined components
     and the handler was not already set (through a ComponentGroup being
     added to the ComponentDisplay)... */
  if (!repaintHandlerSet &&
      (repaintHandler ||
       useDefaultRepaintHandlerFlag ||
       (repaintComponents.getSize () > 0))) {
    // If using the X11 drawing model...
    if (model == X11) {
      // If this is an XView panel...
      if (isPanel)
	xv_set (owObject,
	        PANEL_REPAINT_PROC, &ComponentDisplay::repaintXProc,
	        NULL);
      // Else, this is an XView canvas
      else
	xv_set (owObject,
	        CANVAS_REPAINT_PROC, &ComponentDisplay::repaintXProc,
	        NULL);
    }
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
    // Else, if using the PostScript drawing model...
    else if (model == POSTSCRIPT) {
      // If this is an XView panel...
      if (isPanel)
	xv_set (owObject,
	        PANEL_REPAINT_PROC, &ComponentDisplay::repaintProc,
		NULL);
      // Else, this is an XView canvas
      else
	xv_set (owObject,
		PSCANVAS_REPAINT_PROC, &ComponentDisplay::repaintPSProc,
		NULL);
    }
#endif
#endif
    // Else, this is the XView drawing model...
    else {
      // If this is an XView panel...
      if (isPanel)
	xv_set (owObject,
		PANEL_REPAINT_PROC, &ComponentDisplay::repaintProc,
		NULL);
      // Else, this is an XView canvas
      else
	xv_set (owObject,
		CANVAS_REPAINT_PROC, &ComponentDisplay::repaintProc,
		NULL);
    }
    
    repaintHandlerSet = TRUE;
  }

  xv_set (owObject, CANVAS_RESIZE_PROC, &ComponentDisplay::resizeProc, NULL);

  setUIDisplayAttributes (parent);
}

/*F+ void ComponentDisplay::createDragSite ()
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
void ComponentDisplay::createDragSite ()
{
  if (!dragSite) createXViewDragSite (paintWindow);
}

/*F+ void ComponentDisplay::createDropSite ()
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
void ComponentDisplay::createDropSite ()
{
  Rect *rectangle;

  if (!dropSite) {
    if ((dropRectangle.r_width > 0) && (dropRectangle.r_height > 0))
      rectangle = &dropRectangle;
    else {
      rectangle = (Rect *) xv_get (paintWindow, XV_RECT);

      // Set the X and Y of the rectangle to the origin of the ComponentDisplay
      rectangle->r_left = 0;
      rectangle->r_top = 0;
    }

    createXViewDropSite (paintWindow, rectangle);
  }
}

/*F+ void ComponentDisplay::createHotRegionDragSite (HotRegion &hot_region)
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
void ComponentDisplay::createHotRegionDragSite (HotRegion &hot_region)
{
  if (!hot_region.dragSite) hot_region.createXViewDragSite (paintWindow);
}
 
/*F+ void ComponentDisplay::createHotRegionDropSite (HotRegion &hot_region)
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
void ComponentDisplay::createHotRegionDropSite (HotRegion &hot_region)
{
  Rect rectangle;

  if (!hot_region.dropSite) {
    if ((dropRectangle.r_width > 0) && (dropRectangle.r_height > 0)) {
      rectangle = dropRectangle;
    }
    else {
      int rect_x, rect_y, rect_w, rect_h;

      hot_region.getDefaultRectangle (rect_x, rect_y, rect_w, rect_h);
      rect_construct (&rectangle, rect_x, rect_y, rect_w, rect_h);
    }

    hot_region.createXViewDropSite (paintWindow, &rectangle);
  }
}

#ifdef TEST
#endif /* TEST */
