/*H+ TermDisplay.cc
*
*    SCCS ID: @(#)TermDisplay.cc 1.5 93/04/09 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the TermDisplay class.
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
static char sccsid [] = "@(#)TermDisplay.cc 1.5 93/04/09 SMI"; 
#endif

// Includes
#include "CharacterFont.h"
#include "TermDisplay.h"

/*F+ TermDisplay::TermDisplay ()
*
*    PURPOSE:
*
*    TermDisplay constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TermDisplay::TermDisplay ()
{
  ttyArgs = (char **) NULL;
}

/*F+ TermDisplay::TermDisplay (char *arg, ...)
*
*    PURPOSE:
*
*    TermDisplay constructor.
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
TermDisplay::TermDisplay (char *arg, ...)
{
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

/*F+ TermDisplay::~TermDisplay ()
*
*    PURPOSE:
*
*    TermDisplay destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TermDisplay::~TermDisplay ()
{
  // If shell arguments were specified...
  if (ttyArgs) {
    // Delete the array of string arguments
    for (int i = 0; ttyArgs [i]; i ++)
      delete ttyArgs [i];
    delete ttyArgs;
  }
}

/*F+ void TermDisplay::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the TermDisplay XView object.
*
*    PARAMETER:
*
*    parent - The parent of the TermDisplay.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void TermDisplay::createObject (UIObject *parent)
{
  // If a font is specified...
  if (font) {
    // Create the XView font object
    ((UIObject *) font)->createObject (parent);

    // If shell arguments are specified
    if (ttyArgs) {
      if (border)
        owObject = xv_create (parent->getXViewObject (), TERMSW, 
			      OPENWIN_SHOW_BORDERS,      TRUE, 
			      TTY_ARGV, 	         ttyArgs, 
			      WIN_IS_CLIENT_PANE,	 TRUE,
			      XV_FONT, font->getXViewObject (),
			      NULL);
      else
        owObject = xv_create (parent->getXViewObject (), TERMSW, 
			      TTY_ARGV, 	         ttyArgs, 
			      WIN_IS_CLIENT_PANE,	 TRUE,
			      XV_FONT, font->getXViewObject (),
			      NULL);
    }
    // Else, no shell arguments...
    else {
      if (border)
        owObject = xv_create (parent->getXViewObject (), TERMSW, 
			      OPENWIN_SHOW_BORDERS,      TRUE, 
			      WIN_IS_CLIENT_PANE,	 TRUE,
			      XV_FONT, font->getXViewObject (),
			      NULL);
      else
        owObject = xv_create (parent->getXViewObject (), TERMSW,
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
        owObject = xv_create (parent->getXViewObject (), TERMSW, 
			      OPENWIN_SHOW_BORDERS,      TRUE, 
			      TTY_ARGV, 	         ttyArgs, 
			      NULL);
      else
        owObject = xv_create (parent->getXViewObject (), TERMSW, 
			      TTY_ARGV, 	         ttyArgs, 
			      NULL);
    }
    // Else, no shell arguments
    else {
      if (border)
        owObject = xv_create (parent->getXViewObject (), TERMSW, 
			      OPENWIN_SHOW_BORDERS,      TRUE, 
			      NULL);
      else
        owObject = xv_create (parent->getXViewObject (), TERMSW, NULL);
    }
  }
  
  /* Save the "this" pointer as key data on both the TermSW object and
     its first view (textsw_first) */
  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  xv_set (textsw_first (owObject), XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  // Save the XView frame
  xv_set (owObject,
	  XV_KEY_DATA, UIC_FRAME_KEY, parent->getXViewObject (),
	  NULL);
  
  // Set the XView help data
  setHelpData (textsw_first (owObject));
  
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
    xv_set (textsw_first (owObject),
	    WIN_EVENT_PROC, &UIDisplay::eventProc,
	    NULL);

    int noEvents = events.getSize (); 
    for (int i = 0; i < noEvents; i++) {
      xv_set (owObject,
	      WIN_CONSUME_EVENT, (int) events [i],
	      NULL);
      xv_set (textsw_first (owObject),
	      WIN_CONSUME_EVENT, (int) events [i],
	      NULL);
    }
  }

  if (!showFlag) xv_set (owObject, XV_SHOW, FALSE, NULL);
 
  setUIDisplayAttributes (parent); 
}
       
#ifdef TEST
#endif /* TEST */
