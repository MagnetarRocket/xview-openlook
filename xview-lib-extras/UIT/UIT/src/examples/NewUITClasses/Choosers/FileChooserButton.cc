/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "FileChooserButton.h"

#include <poll.h>

FileChooserButton::FileChooserButton ()
{
  setFileChooserButtonDefaults ();
}

FileChooserButton::FileChooserButton (char *label) : Button (label)
{
  setFileChooserButtonDefaults ();
}

FileChooserButton::FileChooserButton (Image &image) : Button (image)
{
  setFileChooserButtonDefaults ();
}

void FileChooserButton::setFileChooserButtonDefaults ()
{
  useDefaultNotifyHandler (TRUE);

  fcTitle = (char *) NULL;
  fcLeftFooter = (char *) NULL;
  fcRightFooter = (char *) NULL;
  fcButtonLabel = (char *) NULL;
  fcDirectory = getenv ("PWD");
  fcFilePattern = (char *) NULL;
  fcShowDotFiles = FALSE;

  selectionReceived = FALSE;
  selectedDirectory = (char *) NULL;
  selectedFile = (char *) NULL;
}

FileChooserButton::~FileChooserButton ()
{
  if (fcTitle) delete fcTitle;
  if (fcLeftFooter) delete fcLeftFooter;
  if (fcRightFooter) delete fcRightFooter;
  if (fcButtonLabel) delete fcButtonLabel;
  if (fcDirectory) delete fcDirectory;
  if (fcFilePattern) delete fcFilePattern;

  if (selectedDirectory) delete selectedDirectory;
  if (selectedFile) delete selectedFile;
}

void FileChooserButton::setFileChooserTitle (char *title) 
{ 
  if (fcTitle) delete fcTitle;
 
  if (title) 
    fcTitle = strcpy (new char [strlen (title) + 1], title);
  else 
    fcTitle = (char *) NULL; 
} 

void FileChooserButton::setFileChooserLeftFooter (char *footer)
{
  if (fcLeftFooter) delete fcLeftFooter;
 
  if (footer)
    fcLeftFooter = strcpy (new char [strlen (footer) + 1], footer);
  else
    fcLeftFooter = (char *) NULL;
}
 
void FileChooserButton::setFileChooserRightFooter (char *footer)
{
  if (fcRightFooter) delete fcRightFooter;
 
  if (footer)
    fcRightFooter = strcpy (new char [strlen (footer) + 1], footer);
  else
    fcRightFooter = (char *) NULL;
}
 
void FileChooserButton::setFileChooserButtonLabel (char *label) 
{  
  if (fcButtonLabel) delete fcButtonLabel; 
 
  if (label)  
    fcButtonLabel = strcpy (new char [strlen (label) + 1], label); 
  else  
    fcButtonLabel = (char *) NULL;
}  
 
void FileChooserButton::setFileChooserDirectory (char *dir) 
{
  if (fcDirectory) delete fcDirectory; 
  
  if (dir) 
    fcDirectory = strcpy (new char [strlen (dir) + 1], dir); 
  else 
    fcDirectory = getenv ("PWD");
} 

void FileChooserButton::setFileChooserFilePattern (char *pattern) 
{ 
  if (fcFilePattern) delete fcFilePattern; 
   
  if (pattern) 
    fcFilePattern = strcpy (new char [strlen (pattern) + 1], pattern);  
  else 
    fcFilePattern = (char *) NULL; 
}  
 
void FileChooserButton::setSelectionHandler (UICHandlerFunction handler)
{
  selectionHandler = handler;
}

Boolean FileChooserButton::isSelectionReceived ()
{
  return selectionReceived;
}

char *FileChooserButton::getSelectedDirectory ()
{
  if (selectedDirectory)
    return strcpy (new char [strlen (selectedDirectory) + 1],
		   selectedDirectory);

  return (char *) NULL;
}

char *FileChooserButton::getSelectedFile ()
{
  if (selectedFile)
    return strcpy (new char [strlen (selectedFile) + 1], selectedFile);

  return (char *) NULL;
}

void FileChooserButton::defaultNotifyHandler ()
{
  int          mark;
  Tt_message   msg;
  static char *procid;

  clearSelections ();

  if (!procid)
    procid = tt_open ();
  mark = tt_mark();

  if (tt_default_procid_set (procid) != TT_OK) {
    procid = (char *) NULL;
    tt_release (mark);

    selectionReceived = FALSE;
    if (selectionHandler) selectionHandler ((UIObject *) this);
    return;
  }

  tt_session_join (tt_default_session ());

  msg = tt_prequest_create (TT_SESSION, "file_chooser");
  tt_message_arg_add (msg, TT_IN, "string", fcTitle); 
  tt_message_arg_add (msg, TT_IN, "string", fcLeftFooter); 
  tt_message_arg_add (msg, TT_IN, "string", fcRightFooter); 
  tt_message_arg_add (msg, TT_IN, "string", fcButtonLabel); 
  tt_message_arg_add (msg, TT_IN, "string", fcDirectory); 
  tt_message_arg_add (msg, TT_IN, "string", fcFilePattern); 
  tt_message_iarg_add (msg, TT_IN, "int", fcShowDotFiles); 
  tt_message_arg_add (msg, TT_OUT, "string", NULL); 
  tt_message_arg_add (msg, TT_OUT, "string", NULL); 

  struct pollfd poll_fd;
  poll_fd.fd = tt_fd ();
  poll_fd.events = POLLIN;
 
  Boolean handler_started = FALSE;
  Boolean message_failed = FALSE;
  Boolean reply_received = FALSE;

  (void) tt_message_send (msg); 
  while (!reply_received && !message_failed) {
    if (poll (&poll_fd, 1, -1) > 0) {
      (void) tt_message_receive ();

      switch (tt_message_state (msg)) {
      case TT_HANDLED:
	reply_received = TRUE;
        break;
      case TT_STARTED:
	handler_started = TRUE;
        break;
      case TT_FAILED:
	if (handler_started) {
          tt_message_destroy (msg);

  	  msg = tt_prequest_create (TT_SESSION, "file_chooser");
  	  tt_message_arg_add (msg, TT_IN, "string", fcTitle);
  	  tt_message_arg_add (msg, TT_IN, "string", fcLeftFooter);
  	  tt_message_arg_add (msg, TT_IN, "string", fcRightFooter);
  	  tt_message_arg_add (msg, TT_IN, "string", fcButtonLabel);
  	  tt_message_arg_add (msg, TT_IN, "string", fcDirectory);
  	  tt_message_arg_add (msg, TT_IN, "string", fcFilePattern);
  	  tt_message_iarg_add (msg, TT_IN, "int", fcShowDotFiles);
  	  tt_message_arg_add (msg, TT_OUT, "string", NULL);
  	  tt_message_arg_add (msg, TT_OUT, "string", NULL);

          (void) tt_message_send (msg); 
          handler_started = FALSE;
        }
        else
          message_failed = TRUE;
        break;
      default:
        break;
      }
    }
    else
      message_failed = TRUE;
  }

  if (!message_failed) {
    char *dir = tt_message_arg_val (msg, 7);
    char *file = tt_message_arg_val (msg, 8);

    if (dir && strlen (dir)) {
      selectedDirectory = strcpy (new char [strlen (dir) + 1], dir);
      fcDirectory = strcpy (new char [strlen (dir) + 1], dir);
    }

    if (file && strlen (file))
      selectedFile = strcpy (new char [strlen (file) + 1], file);
  }

  if (message_failed || (!selectedDirectory && !selectedFile))
    selectionReceived = FALSE;
  else
    selectionReceived = TRUE;

  tt_message_destroy (msg);
  tt_release (mark);

  if (selectionHandler) selectionHandler ((UIObject *) this);
}

void FileChooserButton::clearSelections ()
{
  if (selectedDirectory) {
    delete selectedDirectory;
    selectedDirectory = (char *) NULL;
  }

  if (selectedFile) {
    delete selectedFile;
    selectedFile = (char *) NULL;
  }
}
