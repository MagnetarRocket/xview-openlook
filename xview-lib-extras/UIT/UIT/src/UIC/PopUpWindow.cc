/*H+ PopUpWindow.cc
*
*    SCCS ID: @(#)PopUpWindow.cc 1.7 93/04/09 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the PopUpWindow class.
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
static char sccsid [] = "@(#)PopUpWindow.cc 1.7 93/04/09 SMI"; 
#endif

// Includes
#include "BaseWindow.h"
#include "CharacterFont.h"
#include "PopUpWindow.h"

/*F+ PopUpWindow::PopUpWindow ()
*
*    PURPOSE:
*
*    PopUpWindow constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
PopUpWindow::PopUpWindow ()
{
  setPopUpWindowDefaults ();
}

/*F+ PopUpWindow::PopUpWindow (char *string_label)
*
*    PURPOSE:
*
*    PopUpWindow constructor.
*
*    PARAMETERS:
*
*    string_label - Specifies a string to be used as the label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
PopUpWindow::PopUpWindow (char *string_label) : UIWindow (string_label)
{
  setPopUpWindowDefaults ();
}

/*F+ void PopUpWindow::setPopUpWindowDefaults ()
*
*    PURPOSE:
*
*    Initializes the data memeber of the PopUpWindow.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void PopUpWindow::setPopUpWindowDefaults ()
{
  pinned = FALSE;
  resizable = FALSE;
  showFlag = FALSE;

  if (!firstPopUp)
    firstPopUp = this;
}

/*F+ PopUpWindow::~PopUpWindow ()
*
*    PURPOSE:
*
*    PopUpWindow destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
PopUpWindow::~PopUpWindow ()
{
}

/*F+ void PopUpWindow::setPinned (Boolean pinned_flag)
*
*    PURPOSE:
*
*    Specifies whether or not this window is pinned when displayed.
*
*    PARAMETERS:
*
*    pinned_flag - TRUE if the window is pinned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void PopUpWindow::setPinned (Boolean pinned_flag)
{
  pinned = pinned_flag;

  if (owObject)
    xv_set (owObject, FRAME_CMD_PUSHPIN_IN, pinned, NULL);
}

/*F+ void PopUpWindow::show (Boolean show_flag)
*
*    PURPOSE:
*
*    Specifies whether or not to display this window.
*
*    PARAMETERS:
*
*    show_flag - TRUE if the window is to be displayed.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void PopUpWindow::show (Boolean show_flag)
{
  showFlag = show_flag;

  /* If no parent window has been specified and XView objects have been
     created... */
  if (!parentWindow && xViewObjectsCreated && (this != baseWindow))
    // Set the parent window to be the main base window
    parentWindow = (UIWindow *) baseWindow;

  // If the pop-up window XView object does not exist, but the parent does...
  if (!owObject && parentWindow && parentWindow->getXViewObject ())
    // Create the XView object
    createObject (parentWindow);

  // If the XView object exists...
  if (owObject) xv_set (owObject, XV_SHOW, showFlag, NULL);
}

/*F+ void PopUpWindow::doneProc (Frame frame)
*
*    PURPOSE:
*
*    Internal wrapper for the done proc on a pop-up window.  When a
*    pop-up window is dismissed, XView calls this function.  This
*    function then sets XV_SHOW to FALSE and calls the user specified
*    done handler.  If the user does not want the window to disappear,
*    then the user can use "show (TRUE)" on the pop-up window inside
*    of the done handler.
*
*    PARAMETERS:
*
*    frame - The XView frame object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void PopUpWindow::doneProc (Frame frame)
{
  PopUpWindow *object = (PopUpWindow *) xv_get (frame,
						XV_KEY_DATA,
						UIC_OBJECT_KEY);
  
  if (!object) return;

  // Turn off the window
  xv_set (object->owObject, XV_SHOW, FALSE, NULL);

  // Call the user specified done handler
  if (object->useDefaultDoneHandlerFlag)
    object->defaultDoneHandler ();
  else if (object->doneHandler)
    (*object->doneHandler) (object);
}

/*F+ void PopUpWindow::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The pop-up window parent (a class derived from UIWindow).
*             If no parent is specified, the default is the main base
*             window.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void PopUpWindow::createObject (UIObject *parent)
{
  // If no parent passed in, use the main BaseWindow
  if (!parent && (this != baseWindow))
    parent = baseWindow;

  // If no parent was specified...
  if (!parentWindow && (this != baseWindow))
    parentWindow = (UIWindow *) baseWindow;

  /* This test is to check for redundant listings of this window
     in more than one child window list.  The parentWindow should
     either be NULL (the parent defaults to the main BaseWindow)
     or be set by an internal call to the setParentWindow member
     function in the addChildWindow member function (indicating
     that this window is the child of a window other then the
     main BaseWindow)... */
  if (parent != parentWindow) return;

  if (parentWindow)
    owObject = xv_create (parentWindow->getXViewObject (), FRAME_CMD,
                          FRAME_SHOW_RESIZE_CORNER, resizable,
  			  FRAME_SHOW_FOOTER,	  displayFooter,
			  XV_SHOW,		  FALSE,
			  NULL);
  else
    owObject = xv_create (NULL, FRAME_CMD,
                          FRAME_SHOW_RESIZE_CORNER, resizable,
  			  FRAME_SHOW_FOOTER,	  displayFooter,
			  XV_SHOW,		  FALSE,
			  NULL);

  // Store the "this" pointer and the frame (its own XView pointer)
  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  xv_set (owObject, XV_KEY_DATA, UIC_FRAME_KEY, owObject, NULL);

  // Turn off the panel in the command frame
  xv_set (xv_get (owObject, FRAME_CMD_PANEL), WIN_SHOW, FALSE, NULL);

  // Set the XView help data
  setHelpData ();

  if (label) xv_set (owObject, XV_LABEL, label, NULL);

  if (font) {
    ((UIObject *) font)->createObject (this);
    xv_set (owObject, XV_FONT, font->getXViewObject (), NULL);
  }

  if (leftFooter)
    xv_set (owObject, FRAME_LEFT_FOOTER, leftFooter, NULL);

  if (rightFooter)
    xv_set (owObject, FRAME_RIGHT_FOOTER, rightFooter, NULL);

  initializeColorMap (this);

  if (foregroundColor != UIC_NOT_SET)
    xv_set (owObject,
	    WIN_FOREGROUND_COLOR, foregroundColor,
	    NULL);
    
  if (backgroundColor != UIC_NOT_SET)
    xv_set (owObject,
  	    WIN_BACKGROUND_COLOR, backgroundColor,
	    NULL);
    
  if (xLoc != UIC_NOT_SET)
    xv_set (owObject, XV_X, xLoc, NULL);
    
  if (yLoc != UIC_NOT_SET)
    xv_set (owObject, XV_Y, yLoc, NULL);
    
  if ((width != UIC_NOT_SET) && (!fitWidth))
    xv_set (owObject, XV_WIDTH, width, NULL);

  if ((height != UIC_NOT_SET) && (!fitHeight))
    xv_set (owObject, XV_HEIGHT, height, NULL);

  xv_set (owObject, FRAME_CMD_PUSHPIN_IN, pinned, NULL);

  // If the user specified an event handler
  if (eventHandler || useDefaultEventHandlerFlag || eventTargets.getSize () ||
      (acceptDrops && (dropHandler || useDefaultDropHandlerFlag)) ||
      draggable || hotRegions.getSize ()) {
    xv_set (owObject, WIN_EVENT_PROC, &UIWindow::eventProc, NULL);

    int noEvents = events.getSize (); 
    for (int i = 0; i < noEvents; i++) 
      xv_set (owObject, WIN_CONSUME_EVENT, (int) events [i], NULL);
  }

  // If the user specified a done handler...
  if (doneHandler || useDefaultDoneHandlerFlag)
    xv_set (owObject, FRAME_DONE_PROC, &PopUpWindow::doneProc, NULL);
    
  int num_displays = displayList.getSize ();

  // Loop through the displays added to the window
  int i;
  for (i=0; i < num_displays; i++)
    // Create the XView object for each display
    ((UIObject *) displayList [i])->createObject (this);

  if (fitWidth)
    window_fit_width (owObject);

  if (fitHeight)
    window_fit_height (owObject);

  int num_child_windows = childWindows.getSize ();

  // Loop through the child windows associated with this window
  for (i=0; i < num_child_windows; i++)
    // Create the XView object for each window
    ((UIObject *) childWindows [i])->createObject (this);

  xv_set (owObject, XV_SHOW, showFlag, NULL);

  setUIWindowAttributes (parent);
}

#ifdef TEST
#endif /* TEST */
