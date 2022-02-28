/*H+ CSComponent.h
*
*    SCCS ID: @(#)CSComponent.h 1.4 93/06/15 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSComponent class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSComponent
*
*    DESCRIPTION:
*
*    The CSComponent is a base class for all of the canvas shell
*    component classes.  It provides many attributes and callback
*    types that are common to all of the canvas shell components.
*
*    INHERITED CLASSES:
*
*    UIObject - The UIObject class allows CSComponents to reuse
*               the event handling logic provided by UIObject.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void addChild (CSComponent &) - Add a child to this object.
*    void deleteChild (CSComponent &) - Delete a child from this object.
*    void deleteAllChildren () 	- Delete all of this object's children
*
*    void setMenu (UIMenu &) - Sets a menu on this component.
*
*    void setAbsoluteX (int)
*    void setAbsoluteY (int) - Sets the X and Y coordinates of this object
*                              relative to the paint window of the
*                              CanvasShellDisplay.
*
*    int getAbsoluteX ()
*    int getAbsoluteY () - Gets the X and Y coordinates of this object relative
*                          to the paint window of the CanvasShellDisplay.
*
*    void setAcceptDrops (Boolean) - Specifies whether or not this CSComponent
*    accepts drops.
*
*    void setBackgroundColor (Color)
*    void setBackgroundColor (char *) - Specifies the background color of this
*    object.
*
*    void setForegroundColor (Color)
*    void setForegroundColor (char *) - Specifies the foreground color of this
*    object.
*
*    void setBorder (int) - Specifies the border for CSComponents that support
*    borders.
*
*    void setFilled (Boolean) - If the object is a CSDraw* object, then
*    this will specify wether or not to fill the interior.
*
*    int getNumberOfChildren () - Returns the number of children.
*
*    CSComponent *getChild (int) - Returns the Nth child of this object.
*    Indexing starts at 1.
*
*    void setExclusiveSelect (Boolean) - If set, all other objects are turned
*    off when this object is selected.
*
*    void setGeometrySlient (Boolean) - If set, the parent will not be notified
*    if the geometry of this object changes.
*
*    void setMaxWidth (int)
*    void setMinWidth (int) - Specifies the minimum and maximum widths of this
*    object.
*
*    void setSelectable (Boolean) - If set, this object is selectable with a
*    single click.
*
*    Boolean isSelectable () - Returns whether or not this object is selectable.
*
*    void setSelected (Boolean) - Specifies whether or not this object is in
*    a selected state.
*
*    Boolean isSelected () - Returns whether or not this object is selected.
*
*    void setSelectionOwner (Selection_owner) - Sets the XView selection owner
*    of this object.
*
*    Selection_owner getXViewSelectionOwner () - Gets the XView selection owner
*    of this object.
*
*    void setXViewSelectionHandler (UICHandlerFunction) - Called when this object
*    is selected or deselected.
*
*    void setStackingPosition (int) - Sets the stacking position of an object
*    relative to siblings of the same parent.
*
*    int getStackingPosition () - Gets the stacking position of an object
*    relative to siblings of the same parent.
*
*    void setToggle (Boolean) - If  set, this object will act as a 2-state
*    toggle.
*
*    Boolean toggleState () - Toggles the state of this object and returns
*    the new state.
*
*    Boolean getToggleState () - Gets the current toggle state.
*
*    CSComponent *getNewChild () - When called from an add child handler
*    this will return the new child.
*
*    CSComponent *getDeletedChild () - When called from a delete child
*    handler this will return the child that was deleted.
*
*    Rect *getRectWithChild () - When called from an add or a delete child
*    handler this will return the rectangle that totally encompesses the
*    object after the child is added and before the child is deleted.
*
*    Rect *getNewRect () - When called from a geometry handler this will
*    return the rectangle that encompassed the object after the geometry
*    changed.
*
*    Rect *getOldRect () - When called from a geometry handler this will
*    return the rectangle that encompassed the object before the geometry
*    changed.
*
*    void setAddChildHandler (UICHandlerFunction) - Specifies a handler to be
*    called when a child is added to this object.
*
*    void setDeleteChildHandler (UICHandlerFunction) - Specifies a handler to be
*    called when a child is deleted from this object.
*
*    void setDoubleClickHandler (UICHandlerFunction) - Specifies a handler
*    to be if a double click event occurs.
*
*    void setGeometryHandler (UICHandlerFunction) - Called when the geometry
*    of this object has changed.
*
*    void setPredifinedCSCallback (PredefinedCSCallback) - Specifies a
*    predefined callback to be called to handle events.
*
*    void setSelectionHandler (UICHandlerFunction) - Specifies a handler to 
*    be called when this component is selected. 
*
*    void setSingleClickHandler (UICHandlerFunction) - Called when a single
*    click event occurs.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    Rectobj encapsulated by this object.
*
*    void setCSComponentAttributes (UIObject *parent) - Sets the CSComponent
*    specific attributes of this object.
*
*    void createDragSite () - Creates the drag site for sourcing drags
*    from this object.
*
*    void createDropSite () - Normally used to create drop sites, but
*    since the SlingShot packages handles it already, this is an empty
*    functions.
*
*    static void eventProc (Xv_window     paint_window,
*  			    Event        *event,
*  			    Canvas_shell  canvas_shell,
*  			    Rectobj       rectobj);
*           - RECTOBJ_EVENT_PROC
*  
*    static void addChildProc (Rectobj  parent,
*  			       Rectobj  child,
*  			       Rect    *rect);
*           - RECTOBJ_ADD_CHILD_PROC
*  
*    static void deleteChildProc (Rectobj  parent,
*  			          Rectobj  child,
*  			          Rect    *rect);
*           - RECTOBJ_DELETE_CHILD_PROC
*  
*    static void doubleClickProc (Xv_window     paint_window,
*  			          Event        *event,
*  			          Canvas_shell  canvas_shell,
*  			          Rectobj       rectobj);
*           - RECTOBJ_DBL_CLICK_PROC
*  
*    static void geometryProc (Rectobj  rectobj,
*  			       Rect    *new_rect,
*  			       Rect    *old_rect);
*           - RECTOBJ_SET_GEOMETRY_PROC
*  
*    static void dropProc (Xv_window     paint_window,
*  			   Event        *event,
*  			   Canvas_shell  canvas_shell,
*  			   Rectobj       rectobj);
*           - RECTOBJ_DROP_PROC
*
*    static void selectionProc (Rectobj  rectobj,
*  			        int      selected,
*                               Event   *event);
*           - RECTOBJ_SELECTION_PROC
*  
*    static void singleClickProc (Xv_window     paint_window,
*  			          Event        *event,
*  			          Canvas_shell  canvas_shell,
*  			          Rectobj       rectobj,
*  			          int	        state);
*           - RECTOBJ_SINGLE_CLICK_PROC
*  
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSComponentDefaults () - Initializes data members.
*
*C-*/

#ifndef _CS_COMPONENT_H
#define _CS_COMPONENT_H

// Includes
#include "UIObject.h"
#include "UIMenu.h"

#include <xview/win_input.h>
#include <xview/rect.h>
#include <pixrect/pixrect.h>
#include <pixrect/pixfont.h>
#include <xview/frame.h>
#include <xview/scrollbar.h>
#include <xview/xv_xrect.h>
#include <sspkg/canshell.h>
#include <sspkg/drawobj.h>

// Type Definitions
typedef void (*PredefinedCSCallback) (Xv_window,
				      Event *,
				      Canvas_shell,
				      Rectobj);

class CanvasShellDisplay;

// Class Definition
class CSComponent : public UIObject {
public:
  CSComponent ();
  ~CSComponent ();

  void addChild (CSComponent &);
  void deleteChild (CSComponent &);

  void deleteAllChildren() 
    {
      int num_children = getNumberOfChildren ();

      for(int i = num_children; i > 0; i--)
	deleteChild (*getChild (i));
    }

  void setMenu (UIMenu &);

  void setAbsoluteX (int x)
    {
      absoluteX = x;

      if (owObject)
	xv_set (owObject, RECTOBJ_X, absoluteX, NULL);
    }

  void setAbsoluteY (int y)
    {
      absoluteY = y;

      if (owObject)
	xv_set (owObject, RECTOBJ_Y, absoluteY, NULL);
    }

  int getAbsoluteX ()
    {
      return (owObject ? xv_get (owObject, RECTOBJ_X) : absoluteX);
    }

  int getAbsoluteY ()
    {
      return (owObject ? xv_get (owObject, RECTOBJ_Y) : absoluteY);
    }

  void setAcceptDrops (Boolean flag)
    {
      acceptDrops = flag;

      if (owObject) {
	xv_set (owObject, RECTOBJ_ACCEPTS_DROP, flag, NULL);

	if (!dropHandlerSet) {
	  dropHandlerSet = TRUE;
	  xv_set (owObject, RECTOBJ_DROP_PROC, &CSComponent::dropProc, NULL);
	}
      }
    }

  void setForegroundColor (Color);
  void setForegroundColor (char *);

  void setBackgroundColor (Color);
  void setBackgroundColor (char *);

  void setBorder (int value)
    {
      border = value;

      if (owObject)
	xv_set (owObject, RECTOBJ_BORDER, border, NULL);
    }

  void setFilled (Boolean flag)
    {  
      filled = flag;
      filledSet = TRUE;

      if (owObject && isDrawObject)
        xv_set (owObject, DRAWOBJ_FILLED, flag, NULL);
    }

  int getNumberOfChildren ()
    {
      return (owObject ? (int) xv_get (owObject, RECTOBJ_N_CHILDREN) :
			 children.getSize ());
    }

  CSComponent *getChild (int);

  void setExclusiveSelect (Boolean flag)
    {
      exclusive = flag;
      exclusiveSet = TRUE;

      if (owObject)
	xv_set (owObject, RECTOBJ_EXCLUSIVE_SELECT, flag, NULL);
    }

  void setGeometrySilent (Boolean flag)
    {
      geometrySilent = flag;
      geometrySilentSet = TRUE;

      if (owObject)
	xv_set (owObject, RECTOBJ_GEOMETRY_SILENT, flag, NULL);
    }

  void setMinHeight (int height)
    {
      minHeight = height;

      if (owObject)
	xv_set (owObject, RECTOBJ_MIN_HEIGHT, minHeight, NULL);
    }

  int getMinHeight ()
    {
      return (owObject ? xv_get (owObject, RECTOBJ_MIN_HEIGHT) : minHeight);
    }

  void setMinWidth (int width)
    {
      minWidth = width;

      if (owObject)
	xv_set (owObject, RECTOBJ_MIN_WIDTH, minWidth, NULL);
    }

  int getMinWidth ()
    {
      return (owObject ? xv_get (owObject, RECTOBJ_MIN_WIDTH) : minWidth);
    }

  void setSelectable (Boolean flag)
    {
      selectable = flag;
      selectableSet = TRUE;

      if (owObject)
	xv_set (owObject, RECTOBJ_SELECTABLE, selectable, NULL);
    }

  Boolean isSelectable ()
    {
      return (owObject ?
	      (Boolean) xv_get (owObject, RECTOBJ_SELECTABLE) : selectable);
    }

  void setSelected (Boolean flag)
    {
      selected = flag;

      if (owObject)
	xv_set (owObject, RECTOBJ_SELECTED, selected, NULL);
    }

  Boolean isSelected ()
    {
      return (owObject ?
	      (Boolean) xv_get (owObject, RECTOBJ_SELECTED) : selected);
    }

  void setXViewSelectionOwner (Selection_owner owner)
    {
      selectionOwner = owner;

      if (owObject)
	xv_set (owObject, RECTOBJ_SELECTION_OWNER, selectionOwner, NULL);
    }

  Selection_owner getXViewSelectionOwner ()
    {
      return (owObject ?
	      (Selection_owner) xv_get (owObject, RECTOBJ_SELECTION_OWNER) :
	      selectionOwner);
    }

  void setStackingPosition (int position)
    {
      stackingPosition = position;

      if (owObject)
	xv_set (owObject, RECTOBJ_STACKING_POSITION, stackingPosition, NULL);
    }

  int getStackingPosition ()
    {
      return (owObject ?
	      xv_get (owObject, RECTOBJ_STACKING_POSITION) : stackingPosition);
    }

  void setToggleMode (Boolean flag)
    {
      toggleMode = flag;

      if (owObject && !eventProcSet) {
	toggleModeSet = TRUE;

	xv_set (owObject,
		RECTOBJ_EVENT_PROC, &CSComponent::eventProc,
		NULL);
      }
    }

  void setToggleState (Boolean flag)
    {
      toggleState = flag;
      toggleStateSet = TRUE;

      if (owObject)
        xv_set (owObject, RECTOBJ_TOGGLE_STATE, toggleState, NULL);
    }

  Boolean getToggleState ()
    {
      toggleStateSet = TRUE;

      if (owObject)
	toggleState = (Boolean) xv_get (owObject, RECTOBJ_TOGGLE_STATE);

      return toggleState;
    }

  CSComponent *getNewChild ()
    {
      return newChild;
    }

  CSComponent *getDeletedChild ()
    {
      return deletedChild;
    }

  Rect *getRectWithChild ()
    {
      return rectWithChild;
    }

  int getStartDragX ()
    {
      return startDragX;
    }
  
  int getStartDragY ()
    {
      return startDragY;
    }
  
  int getStartDragAdjust ()
    {
      return startDragAdjust;
    }
  
  void setAddChildHandler (UICHandlerFunction handler)
    {
      addChildHandler = handler;

      if (owObject && !addChildHandlerSet) {
        addChildHandlerSet = TRUE;

        xv_set (owObject,
                RECTOBJ_ADD_CHILD_PROC, &CSComponent::addChildProc,
                NULL);
      }  
    }

  void setDeleteChildHandler (UICHandlerFunction handler)
    {
      deleteChildHandler = handler;

      if (owObject && !deleteChildHandlerSet) {
        deleteChildHandlerSet = TRUE;

        xv_set (owObject,
                RECTOBJ_DEL_CHILD_PROC, &CSComponent::deleteChildProc,
                NULL);
      }  
    }

  void setDoubleClickHandler (UICHandlerFunction handler)
    {
      doubleClickHandler = handler;

      if (owObject && !doubleClickHandlerSet) {
	doubleClickHandlerSet = TRUE;

	xv_set (owObject,
		RECTOBJ_DBL_CLICK_PROC, &CSComponent::doubleClickProc,
		NULL);
      }
    }

  void setGeometryHandler (UICHandlerFunction handler)
    {
      geometryHandler = handler;

      if (owObject && !geometryHandlerSet) {
	geometryHandlerSet = TRUE;

	xv_set (owObject,
		RECTOBJ_SET_GEOMETRY_PROC, &CSComponent::geometryProc,
		NULL);
      }
    }

  void setPredefinedCSCallback (PredefinedCSCallback callback)
    {
      predefinedCSCallback = callback;

      if (owObject && !eventProcSet) {
	predefinedCSCallbackSet = TRUE;

	xv_set (owObject,
		RECTOBJ_EVENT_PROC, &CSComponent::eventProc,
		NULL);
      }
    }

  void setSelectionHandler (UICHandlerFunction handler)
    {
      selectionHandler = handler;

      if (owObject && !selectionHandlerSet) {
	selectionHandlerSet = TRUE;

	xv_set (owObject,
		RECTOBJ_SELECTION_PROC, &CSComponent::selectionProc,
		NULL);
      }
    }

  void setSingleClickHandler (UICHandlerFunction handler)
    {
      singleClickHandler = handler;

      if (owObject && !singleClickHandlerSet) {
	singleClickHandlerSet = TRUE;

	xv_set (owObject,
		RECTOBJ_SINGLE_CLICK_PROC, &CSComponent::singleClickProc,
		NULL);
      }
    }

protected:
  int                  absoluteX;
  int                  absoluteY;
  int                  border;
  Boolean	       filled;
  Boolean	       filledSet;
  GenericList          children;
  UIMenu	      *componentMenu;
  Boolean              exclusive;
  Boolean              exclusiveSet;
  Boolean              geometrySilent;
  Boolean              geometrySilentSet;
  int                  minHeight;
  int                  minWidth;
  Boolean              selectable;
  Boolean              selectableSet;
  Boolean              selected;
  Selection_owner      selectionOwner;
  int                  stackingPosition;
  int                  startDragX;
  int                  startDragY;
  int                  startDragAdjust;
  Boolean              toggleMode;
  Boolean              toggleModeSet;
  Boolean              toggleState;
  Boolean              toggleStateSet;
  CSComponent         *newChild;
  CSComponent         *deletedChild;
  Rect                *rectWithChild;
  Rect                *newRect;
  Rect                *oldRect;
  Boolean              eventProcSet;
  Boolean              dropHandlerSet;
  UICHandlerFunction   addChildHandler;
  Boolean	       addChildHandlerSet;
  UICHandlerFunction   deleteChildHandler;
  Boolean	       deleteChildHandlerSet;
  UICHandlerFunction   doubleClickHandler;
  Boolean              doubleClickHandlerSet;
  UICHandlerFunction   geometryHandler;
  Boolean              geometryHandlerSet;
  PredefinedCSCallback predefinedCSCallback;
  Boolean              predefinedCSCallbackSet;
  UICHandlerFunction   selectionHandler;
  Boolean              selectionHandlerSet;
  UICHandlerFunction   singleClickHandler;
  Boolean              singleClickHandlerSet;
  Boolean	       isDrawObject;
  CanvasShellDisplay  *canvasShellDisplay;

  virtual void createObject (UIObject *parent);
  void         setCSComponentAttributes (UIObject *parent);

  virtual void createDropSite ();
  virtual void createDragSite ();

#ifdef __GNUC__
public:
#endif
  static void eventProc (Xv_window     paint_window,
			 Event        *event,
			 Canvas_shell  canvas_shell,
			 Rectobj       rectobj);

  static void addChildProc (Rectobj  parent,
			    Rectobj  child,
			    Rect    *rect);

  static void deleteChildProc (Rectobj  parent,
			       Rectobj  child,
			       Rect    *rect);

  static void doubleClickProc (Xv_window     paint_window,
			       Event        *event,
			       Canvas_shell  canvas_shell,
			       Rectobj       rectobj);

  static void geometryProc (Rectobj  rectobj,
			    Rect    *new_rect,
			    Rect    *old_rect);

  static void selectionProc (Rectobj  rectobj,
			     int      selected,
			     Event   *event);

  static void singleClickProc (Xv_window     paint_window,
			       Event        *event,
			       Canvas_shell  canvas_shell,
			       Rectobj       rectobj,
			       int	     state);

  static void dropProc (Xv_window     paint_window,
			Event        *event,
			Canvas_shell  canvas_shell,
			Rectobj       rectobj);

private:    
  void setCSComponentDefaults ();

friend class CanvasShellDisplay;
};

#endif /* _CS_COMPONENT_H */
