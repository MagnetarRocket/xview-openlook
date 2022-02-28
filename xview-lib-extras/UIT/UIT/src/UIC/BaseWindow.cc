/*H+ BaseWindow.cc
*
*    SCCS ID: @(#)BaseWindow.cc 1.8 93/04/09 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines member functions for the BaseWindow class.
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
static char sccsid [] = "@(#)BaseWindow.cc 1.8 93/04/09 SMI"; 
#endif

// Includes
#include "BaseWindow.h"
#include "CharacterFont.h"
#include "ColorMap.h"

/*F+ BaseWindow::BaseWindow ()
*
*    PURPOSE:
*
*    Constructor for the BaseWindow class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Debug and prepare for release
*
*F-*/
BaseWindow::BaseWindow ()
{
  setBaseWindowDefaults ();
}
    
/*F+ BaseWindow::BaseWindow (char *string)
*
*    PURPOSE:
*
*    Constructor for the BaseWindow class.
*
*    PARAMETERS:
*
*    char *string - String label for the base window.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Debug and prepare for release
*
*F-*/
BaseWindow::BaseWindow (char *string) : UIWindow (string)
{
  setBaseWindowDefaults ();
}

/*F+ BaseWindow::setBaseWindowDefaults ()
*
*    PURPOSE:
*
*    Set default values for the data members.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Original creation.
*
*F-*/
void BaseWindow::setBaseWindowDefaults ()
{
  closed = FALSE;
  icon = (Image *) NULL;
  newIcon = FALSE;
  iconMask = (Image *) NULL;
  newIconMask = FALSE;
  iconX = UIC_NOT_SET;
  iconY = UIC_NOT_SET;
  iconLabel = (char *) NULL;
  busyFlag = FALSE;
  noConfirmFlag = TRUE;
  destroyPrompt = (NoticePrompt *) NULL;
  noDestroy = FALSE;
  doneHandlerSet = FALSE;
  saveHandler = (UICHandlerFunction) NULL;
  showFlag = FALSE;
  iconEventHandler = (UICHandlerFunction) NULL;
  iconEventHandlerSet = FALSE;
  resizable = TRUE;
  isBaseWindow = TRUE;
  useDefaultSaveHandlerFlag = FALSE;
  useDefaultIconEventHandlerFlag = FALSE;

  if (!baseWindow)
    baseWindow = this;
}

/*F+ BaseWindow::~BaseWindow ()
*
*    PURPOSE:
*
*    Destructor for the BaseWindow class.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*    MGS       11/01/90   Debug and prepare for release
*
*F-*/
BaseWindow::~BaseWindow ()
{
  childWindows.dropItems ();
  peerWindows->dropItems ();

  if (iconLabel) delete iconLabel;
}

/*F+ BaseWindow::setSaveHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Sets the handler to be called when a save status is
*    received by the base frame.
*
*    PARAMETERS:
*
*    handler - The handler to be called.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Original creation.
*
*F-*/
void BaseWindow::setSaveHandler (UICHandlerFunction handler)
{
  if (owObject && !doneHandlerSet) {
    notify_interpose_destroy_func (owObject,
				   (Notify_func) &BaseWindow::doneProc);
    doneHandlerSet = TRUE;
  }

  saveHandler = handler;
}

/*F+ BaseWindow::setDestroyPrompt (NoticePrompt &prompt)
*
*    PURPOSE:
*
*    Sets the notify prompt to be shown when the desctruction of
*    the base window is to be confirmed.
*
*    PARAMETERS:
*
*    prompt - The NoticePrompt to be shown.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Original creation.
*
*F-*/
void BaseWindow::setDestroyPrompt (NoticePrompt &prompt)
{
  if (owObject && !doneHandlerSet) {
    notify_interpose_destroy_func (owObject,
				   (Notify_func) &BaseWindow::doneProc);
    doneHandlerSet = TRUE;
  }

  destroyPrompt = &prompt;
}

/*F+ BaseWindow::setDoneHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Sets the handler to be called at every stage of a BaseWindow
*    destroy.
*
*    PARAMETERS:
*
*    handler - The handler to be called.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Original creation.
*
*F-*/
void BaseWindow::setDoneHandler (UICHandlerFunction handler)
{
  if (owObject && !doneHandlerSet) {
    notify_interpose_destroy_func (owObject,
				   (Notify_func) &BaseWindow::doneProc);
    doneHandlerSet = TRUE;
  }

  doneHandler = handler;
}

/*F+ BaseWindow::setIconEventHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Sets the handler to be called when an event occurs on the
*    base frame icon.
*
*    PARAMETERS:
*
*    handler - The handler to be called.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Original creation.
*
*F-*/
void BaseWindow::setIconEventHandler (UICHandlerFunction handler)
{
  iconEventHandler = handler;

  if (owObject && !iconEventHandlerSet) {
    Icon icon = (Icon) xv_get (owObject, FRAME_ICON);

    xv_set (icon, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
    notify_interpose_event_func (icon,
    				 (Notify_func) &BaseWindow::iconEventProc,
				 NOTIFY_SAFE);
    iconEventHandlerSet = TRUE;
  }
}

/*F+ Notify_value BaseWindow::doneProc (Notify_client  client,
*					Destroy_status status)
*
*    PURPOSE:
*
*    A wrapper function for the XView frame done proc.  This is called
*    when the BaseWindow is exited or a save layout is requested.
*
*    PARAMETERS:
*
*    client - the frame being quit.
*    status - the XView destroy status of the frame.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Original creation.
*
*F-*/
Notify_value BaseWindow::doneProc (Notify_client  client, 
				   Destroy_status status)
{
  BaseWindow *object = (BaseWindow *) xv_get (client,
					      XV_KEY_DATA,
					      UIC_OBJECT_KEY);

  if (!object)
    return notify_next_destroy_func (client, status);
  
  /* Set the destroy status */
  object->setDestroyStatus ((DestroyStatus) status);

  /* Call the user specified done handler... */
  if (object->useDefaultDoneHandlerFlag)
    object->defaultDoneHandler ();
  else if (object->doneHandler)
    (*object->doneHandler) (object);

  /* Only check data members if the status is not DESTROY_CLEANUP
     becuase if the status is DESTROY_CLEANUP, assume that delete
     was called on the object in the done handler */
  if (status != DESTROY_CLEANUP) {
    /* If a NoticePrompt has been specified, show it... */
    if (object->destroyPrompt && (status == DESTROY_CHECKING))
      if (object->destroyPrompt->show (object) == NOTICE_NO) {
        notify_veto_destroy (client);
        return NOTIFY_DONE;
      }
  
    if (status == DESTROY_SAVE_YOURSELF) {
      /* Call the user specified save handler... */
      if (object->useDefaultSaveHandlerFlag)
        object->defaultSaveHandler ();
      else if (object->saveHandler) {
        (*(object->saveHandler)) (object);
        return NOTIFY_DONE;
      }
    }

    if ((status == DESTROY_CHECKING) && object->noDestroy) {
      notify_veto_destroy (client);
      return NOTIFY_DONE;
    }

    if ((status == DESTROY_CHECKING) && (!object->noConfirmFlag))
      return notify_next_destroy_func (client, status);
  }
  /* Status is DESTROY_CLEANUP */
  else {
    return notify_next_destroy_func (client, status);
  }

  return NOTIFY_DONE;
}

/*F+ Notify_value BaseWindow::iconEventProc (Icon              icon,
*                                            Event            *event,
*				             Notify_arg        arg,
*				             Notify_event_type event_type)
*
*    PURPOSE:
*
*    A warpper function for the XView event proc on an icon.  This is
*    called when an event occurs on an iconified BaseWindow.
*
*    PARAMETERS:
*
*    icon       - The icon the event occured on.
*    event      - The pointer to the XView event structure.
*    arg        - Optional argument (not used)
*    event_type - The type of event (not used).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Original creation.
*
*F-*/
Notify_value BaseWindow::iconEventProc (Icon		   icon,
					Event             *event,
					Notify_arg         arg, 
					Notify_event_type  event_type)
{
  BaseWindow *object = (BaseWindow *) xv_get (icon,
					      XV_KEY_DATA,
					      UIC_OBJECT_KEY);
  
  object->setEventInformation (event);
  
  if (object->useDefaultIconEventHandlerFlag)
    object->defaultIconEventHandler ();
  else if (object->iconEventHandler)
    (*object->iconEventHandler) (object);

  object->resetEventInformation ();

  if (object->eventResultSet) {
    object->eventResultSet = FALSE;
    return (object->eventResult);
  }

  return (notify_next_event_func (icon,
				  (Notify_event) event,
				  arg,
				  event_type)); 
}

/*F+ BaseWindow::show (Boolean show_flag)
*
*    PURPOSE:
*
*    Specifies whether or not to show the BaseWindow.
*
*    PARAMETERS:
*
*    Boolean show_flag - TRUE to show, FALSE not to show
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/01/90   Original creation.
*
*F-*/
void BaseWindow::show (Boolean show_flag)
{
  showFlag = show_flag;

  if (baseWindow->getXViewObject ()) {
    if (!owObject) createObject (parentWindow);
    xv_set (owObject, XV_SHOW, showFlag, NULL);
  }
}

/*F+ BaseWindow::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Sets all of the XView attributes for the BaseWindow XView object.
*
*    PARAMETERS:
*
*    UIObject *parent - The parent of the window.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    JW        10/01/90   Original creation.
*    MGS       11/01/90   Debug and prepare for release.
*
*F-*/
void BaseWindow::createObject (UIObject *parent)
{
  /* The parent should either be NULL or have been set by the
     setParentWindow member function that is called internally
     by addChildWindow.  If this test is not passed, then this
     BaseWindow is listed redundantly in another window's child
     window list and should be ignored... */
  if (parent != parentWindow) return;

  // If a parent was specified, use it
  UICGeneric ow_parent = ((parent) ? parent->getXViewObject () :
				     (UICGeneric) NULL);

  /* XView requires that XV_X, XV_Y and FRAME_SHOW_RESIZE_CORNER
     are set in the xv_create call, if set at all.  XV_WIDTH and
     XV_HEIGHT are set in the xv_create call so that -Ws, -WG and
     -geometry switches have precedence over the initial size values.
     If width and height are set by an xv_set call, then they have
     precedence over server defaults and command line options. */

  // If either an X or Y location is set...
  if ((xLoc != UIC_NOT_SET) || (yLoc != UIC_NOT_SET)) {
    if (xLoc == UIC_NOT_SET) xLoc = 0;
    if (yLoc == UIC_NOT_SET) yLoc = 0;

    // Determine whether or not there is a preset width or height

    // If neither width or height are set...
    if ((width == UIC_NOT_SET) && (height == UIC_NOT_SET)) {
      owObject = xv_create (ow_parent, FRAME,
			    XV_X, 		      xLoc,
			    XV_Y, 		      yLoc,
			    FRAME_SHOW_RESIZE_CORNER, resizable,
			    NULL);
    }
    // Else, if both are set...
    else if ((width != UIC_NOT_SET) && (height != UIC_NOT_SET)) {
      owObject = xv_create (ow_parent, FRAME,
			    XV_X, 		      xLoc,
			    XV_Y, 		      yLoc,
			    XV_WIDTH,                 width,
			    XV_HEIGHT,                height,
			    FRAME_SHOW_RESIZE_CORNER, resizable,
			    NULL);
    }
    // Else, if only width is set...
    else if (width != UIC_NOT_SET) {
      owObject = xv_create (ow_parent, FRAME,
			    XV_X, 		      xLoc,
			    XV_Y, 		      yLoc,
			    XV_WIDTH,                 width,
			    FRAME_SHOW_RESIZE_CORNER, resizable,
			    NULL);
    }
    // Else, only height is set...
    else {
      owObject = xv_create (ow_parent, FRAME,
			    XV_X, 		      xLoc,
			    XV_Y, 		      yLoc,
			    XV_HEIGHT,                height,
			    FRAME_SHOW_RESIZE_CORNER, resizable,
			    NULL);
    }
  }
  // Else, no preset location...
  else {
    // Determine whether or not there is a preset width or height

    // If neither width or height are set...
    if ((width == UIC_NOT_SET) && (height == UIC_NOT_SET)) {
      owObject = xv_create (ow_parent, FRAME,
			    FRAME_SHOW_RESIZE_CORNER, resizable,
			    NULL);
    }
    // Else, if both are set...
    else if ((width != UIC_NOT_SET) && (height != UIC_NOT_SET)) {
      owObject = xv_create (ow_parent, FRAME,
			    XV_WIDTH,                 width,
			    XV_HEIGHT,                height,
			    FRAME_SHOW_RESIZE_CORNER, resizable,
			    NULL);
    }
    // Else, if only width is set...
    else if (width != UIC_NOT_SET) {
      owObject = xv_create (ow_parent, FRAME,
			    XV_WIDTH,                 width,
			    FRAME_SHOW_RESIZE_CORNER, resizable,
			    NULL);
    }
    // Else, only height is set...
    else {
      owObject = xv_create (ow_parent, FRAME,
			    XV_HEIGHT,                height,
			    FRAME_SHOW_RESIZE_CORNER, resizable,
			    NULL);
    }
  }

  xv_set (owObject, XV_KEY_DATA, UIC_FRAME_KEY, owObject, NULL);
  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  setHelpData ();

  if (label) xv_set (owObject, XV_LABEL, label, NULL);
  
  if (font) {
    ((UIObject *) font)->createObject (this);
    xv_set (owObject, XV_FONT, font->getXViewObject (), NULL);
  }
  
  xv_set (owObject, FRAME_BUSY, busyFlag, NULL);
  xv_set (owObject, FRAME_NO_CONFIRM, noConfirmFlag, NULL);
  xv_set (owObject, FRAME_SHOW_RESIZE_CORNER, resizable, NULL);
  xv_set (owObject, FRAME_SHOW_FOOTER, displayFooter, NULL);
     
  if (leftFooter)
    xv_set (owObject, FRAME_LEFT_FOOTER, leftFooter, NULL);
		 
  if (rightFooter)
    xv_set (owObject, FRAME_RIGHT_FOOTER, rightFooter, NULL);
  
  if (label != (char *) NULL)
    xv_set (owObject, XV_LABEL, label, NULL);
  
  initializeColorMap (this);
  
  if (foregroundColor != UIC_NOT_SET)
    xv_set (owObject,
	    WIN_FOREGROUND_COLOR, foregroundColor,
	    NULL);
  
  if (backgroundColor != UIC_NOT_SET)
    xv_set (owObject,
	    WIN_BACKGROUND_COLOR, backgroundColor,
	    NULL);
  
  if (icon != (Image *) NULL) {
    Icon temp_icon;

    ((UIObject *) icon)->createObject (this);

    if (iconMask != (Image *) NULL) {
      ((UIObject *) iconMask)->createObject (this);
      temp_icon = xv_create (owObject, ICON,
			     ICON_IMAGE, icon->getXViewObject (),
			     ICON_MASK_IMAGE, iconMask->getXViewObject (),
			     NULL);
    }
    else
      temp_icon = xv_create (owObject, ICON,
			     ICON_IMAGE, icon->getXViewObject (),
			     NULL);

    xv_set (owObject, FRAME_ICON, temp_icon, NULL);

    if (iconX != UIC_NOT_SET)
      xv_set (temp_icon, XV_X, iconX, NULL);

    if (iconY != UIC_NOT_SET)
      xv_set (temp_icon, XV_Y, iconY, NULL);

    if (iconLabel)
      xv_set (temp_icon, XV_LABEL, iconLabel, NULL);
  }
  
  if (closed)
    xv_set (owObject, FRAME_CLOSED, TRUE, NULL);
  
  // If the user specified an event handler
  if (eventHandler || eventTargets.getSize () || useDefaultEventHandlerFlag ||
      (acceptDrops && (dropHandler || useDefaultDropHandlerFlag)) ||
      draggable || hotRegions.getSize ()) {
    xv_set (owObject, WIN_EVENT_PROC, &UIWindow::eventProc, NULL);

    int noEvents = events.getSize (); 
    for (int i = 0; i < noEvents; i++) 
      xv_set (owObject, WIN_CONSUME_EVENT, (int) events [i], NULL);
  }

  if (saveHandler || destroyPrompt || doneHandler ||
      useDefaultSaveHandlerFlag || useDefaultDoneHandlerFlag) {
    notify_interpose_destroy_func (owObject,
			           (Notify_func) &BaseWindow::doneProc);
    doneHandlerSet = TRUE;
  }
  
  if (iconEventHandler || useDefaultIconEventHandlerFlag) {
    Icon icon = (Icon) xv_get (owObject, FRAME_ICON);

    xv_set (icon, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
    notify_interpose_event_func (icon,
    				 (Notify_func) &BaseWindow::iconEventProc,
				 NOTIFY_SAFE);
    iconEventHandlerSet = TRUE;
  }

  int num_displays = displayList.getSize ();
  int i;
  for (i=0; i < num_displays; i++)
    ((UIObject *) displayList [i])->createObject (this);
  
  if (fitWidth)
    window_fit_width (owObject);
  
  if (fitHeight)
    window_fit_height (owObject);
  
  if (this == baseWindow) {
    int num_peer_windows = peerWindows->getSize ();

    for (i=0; i < num_peer_windows; i++)
      if ((*peerWindows) [i] != this)
        ((UIObject *) (*peerWindows) [i])->createObject ((UIObject *) NULL);
  }

  int num_child_windows = childWindows.getSize ();
  for (i=0; i < num_child_windows; i++)
    ((UIObject *) childWindows [i])->createObject (this);

  setUIWindowAttributes (parent);
}

#ifdef TEST
#endif /* TEST */
