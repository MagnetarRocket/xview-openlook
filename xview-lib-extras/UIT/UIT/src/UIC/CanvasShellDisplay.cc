/*H+ CanvasShellDisplay.cc
*
*    SCCS ID: @(#)CanvasShellDisplay.cc 1.3 93/06/01 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines member function of the CanvasShellDisplay class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)CanvasShellDisplay.cc 1.3 93/06/01 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CharacterFont.h"
#include "CanvasShellDisplay.h"

// External Functions
extern "C" {
void canvas_shell_event_proc (Xv_window, Event *, Notify_arg);
}

/*F+ CanvasShellDisplay::CanvasShellDisplay ()
*
*    PURPOSE:
*
*    CanvasShellDisplay constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CanvasShellDisplay::CanvasShellDisplay ()
{
  setCanvasShellDisplayDefaults ();
}

/*F+ void CanvasShellDisplay::setCanvasShellDisplayDefaults ()
*
*    PURPOSE:
*
*    Initializes various CanvasShellDisplay data members.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CanvasShellDisplay::setCanvasShellDisplayDefaults ()
{
  componentHotRegion = FALSE;
  dropComponents = FALSE;
  numScrollers = 0;
  numCanvasComponents = 0;
  model = XVIEW;
  dynamic = FALSE;
  bufferRepaint = FALSE;
  delayRepaint = FALSE;
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
  canvasXRectlist = (Xv_xrectlist *) NULL;
  canvasRectlist = (Rectlist *) NULL;
  useDefaultRepaintHandlerFlag = FALSE;
  useDefaultResizeHandlerFlag = FALSE;
}

/*F+ CanvasShellDisplay::~CanvasShellDisplay ()
*
*    PURPOSE:
*
*    CanvasShellDisplay destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CanvasShellDisplay::~CanvasShellDisplay ()
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

/*F+ void CanvasShellDisplay::setForegroundColor (Color color)
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
void CanvasShellDisplay::setForegroundColor (Color color)
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
  }
}

/*F+ void CanvasShellDisplay::setForegroundColor (char *color)
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
void CanvasShellDisplay::setForegroundColor (char *color)
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
  }
}

/*F+ void CanvasShellDisplay::setBackgroundColor (Color color)
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
void CanvasShellDisplay::setBackgroundColor (Color color)
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
  }
}

/*F+ void CanvasShellDisplay::setBackgroundColor (char *color)
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
void CanvasShellDisplay::setBackgroundColor (char *color)
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
  }
}

/*F+ void CanvasShellDisplay::setMenu (UIMenu &menu)
*
*    PURPOSE:
*
*    Specifies the CanvasShellDisplay menu.
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
void CanvasShellDisplay::setMenu (UIMenu &menu)
{
  // Save the menu pointer
  displayMenu = &menu;

  // If the XView object has been created...
  if (owObject) {
    // If an old menu exists, delete it
    Menu old_menu = (Menu) xv_get (paintWindow, WIN_MENU);
    if (old_menu) xv_destroy_safe (old_menu);

    // Specify the new XView menu as the panel menu
    xv_set (paintWindow, WIN_MENU, displayMenu->getXViewObject (), NULL);

    // If no event handler has been set, set one to catch the menu
    // button presses
    if (!eventHandlerSet) {
      xv_set (paintWindow,
              WIN_EVENT_PROC, &CanvasShellDisplay::eventProc,
              NULL);
    }
  }
}

/*F+ UIMenu *CanvasShellDisplay::getMenu ()
*
*    PURPOSE:
*
*    Returns a pointer to the current CanvasShellDisplay menu.
*
*    RETURN VALUES:
*
*    Returns the pointer to the last set canvas shell display menu.
*    Returns NULL if no menu is set.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIMenu *CanvasShellDisplay::getMenu ()
{
  return displayMenu;
}

/*F+ void CanvasShellDisplay::setRepaintHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Specifies the CanvasShellDisplay repaint handler.
*
*    PARAMETERS:
*
*    handler - The function to call to repaint the canvas shell display.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CanvasShellDisplay::setRepaintHandler (UICHandlerFunction handler)
{
  repaintHandler = handler;

  /* If the XView object already exists and the repaint handler was
     never set before... */
  if (owObject && !repaintHandlerSet) {
    // If using the X11 drawing model...
    if (model == X11) {
      xv_set (owObject,
	      CANVAS_REPAINT_PROC, &CanvasShellDisplay::repaintXProc,
	      NULL);
    }
    // Else, this is the XView drawing model...
    else {
      xv_set (owObject,
      	      CANVAS_REPAINT_PROC, &CanvasShellDisplay::repaintProc,
	      NULL);
    }
    
    repaintHandlerSet = TRUE;
  }
}

/*F+ void CanvasShellDisplay::setResizeHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Specifies the CanvasShellDisplay resize handler.
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
void CanvasShellDisplay::setResizeHandler (UICHandlerFunction handler)
{
  resizeHandler = handler;
}

/*F+ void CanvasShellDisplay::setFont (CharacterFont &char_font)
*
*    PURPOSE:
*
*    Specifies the font to be used with this object.
*
*    PARAMETERS: 
*
*    char_font - The UIC font to use.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void CanvasShellDisplay::setFont (CharacterFont &char_font)
{
  UIObject::setFont (char_font);

  // If the XView object already exists...
  if (owObject) {
    // Create the XView font object and set it
    ((UIObject *) font)->createObject (this);
    xv_set (owObject, CANVAS_SHELL_FONT, font->getXViewObject (), NULL);
  }
}
 
/*F+ void CanvasShellDisplay::repaintProc (Canvas     canvas,
*                                        Xv_window  xview_window,
*					 Rectlist  *rectlist)
*
*    PURPOSE:
*
*    The XView drawing model repaint proc for the CanvasShellDisplay.
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
void CanvasShellDisplay::repaintProc (Canvas     canvas,
				      Xv_window  xview_window,
				      Rectlist  *rectlist)
{
  CanvasShellDisplay *object = (CanvasShellDisplay *) xv_get (canvas,
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
}

/*F+ void CanvasShellDisplay::repaintXProc (Canvas       canvas,
*                                           Xv_window    xview_window,
*                                           Display     *display,
*                                           Window       window,
*					    Xv_rectlist *rectlist)
*
*    PURPOSE:
*
*    The X11 drawing model repaint proc for the CanvasShellDisplay.
*    This is called by XView and is used to call the user specified
*    repaint handler.  This member function is protected.
*
*    PARAMETERS:
*
*    canvas       - The XView canvas or panel to repaint
*    xview_window - The paint window of the canvas or panel.
*    display      - The display pointer for the canvas shell display.
*    window       - The XID or Window ID of the canvas shell display.
*    rectlist     - The clipping rectangles of the area that needs repainting.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CanvasShellDisplay::repaintXProc (Canvas        canvas,
				       Xv_window     xview_window,
				       Display      *display,
				       Window        window,
				       Xv_xrectlist *rectlist)
{
  CanvasShellDisplay *object = (CanvasShellDisplay *) xv_get (canvas,
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
}

/*F+ void CanvasShellDisplay::resizeProc (Canvas canvas, int width, int height)
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
void CanvasShellDisplay::resizeProc (Canvas canvas, int width, int height)
{
  CanvasShellDisplay *object = (CanvasShellDisplay *) xv_get (canvas,
                                                              XV_KEY_DATA,
                                                              UIC_OBJECT_KEY);

  // Call the user specified handler
  if (object->useDefaultResizeHandlerFlag)
    object->defaultResizeHandler ();
  else if (object->resizeHandler)
    (*object->resizeHandler) (object);
}

/*F+ void CanvasShellDisplay::eventProc (Xv_window     win,
*                                        Event        *event,
*                                        Canvas_shell  cs)
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
void CanvasShellDisplay::eventProc (Xv_window     win,
                                    Event        *event,
				    Canvas_shell  cs)
{
  CanvasShellDisplay *object = (CanvasShellDisplay *) xv_get (win,
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
    HotRegion   *hot_region = ((CSComponentHotRegion *)
		               object->componentHotRegions [i])->hotRegion;
    UIComponent *component = ((CSComponentHotRegion *)
		              object->componentHotRegions [i])->component;
    Boolean background =
	    ((CSComponentHotRegion *)
	     object->componentHotRegions [i])->trapBackgroundEvents;

    // If this event is in a component hot region...
    if (hot_region->detectHotRegionEvent (object, event)) {
      object->componentHotRegion = TRUE;

      // Call the user defined component hot region event proc
      component->componentHotRegionEventProc ((ComponentDisplay *) object);

      // If this component is not event transparent...
      if (!hot_region->isTransparent ())
	break;
    }
    /* Else, if background events (any event not on the component,
       but on the panel) events are to be detected for the user defined
       component... */
    else if (background)
      component->componentBackgroundEventProc ((ComponentDisplay *) object);
  }

  /* If this is an XView canvas and not on a user defined component
     and the mouse menu button is down... */ 
  if (!object->componentHotRegion &&
      (event_action (event) == ACTION_MENU) && event_is_down (event)) {
    // If a menu is specified, show it...
    Menu menu = (Menu) xv_get (win, WIN_MENU);
    if (menu) menu_show (menu, win, event, NULL);
  }

  // Erase the UIC event information
  object->resetEventInformation ();

  canvas_shell_event_proc (win, event, (Notify_arg) cs);
}

/*F+ void CanvasShellDisplay::addComponent (UIComponent &component)
*
*    PURPOSE:
*
*    Adds the specified component to the canvas shell display.
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
void CanvasShellDisplay::addComponent (UIComponent &component)
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

  // If the XView canvas shell display object has already been created...
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
      xv_set (paintWindow, 
	      WIN_EVENT_PROC, &CanvasShellDisplay::eventProc,
	      NULL);
      
      // Set the user specified events to be consumed
      int num_events = events.getSize (); 
      for (int i = 0; i < num_events; i++) {
        xv_set (owObject, WIN_CONSUME_EVENT, (int) events [i], NULL);
        xv_set (paintWindow, WIN_CONSUME_EVENT, (int) events [i], NULL);
      }
      
      eventHandlerSet = TRUE;
    }
    
    /* If a repaint handler has not been set and a user defined component
       has been added... */
    if (!repaintHandlerSet && (repaintComponents.getSize () > 0)) {
      // If using the X11 drawing model...
      if (model == X11) {
	xv_set (owObject,
		CANVAS_REPAINT_PROC, &CanvasShellDisplay::repaintXProc,
		NULL);
      }
      // Else, this is the XView drawing model...
      else {
	xv_set (owObject,
		CANVAS_REPAINT_PROC, &CanvasShellDisplay::repaintProc,
		NULL);
      }
    
      repaintHandlerSet = TRUE;
    }
  }
}

/*F+ void CanvasShellDisplay::addComponent (CSComponent &component)
*
*    PURPOSE:
*
*    Adds the specified component to the canvas shell display.
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
void CanvasShellDisplay::addComponent (CSComponent &component)
{
  // Add the component to the list of components
  componentList.addItem ((Generic *) (&component));
  numCanvasComponents++;
  
  // See if this component accepts drops
  if (component.acceptDrops)
    dropComponents = TRUE;

  // If the XView canvas shell display object has already been created...
  if (owObject) {
    // Let the CS component know who its canvas shell is
    ((CSComponent *) &component)->canvasShellDisplay = this;

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
      xv_set (paintWindow, 
	      WIN_EVENT_PROC, &CanvasShellDisplay::eventProc,
	      NULL);
      
      // Set the user specified events to be consumed
      int num_events = events.getSize (); 
      for (int i = 0; i < num_events; i++) {
        xv_set (owObject, WIN_CONSUME_EVENT, (int) events [i], NULL);
        xv_set (paintWindow, WIN_CONSUME_EVENT, (int) events [i], NULL);
      }
      
      eventHandlerSet = TRUE;
    }
    
    /* If a repaint handler has not been set and a user defined component
       has been added... */
    if (!repaintHandlerSet && (repaintComponents.getSize () > 0)) {
      // If using the X11 drawing model...
      if (model == X11) {
	xv_set (owObject,
		CANVAS_REPAINT_PROC, &CanvasShellDisplay::repaintXProc,
		NULL);
      }
      // Else, this is the XView drawing model...
      else {
	xv_set (owObject,
		CANVAS_REPAINT_PROC, &CanvasShellDisplay::repaintProc,
		NULL);
      }
    
      repaintHandlerSet = TRUE;
    }
  }
}

/*F+ void CanvasShellDisplay::removeComponent (UIComponent &component)
*
*    PURPOSE:
*
*    Removes the specified component from the canvas shell display.  The
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
void CanvasShellDisplay::removeComponent (UIComponent &component)
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
    if (((CSComponentHotRegion *)
         componentHotRegions [i])->component == &component) {
      (void) componentHotRegions.removeItem (i); 
      found = TRUE;
      break; 
    } 
  } 

  if (found) component.show (FALSE);
}
 
/*F+ void CanvasShellDisplay::removeComponent (CSComponent &component)
*
*    PURPOSE:
*
*    Removes the specified component from the canvas shell display.  The
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
void CanvasShellDisplay::removeComponent (CSComponent &component)
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
 
  if (found) component.show (FALSE);
}
 
/*F+ void CanvasShellDisplay::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Create the XView CanvasShellDisplay object.
*
*    PARAMETERS:
*
*    parent - The CanvasShellDisplay parent (a class derived from UIWindow)
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CanvasShellDisplay::createObject (UIObject *parent)
{
  // If the drawing model is X11...
  if (model == X11) {
    owObject = xv_create (parent->getXViewObject (), CANVAS_SHELL,
			  CANVAS_X_PAINT_WINDOW,     TRUE,
			  WIN_DYNAMIC_VISUAL,	     dynamic,
			  WIN_BIT_GRAVITY,	     bitGravity,
			  NULL);
  }
  // Else the drawing model is XView
  else {
    owObject = xv_create (parent->getXViewObject (), CANVAS_SHELL,
			  WIN_DYNAMIC_VISUAL,	dynamic,
			  NULL);
  } 

  if (resourceName) xv_set (owObject, XV_INSTANCE_NAME, resourceName, NULL);

  // Store the "this" pointer and the XView pointer of the parent
  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  xv_set (owObject,
	  XV_KEY_DATA, UIC_FRAME_KEY, parent->getXViewObject (),
	  NULL);

  paintWindow = canvas_paint_window (owObject);
  xv_set (paintWindow, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  // Get the X display pointer and the XID (Window)
  canvasXDisplay = (Display *) xv_get (paintWindow, XV_DISPLAY);
  canvasXWindow = (Window) xv_get (paintWindow, XV_XID);

  // If a paint color map was specified...
  if (paintColorMap) {
    // Create the XView color map and set the canvas shell display color map
    ((UIObject *) paintColorMap)->createObject (this);

    xv_set (owObject, WIN_CMS, paintColorMap->getXViewObject (), NULL);
  }

  if (foregroundColor != UIC_NOT_SET)
    xv_set (owObject, WIN_FOREGROUND_COLOR, foregroundColor, NULL);
  if (backgroundColor != UIC_NOT_SET)
    xv_set (owObject, WIN_BACKGROUND_COLOR, backgroundColor, NULL);

  // Set the XView help data
  setHelpData ();
  
  // If paint window width and height are no set...
  if ((paintWidth != UIC_NOT_SET) || (paintHeight != UIC_NOT_SET))
    /* Do not let the canvas or panel automatically shrink or expand
       by default... */
    xv_set (owObject,
	    CANVAS_AUTO_SHRINK, FALSE,
	    CANVAS_AUTO_EXPAND, FALSE,
	    NULL);
    
  if (paintWidth != UIC_NOT_SET)
    xv_set (owObject, CANVAS_MIN_PAINT_WIDTH, paintWidth, NULL);
    
  if (paintHeight != UIC_NOT_SET)
    xv_set (owObject, CANVAS_MIN_PAINT_HEIGHT, paintHeight, NULL);
    
  if (autoExpand != UIC_NOT_SET)
    xv_set (owObject, CANVAS_AUTO_EXPAND, autoExpand, NULL);
  
  // If a font was specified...
  if (font) {
    // Create the XView font and set the font
    ((UIObject *) font)->createObject (parent);
    xv_set (owObject, CANVAS_SHELL_FONT, font->getXViewObject (), NULL);
  }
  
  // If a menu was specified...
  if (displayMenu) {
    // Create the XView menu
    ((UIObject *) displayMenu)->createObject (this);

    // Set the menu on the paint window
    xv_set (paintWindow, WIN_MENU, displayMenu->getXViewObject (), NULL);
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
    // Let the CS component know who its parent is
    if (!((UIObject *) componentList [i])->isUIComponent)
      ((CSComponent *) componentList [i])->canvasShellDisplay = this;

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
  
  if (fitWidth)
    window_fit_width (owObject);
  
  if (fitHeight)
    window_fit_height (owObject);
  
  /* If an event handler was specified or there are user defined components
     and the handler was not already set (through a ComponentGroup being
     added to the CanvasShellDisplay)... */
  if (!eventHandlerSet &&
      (eventHandler ||
       useDefaultEventHandlerFlag ||
       eventTargets.getSize () ||
       (componentHotRegions.getSize () > 0) ||
       (acceptDrops && (dropHandler || useDefaultDropHandlerFlag)) ||
       displayMenu) ||
       draggable || dropComponents || hotRegions.getSize ()) {
    // Set the XView event proc
    xv_set (paintWindow,
	    WIN_EVENT_PROC, &CanvasShellDisplay::eventProc,
            NULL);

    // Set user specified events to be consumed
    int num_events = events.getSize (); 
    for (int i = 0; i < num_events; i++) {
      xv_set (owObject, WIN_CONSUME_EVENT, (int) events [i], NULL);
    }
    
    eventHandlerSet = TRUE;
  }
  
  /* If a repaint handler was specified or there are user defined components
     and the handler was not already set (through a ComponentGroup being
     added to the CanvasShellDisplay)... */
  if (!repaintHandlerSet &&
      (repaintHandler ||
       useDefaultRepaintHandlerFlag ||
       (repaintComponents.getSize () > 0))) {
    // If using the X11 drawing model...
    if (model == X11) {
      xv_set (owObject,
	      CANVAS_REPAINT_PROC, &CanvasShellDisplay::repaintXProc,
	      NULL);
    }
    // Else, this is the XView drawing model...
    else {
      xv_set (owObject,
	      CANVAS_REPAINT_PROC, &CanvasShellDisplay::repaintProc,
	      NULL);
    }
    
    repaintHandlerSet = TRUE;
  }

  xv_set (owObject, CANVAS_RESIZE_PROC, &CanvasShellDisplay::resizeProc, NULL);

  if (bufferRepaint) xv_set (owObject, CANVAS_SHELL_BATCH_REPAINT, TRUE, NULL);
  if (delayRepaint) xv_set (owObject, CANVAS_SHELL_DELAY_REPAINT, TRUE, NULL);

  setUIDisplayAttributes (parent);
}

/*F+ void CanvasShellDisplay::createDragSite ()
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
void CanvasShellDisplay::createDragSite ()
{
  if (!dragSite) createXViewDragSite (owObject);
}

/*F+ void CanvasShellDisplay::createDropSite ()
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
void CanvasShellDisplay::createDropSite ()
{
  xv_set (owObject, CANVAS_SHELL_AUTO_DROP_SITE, TRUE, NULL);

  dropSite = xv_get (paintWindow, CANVAS_SHELL_DROP_SITE, NULL);
  if (dropSite) xv_destroy_safe (dropSite);

  dropSite = xv_create (paintWindow, DROP_SITE_ITEM,
                	DROP_SITE_ID,         uiObjectID,
                	DROP_SITE_REGION,     xv_get (paintWindow, XV_RECT),
                	DROP_SITE_EVENT_MASK, DND_ENTERLEAVE | DND_MOTION,
                	NULL);
  xv_set (paintWindow,
          CANVAS_SHELL_DROP_SITE, dropSite,
          NULL);

  xv_set (dropSite, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  dropSiteList->addItem ((Generic *) dropSite);
}

/*F+ void CanvasShellDisplay::createHotRegionDragSite (HotRegion &hot_region)
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
void CanvasShellDisplay::createHotRegionDragSite (HotRegion &hot_region)
{
  if (!hot_region.dragSite) hot_region.createXViewDragSite (paintWindow);
}
 
/*F+ void CanvasShellDisplay::createHotRegionDropSite (HotRegion &hot_region)
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
void CanvasShellDisplay::createHotRegionDropSite (HotRegion &hot_region)
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

#endif /* SLINGSHOT */
