/*H+ FileChooser.cc
*
*    SCCS ID: @(#)FileChooser.cc 1.2 93/04/09 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the FileChooser class.
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
static char sccsid [] = "@(#)FileChooser.cc 1.2 93/04/09 SMI"; 
#endif

// Includes
#include "BaseWindow.h"
#include "CharacterFont.h"

#if (XV_VERSION_NUMBER >= 3200)
#include "FileChooser.h"

/*F+ FileChooser::FileChooser ()
*
*    PURPOSE:
*
*    FileChooser constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
FileChooser::FileChooser ()
{
  setFileChooserDefaults ();
}

/*F+ FileChooser::FileChooser (char *string_label)
*
*    PURPOSE:
*
*    FileChooser constructor.
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
FileChooser::FileChooser (char *string_label) : PopUpWindow (string_label)
{
  setFileChooserDefaults ();
}

/*F+ void FileChooser::setFileChooserDefaults ()
*
*    PURPOSE:
*
*    Initializes the data memeber of the FileChooser.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void FileChooser::setFileChooserDefaults ()
{
  type = (File_chooser_type) UIC_NOT_SET;
  defaultSaveAsName = (char *) NULL;
  saveToDirectory = FALSE;
  saveToDirectorySet = FALSE;
  filterString = (char *) NULL;
  showDotFiles = FALSE;
  showDotFilesSet = FALSE;
  chosenPath = (char *) NULL;
  chosenFile = (char *) NULL;
  notifyHandler = (UICHandlerFunction) NULL;
  notifyHandlerSet = FALSE;
  useDefaultNotifyHandlerFlag = FALSE;
}

/*F+ FileChooser::~FileChooser ()
*
*    PURPOSE:
*
*    FileChooser destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
FileChooser::~FileChooser ()
{
  if (defaultSaveAsName) delete defaultSaveAsName;
  if (filterString) delete filterString;
  if (chosenPath) delete chosenPath;
  if (chosenFile) delete chosenFile;
}

/*F+ void FileChooser::notifyProc (File_chooser  fc,
*                                  char         *path,
*                                  char         *file,
*                                  Xv_opaque     client_data)
*
*    PURPOSE:
*
*    Internal XView event proc wrapper.  This is called whenever the
*    user has selected a file from the file chooser.
*
*    PARAMETERS:
*
*    fc - The file chooser
*    path - The selected path
*    file - The selected file
*    client_data - Data associated with the selected file.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void FileChooser::notifyProc (File_chooser  fc,
                              char         *path,
                              char         *file,
                              Xv_opaque     client_data)
{
  FileChooser *object = (FileChooser *)
		        xv_get (fc, XV_KEY_DATA, UIC_OBJECT_KEY);

  if (object->chosenPath) delete object->chosenPath;
  object-> chosenPath = (path ? strcpy (new char [strlen (path) + 1], path) :
			        (char *) NULL);

  if (object->chosenFile) delete object->chosenFile;
  object->chosenFile = (file ? strcpy (new char [strlen (file) + 1], file) :
			       (char *) NULL);
  
  // Call the user specified event handler
  if (object->useDefaultNotifyHandlerFlag)
    object->defaultNotifyHandler ();
  else if (object->notifyHandler)
    (*object->notifyHandler) (object);
}

/*F+ void FileChooser::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The FileChooser window parent (a class derived from UIWindow).
*             If no parent is specified, the default is the main base
*             window.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void FileChooser::createObject (UIObject *parent)
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

  if (type == (File_chooser_type)UIC_NOT_SET)
    type = FILE_CHOOSER_OPEN;

  if (parentWindow)
    owObject = xv_create (parentWindow->getXViewObject (), FILE_CHOOSER,
			  FILE_CHOOSER_TYPE, type,
			  XV_SHOW,           FALSE,
			  NULL);
  else
    owObject = xv_create (NULL, FILE_CHOOSER,
			  FILE_CHOOSER_TYPE, type, 
			  XV_SHOW,           FALSE,
			  NULL);

  // Store the "this" pointer and the frame (its own XView pointer)
  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  xv_set (owObject, XV_KEY_DATA, UIC_FRAME_KEY, owObject, NULL);

  // Set the XView help data
  setHelpData ();

  if (notifyHandler) {
    notifyHandlerSet = TRUE;
    xv_set (owObject,
	    FILE_CHOOSER_NOTIFY_FUNC, &FileChooser::notifyProc,
	    NULL);
  }

  if (defaultSaveAsName)
    xv_set (owObject, FILE_CHOOSER_DOC_NAME, defaultSaveAsName, NULL);

  if (saveToDirectorySet)
    xv_set (owObject, FILE_CHOOSER_SAVE_TO_DIR, saveToDirectory, NULL);

  if (filterString)
    xv_set (owObject, FILE_CHOOSER_FILTER_STRING, filterString, NULL);

  if (showDotFilesSet)
    xv_set (owObject, FILE_CHOOSER_SHOW_DOT_FILES, showDotFiles, NULL);

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
    xv_set (owObject, FRAME_DONE_PROC, &FileChooser::doneProc, NULL);
    
  xv_set (owObject, XV_SHOW, showFlag, NULL);

  setUIWindowAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* XV_VERSION_NUMBER */
