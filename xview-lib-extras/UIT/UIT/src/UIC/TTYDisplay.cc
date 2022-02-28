/*H+ TTYDisplay.cc
*
*    SCCS ID: @(#)TTYDisplay.cc 1.2 93/04/09 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the TTYDisplay class.
*
*    AUTHOR: Terry Glanfield and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)TTYDisplay.cc 1.2 93/04/09 SMI"; 
#endif

// Includes
#include "CharacterFont.h"
#include "TTYDisplay.h"

/*F+ TTYDisplay::TTYDisplay ()
*
*    PURPOSE:
*
*    TTYDisplay constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TTYDisplay::TTYDisplay ()
{
  setTTYDisplayDefaults ();
}

/*F+ TTYDisplay::TTYDisplay (char *arg, ...)
*
*    PURPOSE:
*
*    TTYDisplay constructor.
*
*    PARAMETERS:
*
*    arg - First argument to be sent to the shell corresponding to
*          this display.  The arguments specified will be executed
*          in the display.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TTYDisplay::TTYDisplay (char *arg, ...)
{
  setTTYDisplayDefaults ();

  va_list      ap;
  char	      *argn;
  int	       num_args = 0;
  GenericList  arg_list;
  
  // Get the first argument from the arg list
  va_start (ap, arg);
  argn = arg;
  
  // While the argument is not NULL
  while (argn) {
    // Copy the argument and add it to a list
    arg_list.addItem ((Generic *) strcpy (new char [strlen (argn) + 1], argn));
    
    // Get the next argument
    argn = va_arg (ap, char *);
  }
  
  // If arguments were specified...
  if ((num_args = arg_list.getSize ()) > 0) {
    // Create an array of strings containing the arguments
    ttyArgs = new char* [num_args+1];
    for (int i=0; i < num_args; i++)
      ttyArgs [i] = (char *) arg_list [i];
    ttyArgs [num_args] = (char *) NULL;
    
    // Clear the list
    arg_list.dropItems ();
  }
  
  va_end (ap);
}

/*F+ void TTYDisplay::setTTYDisplayDefaults ()
*
*    PURPOSE:
*
*    Initializes TTYDisplay data members.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void TTYDisplay::setTTYDisplayDefaults ()
{
  fdToSTDIO = FALSE;
  ttyArgs = (char **) NULL;
}

/*F+ TTYDisplay::~TTYDisplay ()
*
*    PURPOSE:
*
*    TTYDisplay destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TTYDisplay::~TTYDisplay ()
{
  // If shell arguments were specified...
  if (ttyArgs) {
    // Delete the array of string arguments
    for (int i = 0; ttyArgs [i]; i ++)
      delete ttyArgs [i];
    delete ttyArgs;
  }
}

/*F+ void TTYDisplay::createDragSite ()
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
void TTYDisplay::createDragSite ()
{
  if (!dragSite) createXViewDragSite (owObject);
}

/*F+ void TTYisplay::createDropSite ()
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
void TTYDisplay::createDropSite ()
{
  Rect *rectangle;

  if (!dropSite) {
    if ((dropRectangle.r_width > 0) && (dropRectangle.r_height > 0))
      rectangle = &dropRectangle;
    else
      rectangle = (Rect *) xv_get (owObject, XV_RECT);

    createXViewDropSite (owObject, rectangle);
  }
}

/*F+ void TTYDisplay::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the TTYDisplay XView object.
*
*    PARAMETER:
*
*    parent - The parent of the TTYDisplay.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void TTYDisplay::createObject (UIObject *parent)
{
  // If a font is specified...
  if (font) {
    // Create the XView font object
    ((UIObject *) font)->createObject (parent);
    
    // If shell arguments are specified
    if (ttyArgs) {
      if (border)
        owObject = xv_create (parent->getXViewObject (), TTY, 
			      OPENWIN_SHOW_BORDERS,      TRUE, 
			      TTY_ARGV, 		 ttyArgs,
			      WIN_IS_CLIENT_PANE,	 TRUE,
			      XV_FONT, font->getXViewObject (),
			      NULL);
      else
        owObject = xv_create (parent->getXViewObject (), TTY, 
			      TTY_ARGV, 		 ttyArgs,
			      WIN_IS_CLIENT_PANE,	 TRUE,
			      XV_FONT, font->getXViewObject (),
			      NULL);
    }
    // Else, no shell arguments...
    else {
      if (border)
        owObject = xv_create (parent->getXViewObject (), TTY, 
			      TTY_ARGV, 		 TTY_ARGV_DO_NOT_FORK,
			      OPENWIN_SHOW_BORDERS,      TRUE, 
			      WIN_IS_CLIENT_PANE,	 TRUE,
			      XV_FONT, font->getXViewObject (),
			      NULL);
      else
        owObject = xv_create (parent->getXViewObject (), TTY,
			      TTY_ARGV, 		 TTY_ARGV_DO_NOT_FORK,
			      WIN_IS_CLIENT_PANE,	 TRUE,
			      XV_FONT, font->getXViewObject (),
			      NULL);
    }
  }
  // Else, no font specified...
  else {
    // If shell arguments are specified
    if (ttyArgs) {
      if (border)
        owObject = xv_create (parent->getXViewObject (), TTY, 
			      OPENWIN_SHOW_BORDERS,      TRUE, 
			      TTY_ARGV, 		 ttyArgs,
			      NULL);
      else
        owObject = xv_create (parent->getXViewObject (), TTY, 
			      TTY_ARGV, 		 ttyArgs,
			      NULL);
    }
    // Else, no shell arguments
    else {
      if (border)
        owObject = xv_create (parent->getXViewObject (), TTY, 
			      TTY_ARGV, 		 TTY_ARGV_DO_NOT_FORK,
			      OPENWIN_SHOW_BORDERS,      TRUE, 
			      NULL);
      else
        owObject = xv_create (parent->getXViewObject (), TTY,
			      TTY_ARGV, 		 TTY_ARGV_DO_NOT_FORK,
			      NULL);
    }
  }

  if (fdToSTDIO) {
    dup2 ((int) xv_get (owObject, TTY_TTY_FD), 0);
    dup2 ((int) xv_get (owObject, TTY_TTY_FD), 1);
  }

  /* Save the "this" pointer as key data on the TTY object */
  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  
  // Save the XView frame
  xv_set (owObject,
	  XV_KEY_DATA, UIC_FRAME_KEY, parent->getXViewObject (),
	  NULL);
  
  // Set the XView help data
  setHelpData (owObject);
  
  initializeColorMap (this);
  
  if (foregroundColor != UIC_NOT_SET)
    xv_set (owObject,
	    WIN_FOREGROUND_COLOR, foregroundColor,
	    NULL);
  
  if (backgroundColor != UIC_NOT_SET)
    xv_set (owObject,
	    WIN_BACKGROUND_COLOR, backgroundColor,
	    NULL);
  
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
  
  if (extendWidth)
    xv_set (owObject, XV_WIDTH, WIN_EXTEND_TO_EDGE, NULL);
  else if (width != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_WIDTH, width, NULL, NULL);
    else
      xv_set (owObject, XV_WIDTH, width, NULL);
  }
  
  if (extendHeight)
    xv_set (owObject, XV_HEIGHT, WIN_EXTEND_TO_EDGE, NULL);
  else if (height != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_HEIGHT, height, NULL, NULL);
    else
      xv_set (owObject, XV_HEIGHT, height, NULL);
  }
  
  // If the user specified an event handler...
  if (eventHandler || useDefaultEventHandlerFlag || eventTargets.getSize () ||
      (acceptDrops && (dropHandler || useDefaultDropHandlerFlag)) ||
      draggable || hotRegions.getSize ()) {
    xv_set (owObject, WIN_EVENT_PROC, &UIDisplay::eventProc, NULL);
    
    int noEvents = events.getSize (); 
    for (int i = 0; i < noEvents; i++) {
      xv_set (owObject,
	      WIN_CONSUME_EVENT, (int) events [i],
	      NULL);
    }
  }
  
  if (!showFlag) xv_set (owObject, XV_SHOW, FALSE, NULL);
  
  setUIDisplayAttributes (parent); 
}
       
#ifdef TEST
#endif /* TEST */
