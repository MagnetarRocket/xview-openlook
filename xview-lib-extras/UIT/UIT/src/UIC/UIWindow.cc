/*H+ UIWindow.cc
*
*    SCCS ID: @(#)UIWindow.cc 1.7 93/06/17 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the UIWindow class.
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
static char sccsid [] = "@(#)UIWindow.cc 1.7 93/06/17 SMI"; 
#endif

// Includes
#include "UIWindow.h"

// Static member initialization
GenericList *UIWindow::peerWindows = (GenericList *) NULL;

/*F+ UIWindow::UIWindow ()
*
*    PURPOSE:
*
*    UIWindow constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIWindow::UIWindow ()
{
  setUIWindowDefaults ();
}

/*F+ UIWindow::UIWindow (char *string_label)
*
*    PURPOSE:
*
*    UIWindow constructor.
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
UIWindow::UIWindow (char *string_label) : UIObject (string_label)
{
  setUIWindowDefaults ();
}

/*F+ void UIWindow::setUIWindowDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the UIWindow class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIWindow::setUIWindowDefaults ()
{
  foregroundColor = UIC_NOT_SET;
  backgroundColor = UIC_NOT_SET;
  help = (char *) NULL;
  displayFooter = TRUE;
  doneHandler = (UICHandlerFunction) NULL;
  leftFooter = (char *) NULL;
  rightFooter = (char *) NULL;
  fitHeight = FALSE;
  fitWidth = FALSE;
  parentWindow = (UIWindow *) NULL;
  useDefaultDoneHandlerFlag = FALSE;
  newXParent = (XID) UIC_NOT_SET;
  newXParentX = UIC_NOT_SET;
  newXParentY = UIC_NOT_SET;
  
  if (!peerWindows)
    peerWindows = new GenericList;
  peerWindows->addItem ((Generic *) this);
}

/*F+ UIWindow::~UIWindow ()
*
*    PURPOSE:
*
*    UIWindow destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIWindow::~UIWindow ()
{
  if (leftFooter) delete leftFooter;
  if (rightFooter) delete rightFooter;

  // If this is the main base window...
  if (baseWindow == this) {
    // Delete the peer windows list
    peerWindows->dropItems ();
    delete peerWindows;
  }

  if (!destroyChildren) {
    childWindows.dropItems ();
    displayList.dropItems ();
  }
  else {
    UIObject *child;

    Boolean reset_flag = TRUE;
    while (child = (UIObject *) childWindows.traverse (reset_flag)) {
      reset_flag = FALSE;
      child->setDestroyChildren (TRUE);
    }
    childWindows.clear ();

    reset_flag = TRUE;
    while (child = (UIObject *) displayList.traverse (reset_flag)) {
      reset_flag = FALSE;
      child->setDestroyChildren (TRUE);
    }
    displayList.clear ();
  }
}

/*F+ void UIWindow::setDoneHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Specified the done handler for the window.
*
*    PARAMETERS:
*
*    handler - The handler funtion.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIWindow::setDoneHandler (UICHandlerFunction handler)
{
  doneHandler = handler;
}

/*F+ void UIWindow::eventProc (Xv_window win, Event *event)
*
*    PURPOSE:
*
*    The XView window event proc.  This is called whenever an
*    event occurs on a window object.
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
void UIWindow::eventProc (Xv_window win, Event *event)
{
  UIWindow *object = (UIWindow *) xv_get (win, XV_KEY_DATA, UIC_OBJECT_KEY);
  
  // Set the UIC event information
  object->setEventInformation (event);
  
  // Call the user specified event handler
  if (object->useDefaultEventHandlerFlag)  
    object->defaultEventHandler ();   
  else if (object->eventHandler)
    (*object->eventHandler) (object);
  
  // Erase the UIC event information
  object->resetEventInformation ();
}

#ifdef TEST
#endif /* TEST */
