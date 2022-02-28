/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "ColorChooserButton.h"

#include <poll.h>

ColorChooserButton::ColorChooserButton ()
{
  setColorChooserButtonDefaults ();
}

ColorChooserButton::ColorChooserButton (char *label) : Button (label)
{
  setColorChooserButtonDefaults ();
}

ColorChooserButton::ColorChooserButton (Image &image) : Button (image)
{
  setColorChooserButtonDefaults ();
}

void ColorChooserButton::setColorChooserButtonDefaults ()
{
  useDefaultNotifyHandler (TRUE);

  ccTitle = (char *) NULL;
  ccLeftFooter = (char *) NULL;
  ccRightFooter = (char *) NULL;
  ccColor = (char *) NULL;

  selectionReceived = FALSE;
  selectedColor = (char *) NULL;
}

ColorChooserButton::~ColorChooserButton ()
{
  if (ccTitle) delete ccTitle;
  if (ccLeftFooter) delete ccLeftFooter;
  if (ccRightFooter) delete ccRightFooter;
  if (ccColor) delete ccColor;

  if (selectedColor) delete selectedColor;
}

void ColorChooserButton::setColorChooserTitle (char *title)
{
  if (ccTitle) delete ccTitle;

  if (title)
    ccTitle = strcpy (new char [strlen (title) + 1], title);
  else
    ccTitle = (char *) NULL;
}

void ColorChooserButton::setColorChooserLeftFooter (char *footer) 
{ 
  if (ccLeftFooter) delete ccLeftFooter;
 
  if (footer) 
    ccLeftFooter = strcpy (new char [strlen (footer) + 1], footer);
  else 
    ccLeftFooter = (char *) NULL; 
} 

void ColorChooserButton::setColorChooserRightFooter (char *footer) 
{ 
  if (ccRightFooter) delete ccRightFooter;
  
  if (footer)  
    ccRightFooter = strcpy (new char [strlen (footer) + 1], footer); 
  else 
    ccRightFooter = (char *) NULL;  
}  

void ColorChooserButton::setColorChooserColor (char *color) 
{ 
  if (ccColor) delete ccColor;
 
  if (color) 
    ccColor = strcpy (new char [strlen (color) + 1], color);
  else 
    ccColor = (char *) NULL; 
} 

void ColorChooserButton::setSelectionHandler (UICHandlerFunction handler)
{
  selectionHandler = handler;
}

Boolean ColorChooserButton::isSelectionReceived ()
{
  return selectionReceived;
}

char *ColorChooserButton::getSelectedColor ()
{
  if (selectedColor)
    return strcpy (new char [strlen (selectedColor) + 1], selectedColor);

  return (char *) NULL;
}

void ColorChooserButton::defaultNotifyHandler ()
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

  msg = tt_prequest_create (TT_SESSION, "color_chooser");
  tt_message_arg_add (msg, TT_IN, "string", ccTitle); 
  tt_message_arg_add (msg, TT_IN, "string", ccLeftFooter); 
  tt_message_arg_add (msg, TT_IN, "string", ccRightFooter); 
  tt_message_arg_add (msg, TT_IN, "string", ccColor); 
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

          msg = tt_prequest_create (TT_SESSION, "color_chooser");
          tt_message_arg_add (msg, TT_IN, "string", ccTitle); 
          tt_message_arg_add (msg, TT_IN, "string", ccLeftFooter); 
          tt_message_arg_add (msg, TT_IN, "string", ccRightFooter); 
          tt_message_arg_add (msg, TT_IN, "string", ccColor); 
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
    char *color = tt_message_arg_val (msg, 4);

    if (color && strlen (color)) {
      selectedColor = strcpy (new char [strlen (color) + 1], color);
      ccColor = strcpy (new char [strlen (color) + 1], color);
    }
  }

  if (message_failed || !selectedColor)
    selectionReceived = FALSE;
  else
    selectionReceived = TRUE;

  tt_message_destroy (msg);
  tt_release (mark);

  if (selectionHandler) selectionHandler ((UIObject *) this);
}

void ColorChooserButton::clearSelections ()
{
  if (selectedColor) {
    delete selectedColor;
    selectedColor = (char *) NULL;
  }
}
