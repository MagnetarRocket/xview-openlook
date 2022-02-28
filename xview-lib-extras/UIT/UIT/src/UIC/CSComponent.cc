/*H+ CSComponent.cc
*
*    SCCS ID: @(#)CSComponent.cc 1.5 93/06/16 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSComponent class.
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
static char sccsid [] = "@(#)CSComponent.cc 1.2 93/04/09 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "ColorMap.h"
#include "CanvasShellDisplay.h"
#include "CSComponent.h"

// External Function Declarations
extern "C" {
void rectobj_toggle_event_proc (Xv_window, Event *, Canvas_shell, Rectobj);
}

/*F+ CSComponent::CSComponent ()
*
*    PURPOSE:
*
*    Constructor of the CSComponent class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSComponent::CSComponent ()
{
  setCSComponentDefaults ();
}

/*F+ CSComponent::setCSComponentDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSComponent class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::setCSComponentDefaults ()
{
  componentMenu = (UIMenu *) NULL;
  absoluteX = UIC_NOT_SET;
  absoluteY = UIC_NOT_SET;
  border = UIC_NOT_SET;
  filled = FALSE;
  filledSet = FALSE;
  exclusive = FALSE;
  exclusiveSet = FALSE;
  geometrySilent = FALSE;
  geometrySilentSet = FALSE;
  minHeight = UIC_NOT_SET;
  minWidth = UIC_NOT_SET;
  selectable = TRUE;
  selectableSet = TRUE;
  selected = FALSE;
  selectionOwner = (Selection_owner) NULL;
  stackingPosition = UIC_NOT_SET;
  startDragX = UIC_NOT_SET;
  startDragY = UIC_NOT_SET;
  startDragAdjust = UIC_NOT_SET;
  toggleMode = FALSE;
  toggleModeSet = FALSE;
  toggleState = FALSE;
  toggleStateSet = FALSE;
  newChild = (CSComponent *) NULL;
  deletedChild = (CSComponent *) NULL;
  rectWithChild = (Rect *) NULL;
  newRect = (Rect *) NULL;
  oldRect = (Rect *) NULL;
  eventProcSet = FALSE;
  dropHandlerSet = FALSE;
  addChildHandler = (UICHandlerFunction) NULL;
  addChildHandlerSet = FALSE;
  deleteChildHandler = (UICHandlerFunction) NULL;
  deleteChildHandlerSet = FALSE;
  doubleClickHandler = (UICHandlerFunction) NULL;
  doubleClickHandlerSet = FALSE;
  geometryHandler = (UICHandlerFunction) NULL;
  geometryHandlerSet = FALSE;
  predefinedCSCallback = (PredefinedCSCallback) NULL;
  predefinedCSCallbackSet = FALSE;
  selectionHandler = (UICHandlerFunction) NULL;
  singleClickHandler = (UICHandlerFunction) NULL;
  singleClickHandlerSet = FALSE;
  isDrawObject = FALSE;
  canvasShellDisplay = (CanvasShellDisplay *) NULL;
}

/*F+ CSComponent::~CSComponent ()
*
*    PURPOSE:
*
*    Destructor of the CSComponent class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSComponent::~CSComponent ()
{
  if (!destroyChildren) {
    children.dropItems ();
  }
  else {
    if (owObject) {
      int num_children = getNumberOfChildren ();

      for (int i=1; i < num_children; i++) {
	CSComponent *child = getChild (i);
	child->setDestroyChildren (TRUE);
	delete child;
      }
    }
    else {
      CSComponent *child;

      Boolean reset_flag = TRUE;
      while (child = (CSComponent *) children.traverse (reset_flag)) {
        reset_flag = FALSE;
        child->setDestroyChildren (TRUE);
      }
      children.clear ();
    }
  }
}

/*F+ void CSComponent::addChild (CSComponent &child)
*
*    PURPOSE:
*
*    Adds a child component to this component.
*
*    PARAMETERS:
*
*    child - The child component
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::addChild (CSComponent &child)
{
  if (&child == this) return;

  if (owObject) {
    xv_set (owObject,
	    XV_KEY_DATA, UIC_CHILD_KEY, (UIObject *) &child,
	    NULL);

    ((UIObject *) &child)->createObject (this);

    UICGeneric object = child.getXViewObject ();
    if (object)
      xv_set (object,
              XV_KEY_DATA, UIC_FRAME_KEY, (Frame) xv_get (owObject,
							  XV_KEY_DATA,
							  UIC_FRAME_KEY),
              NULL);
  }
  else if (children.findItem ((Generic *) &child) < 0) {
    children.addItem ((Generic *) &child);
  }
}

/*F+ void CSComponent::deleteChild (CSComponent &child)
*
*    PURPOSE:
*
*    Deletes a child component from this component.
*
*    PARAMETERS:
*
*    child - The child component
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::deleteChild (CSComponent &child)
{
  if (owObject) {
    xv_set (child.getXViewObject (), XV_OWNER, NULL, NULL);
  }
  else {
    children.removeItem ((Generic *) &child);
  }
}

/*F+ void CSComponent::setMenu (UIMenu &menu)
*
*    PURPOSE:
*
*    Sets the component menu.
*
*    PARAMETERS:
*
*    menu - The component menu.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::setMenu (UIMenu &menu)
{
  /* Save the component menu pointer */
  componentMenu = &menu;

  /* If the XView object has already been created... */
  if (owObject) {
    /* Get the current XView component menu and delete it if it exists */
    Menu old_menu = (Menu) xv_get (owObject, RECTOBJ_MENU);
    if (old_menu) xv_destroy_safe (old_menu);

    /* Create a new XView menu and set the component menu */
    ((UIObject *) componentMenu)->createObject (canvasShellDisplay);
    xv_set (owObject, RECTOBJ_MENU, componentMenu->getXViewObject (), NULL);
  }
}

/*F+ void CSComponent::setForegroundColor (Color color)
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
void CSComponent::setForegroundColor (Color color)
{
  if (!colorMap) {
    useDefaultColorMap ();
    if (owObject) initializeColorMap (serverParent);
  }
    
  foregroundColor = colorMap->findColor (color);
    
  if (owObject)
    xv_set (owObject, RECTOBJ_FG, foregroundColor, NULL);
}

/*F+ void CSComponent::setForegroundColor (char *color)
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
void CSComponent::setForegroundColor (char *color)
{
  if (!colorMap) {
    useDefaultColorMap ();
    if (owObject) initializeColorMap (serverParent);
  }
    
  foregroundColor = colorMap->findColor (color);
    
  if (owObject)
    xv_set (owObject, RECTOBJ_FG, foregroundColor, NULL);
}

/*F+ void CSComponent::setBackgroundColor (Color color)
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
void CSComponent::setBackgroundColor (Color color)
{
  if (!colorMap) {
    useDefaultColorMap ();
    if (owObject) initializeColorMap (serverParent);
  }
    
  backgroundColor = colorMap->findColor (color);
    
  if (owObject)
    xv_set (owObject, RECTOBJ_BG, backgroundColor, NULL);
}

/*F+ void CSComponent::setBackgroundColor (char *color)
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
void CSComponent::setBackgroundColor (char *color)
{
  if (!colorMap) {
    useDefaultColorMap ();
    if (owObject) initializeColorMap (serverParent);
  }
    
  backgroundColor = colorMap->findColor (color);
    
  if (owObject)
    xv_set (owObject, RECTOBJ_BG, backgroundColor, NULL);
}

/*F+ CSComponent * CSComponent::getChild (int index)
*
*    PURPOSE:
*
*    Gets the specified child of this object.
*
*    PARAMETERS:
*
*    index - The index of the child to get.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSComponent *CSComponent::getChild (int index)
{
  CSComponent *child = (CSComponent *) NULL;

  if (owObject) {
    UICGeneric xview_child = xv_get (owObject, RECTOBJ_NTH_CHILD, index-1);
    child = (CSComponent *) xv_get (xview_child, XV_KEY_DATA, UIC_OBJECT_KEY);
  }
  else {
    int numChildren = children.getSize ();

    if ((index > 0) || (index <= numChildren))
      child = (CSComponent *) children.getItem (index-1);
  }

  return child;
}

/*F+ void CSComponent::eventProc (Xv_window     paint_window,
*                                 Event        *event,
*                                 Canvas_shell  canvas_shell,
*                                 Rectobj       rectobj)
*
*    PURPOSE:
*
*    Internal XView event proc wrapper.  This is called whenever an
*    event occurs on a component.
*
*    PARAMETERS:
*
*    paint_window - The paint window the object is on
*    event - The event that occurred
*    canvas_shell - The canvas shell the object is on
*    rectobj - The object
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::eventProc (Xv_window     paint_window,
			     Event        *event,
			     Canvas_shell  canvas_shell,
			     Rectobj       rectobj)
{
  CSComponent *object = (CSComponent *) xv_get (rectobj,
					        XV_KEY_DATA,
						UIC_OBJECT_KEY);
  if (!object) return;

  object->setEventInformation (event);
  
  // Call the user specified event handler
  if (object->useDefaultEventHandlerFlag)
    object->defaultEventHandler ();
  else if (object->eventHandler)
    (*object->eventHandler) (object);
  
  // Call the predefined callback
  if (object->predefinedCSCallback)
    (*object->predefinedCSCallback) (paint_window,
				     event,
				     canvas_shell,
				     rectobj);

  if (object->toggleMode)
    rectobj_toggle_event_proc (paint_window,
			       event,
			       canvas_shell,
			       rectobj);

  /* If the menu button is down and there is a menu on this component... */
  if ((event_action (event) == ACTION_MENU) && event_is_down (event)) {
    // If a menu is specified, show it...
    Menu menu = (Menu) xv_get (rectobj, RECTOBJ_MENU);
    if (menu) menu_show (menu, paint_window, event, NULL);
  }
 
  object->resetEventInformation ();
}

/*F+ void CSComponent::addChildProc (Rectobj  parent,
*                                    Rectobj  child,
*                                    Rect    *rect)
*
*    PURPOSE:
*
*    Internal XView add child proc wrapper.  This is called whenever a
*    child is added to this object.
*
*    PARAMETERS:
*
*    parent - This object
*    child - The child added
*    rect - The rectangle boundary with the child added 
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::addChildProc (Rectobj  parent,
			        Rectobj  child,
			        Rect    *rect)
{
  CSComponent *object = (CSComponent *) xv_get (parent,
					        XV_KEY_DATA,
						UIC_OBJECT_KEY);
  if (!object) return;

  CSComponent *child_object = (CSComponent *) xv_get (parent,
						      XV_KEY_DATA,
						      UIC_CHILD_KEY);
  if (!child_object) return;

  if (object->addChildHandler) {
    object->newChild = child_object;
    object->rectWithChild = rect;

    (*object->addChildHandler) (object);

    object->newChild = (CSComponent *) NULL;
    object->rectWithChild = (Rect *) NULL;
  }
}

/*F+ void CSComponent::deleteChildProc (Rectobj  parent,
*                                       Rectobj  child,
*                                       Rect    *rect)
*
*    PURPOSE:
*
*    Internal XView deleted child proc wrapper.  This is called whenever a
*    child is deleted from this object.
*
*    PARAMETERS:
*
*    parent - This object
*    child - The child deleted
*    rect - The rectangle boundary before the child is deleted 
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::deleteChildProc (Rectobj  parent,
                                   Rectobj  child,
				   Rect    *rect)
{
  CSComponent *object = (CSComponent *) xv_get (parent,
					        XV_KEY_DATA,
						UIC_OBJECT_KEY);
  if (!object) return;

  CSComponent *child_object = (CSComponent *) xv_get (child,
						      XV_KEY_DATA,
						      UIC_OBJECT_KEY);
  if (!child_object) return;

  if (object->deleteChildHandler) {
    object->deletedChild = child_object;
    object->rectWithChild = rect;

    (*object->deleteChildHandler) (object);

    object->deletedChild = (CSComponent *) NULL;
    object->rectWithChild = (Rect *) NULL;
  }
}

/*F+ void CSComponent::doubleClickProc (Xv_window     paint_window,
*                                       Event        *event,
*                                       Canvas_shell  canvas_shell,
*                                       Rectobj       rectobj)
*
*    PURPOSE:
*
*    Internal XView double click proc wrapper.  This is called whenever an
*    double click occurs on a component.
*
*    PARAMETERS:
*
*    paint_window - The paint window the object is on
*    event - The event that occurred
*    canvas_shell - The canvas shell the object is on
*    rectobj - The object
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::doubleClickProc (Xv_window     paint_window,
                                   Event        *event,
				   Canvas_shell  canvas_shell,
				   Rectobj       rectobj)
{
  CSComponent *object = (CSComponent *) xv_get (rectobj,
					        XV_KEY_DATA,
						UIC_OBJECT_KEY);
  if (!object) return;

  object->setEventInformation (event);
  
  // Call the user specified double click handler
  if (object->doubleClickHandler)
    (*object->doubleClickHandler) (object);

  object->resetEventInformation ();
}

/*F+ void CSComponent::geometryProc (Rectobj  rectobj,
*                                    Rect    *new_rect,
*                                    Rect    *old_rect)
*
*    PURPOSE:
*
*    Internal XView geometry proc wrapper.  This is called whenever a
*    geometry in this object changes.
*
*    PARAMETERS:
*
*    rectobj - This object
*    new_rect - The rectangle boundary after the geometry changed 
*    old_rect - The rectangle boundary before the geometry changed 
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::geometryProc (Rectobj  rectobj,
                                Rect    *new_rect,
				Rect    *old_rect)
{
  CSComponent *object = (CSComponent *) xv_get (rectobj,
					        XV_KEY_DATA,
						UIC_OBJECT_KEY);
  if (!object) return;

  if (object->geometryHandler) {
    object->newRect = new_rect;
    object->oldRect = old_rect;

    (*object->geometryHandler) (object);

    object->newRect = (Rect *) NULL;
    object->oldRect = (Rect *) NULL;
  }
}

/*F+ void CSComponent::selectionProc (Rectobj  rectobj,
*                                     int      selected,
*                                     Event   *event);
*
*    PURPOSE:
*
*    Internal XView selection proc wrapper.  This is called whenever an
*    selection or deselection occurs on a component.
*
*    PARAMETERS:
*
*    rectobj - The object
*    selected - TRUE if the object was selected
*    event - The event that occurred
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::selectionProc (Rectobj  rectobj,
                                 int      selected,
				 Event   *event)
{
  CSComponent *object = (CSComponent *) xv_get (rectobj,
					        XV_KEY_DATA,
						UIC_OBJECT_KEY);
  if (!object) return;

  object->setEventInformation (event);
  object->selected = (selected ? TRUE : FALSE);
  
  // Call the user specified single click handler
  if (object->selectionHandler)
    (*object->selectionHandler) (object);

  object->resetEventInformation ();
}

/*F+ void CSComponent::singleClickProc (Xv_window     paint_window,
*                                       Event        *event,
*                                       Canvas_shell  canvas_shell,
*                                       Rectobj       rectobj,
*                                       int           state)
*
*    PURPOSE:
*
*    Internal XView single click proc wrapper.  This is called whenever an
*    single click occurs on a component.
*
*    PARAMETERS:
*
*    paint_window - The paint window the object is on
*    event - The event that occurred
*    canvas_shell - The canvas shell the object is on
*    rectobj - The object
*    state - The toggle state
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::singleClickProc (Xv_window     paint_window,
                                   Event        *event,
				   Canvas_shell  canvas_shell,
				   Rectobj       rectobj,
				   int           state)
{
  CSComponent *object = (CSComponent *) xv_get (rectobj,
					        XV_KEY_DATA,
						UIC_OBJECT_KEY);
  if (!object) return;

  object->setEventInformation (event);
  
  // Call the user specified single click handler
  if (object->singleClickHandler)
    (*object->singleClickHandler) (object);

  object->resetEventInformation ();
}

/*F+ void CSComponent::dropProc (Xv_window     paint_window,
*                                Event        *event,
*                                Canvas_shell  canvas_shell,
*                                Rectobj       rectobj)
*
*    PURPOSE:
*
*    Internal XView drop proc wrapper.  This is called whenever an
*    drop occurs on a component.
*
*    PARAMETERS:
*
*    paint_window - The paint window the object is on
*    event - The event that occurred
*    canvas_shell - The canvas shell the object is on
*    rectobj - The object
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::dropProc (Xv_window     paint_window,
                            Event        *event,
			    Canvas_shell  canvas_shell,
			    Rectobj       rectobj)
{
  CSComponent *object = (CSComponent *) xv_get (rectobj,
					        XV_KEY_DATA,
						UIC_OBJECT_KEY);
  if (!object) return;

  object->setEventInformation (event);
  
  // Call the user specified drop handler
  if (object->dropHandler)
    (*object->dropHandler) (object);

  object->resetEventInformation ();
}

/*F+ void CSComponent::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSComponent object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), RECTOBJ, NULL);

  setCSComponentAttributes (parent);
}

/*F+ void CSComponent::setCSComponentAttributes (UIObject *parent)
*
*    PURPOSE:
*
*    Sets common CSComponent XView attributes that are required for creating
*    the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSComponent object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSComponent::setCSComponentAttributes (UIObject *parent)
{
  if (resourceName) xv_set (owObject, XV_INSTANCE_NAME, resourceName, NULL);

  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  if (!canvasShellDisplay) {
    CSComponent *parent_component = (CSComponent *) parent;
    canvasShellDisplay = parent_component->canvasShellDisplay;
  }

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
  
  initializeColorMap (parent);

  if (absoluteX != UIC_NOT_SET)
    xv_set (owObject, RECTOBJ_X, absoluteX, NULL);

  if (absoluteY != UIC_NOT_SET)
    xv_set (owObject, RECTOBJ_Y, absoluteY, NULL);

  if (componentMenu) {
    /* Create the XView menu object and set the component menu */
    ((UIObject *) componentMenu)->createObject (canvasShellDisplay);
    xv_set (owObject, RECTOBJ_MENU, componentMenu->getXViewObject (), NULL);
  }
  
  if (foregroundColor != UIC_NOT_SET)
    xv_set (owObject, RECTOBJ_FG, foregroundColor, NULL);
  
  if (backgroundColor != UIC_NOT_SET)
    xv_set (owObject, RECTOBJ_BG, backgroundColor, NULL);
  
  if (border != UIC_NOT_SET)
    xv_set (owObject, RECTOBJ_BORDER, border, NULL);

  if (filledSet && isDrawObject)
    xv_set (owObject, DRAWOBJ_FILLED, filledSet, NULL);

  if (exclusiveSet)
    xv_set (owObject, RECTOBJ_EXCLUSIVE_SELECT, exclusiveSet, NULL);

  if (geometrySilentSet)
    xv_set (owObject, RECTOBJ_GEOMETRY_SILENT, geometrySilent, NULL);

  if (minHeight != UIC_NOT_SET)
    xv_set (owObject, RECTOBJ_MIN_HEIGHT, minHeight, NULL);

  if (minWidth != UIC_NOT_SET)
    xv_set (owObject, RECTOBJ_MIN_WIDTH, minWidth, NULL);

  if (selectableSet)
    xv_set (owObject, RECTOBJ_SELECTABLE, selectable, NULL);

  if (selectionOwner)
    xv_set (owObject, RECTOBJ_SELECTION_OWNER, selectionOwner, NULL);

  if (stackingPosition != UIC_NOT_SET)
    xv_set (owObject, RECTOBJ_STACKING_POSITION, stackingPosition, NULL);

  if (toggleStateSet)
    xv_set (owObject, RECTOBJ_TOGGLE_STATE, toggleState, NULL);

  serverParent = canvasShellDisplay;
  UIObject::setUIObjectAttributes (parent);

  int num_children = children.getSize ();
  for (int i=0; i < num_children; i++) {
    xv_set (owObject,
	    XV_KEY_DATA, UIC_CHILD_KEY, (UIObject *) children [i],
	    NULL);

    ((UIObject *) children [i])->createObject (this);

    UICGeneric object = ((UIObject *) children [i])->getXViewObject ();
    if (object)
      xv_set (object,
	      XV_KEY_DATA, UIC_FRAME_KEY,
	      (Frame) xv_get (canvasShellDisplay->getXViewObject (),
			      XV_KEY_DATA,
			      UIC_FRAME_KEY),
	      NULL);
  }
  children.dropItems ();

  if (addChildHandler) {
    addChildHandlerSet = TRUE;
    xv_set (owObject,
	    RECTOBJ_ADD_CHILD_PROC, &CSComponent::addChildProc,
	    NULL);
  }
  
  if (deleteChildHandler) {
    deleteChildHandlerSet = TRUE;
    xv_set (owObject,
	    RECTOBJ_DEL_CHILD_PROC, &CSComponent::deleteChildProc,
	    NULL);
  }

  if (doubleClickHandler) {
    doubleClickHandlerSet = TRUE;
    xv_set (owObject,
	    RECTOBJ_DBL_CLICK_PROC, &CSComponent::doubleClickProc,
	    NULL);
  }
  
  if (geometryHandler) {
    geometryHandlerSet = TRUE;
    xv_set (owObject,
	    RECTOBJ_SET_GEOMETRY_PROC, &CSComponent::geometryProc,
	    NULL);
  }
  
  xv_set (owObject,
          RECTOBJ_SELECTION_PROC, &CSComponent::selectionProc,
          NULL);
 
  if (singleClickHandler) {
    singleClickHandlerSet = TRUE;
    xv_set (owObject,
	    RECTOBJ_SINGLE_CLICK_PROC, &CSComponent::singleClickProc,
	    NULL);
  }
  
  if (acceptDrops) {
    xv_set (owObject, RECTOBJ_ACCEPTS_DROP, TRUE, NULL);

    if (dropHandler) {
      dropHandlerSet = TRUE;
      xv_set (owObject, RECTOBJ_DROP_PROC, &CSComponent::dropProc, NULL);
    }
  }

  // If the user specified an event handler...
  if (eventHandler || useDefaultEventHandlerFlag || eventTargets.getSize () ||
      toggleMode || (draggable && !isDropTarget) || hotRegions.getSize () ||
      predefinedCSCallback) {
    eventProcSet = TRUE;

    xv_set (owObject, RECTOBJ_EVENT_PROC, &CSComponent::eventProc, NULL);
    int noEvents = events.getSize ();
    for (int i = 0; i < noEvents; i++)
      xv_set (owObject, WIN_CONSUME_EVENT, (int) events [i], NULL); 
  }

  if (selected) xv_set (owObject, RECTOBJ_SELECTED, TRUE, NULL);
  
  xv_set (owObject, XV_SHOW, showFlag, NULL);
}

/*F+ void CSComponent::createDragSite ()
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
void CSComponent::createDragSite ()
{
  if (!dragSite)
    createXViewDragSite (canvasShellDisplay->getPaintWindow ());
}

/*F+ void CSComponent::createDropSite ()
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
void CSComponent::createDropSite ()
{
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
