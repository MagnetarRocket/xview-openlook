/*H+ TextDisplay.cc
*
*    SCCS ID: @(#)TextDisplay.cc 1.5 93/04/09 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the TextDisplay class.
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
static char sccsid [] = "@(#)TextDisplay.cc 1.5 93/04/09 SMI"; 
#endif

// Includes
#include "CharacterFont.h"
#include "TextDisplay.h"

/*F+ TextDisplay::TextDisplay ()
*
*    PURPOSE:
*
*    TextDisplay constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TextDisplay::TextDisplay ()
{
  file = (char *) NULL;
  readOnly = FALSE;
  ignoreEdits = FALSE;
  modified = FALSE;
}

/*F+ TextDisplay::~TextDisplay ()
*
*    PURPOSE:
*
*    TextDisplay destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TextDisplay::~TextDisplay ()
{
  if (file)
    delete file;
}

/*F+ TextDisplayStatus TextDisplay::loadFile (char *file_name)
*
*    PURPOSE:
*
*    Loads the specified file into the TextDisplay.
*
*    RETURN VALUES:
*
*    Returns the status of the TextDisplay after the attempted file load.
*    (See TEXTSW_STATUS in the XView manual).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TextDisplayStatus TextDisplay::loadFile (char *file_name)
{
  loadStatus = TEXTSW_STATUS_OKAY;
    
  // Make a copy of the file name
  if (file) delete file;
  file = strcpy (new char [strlen (file_name) + 1], file_name);

  // If the XView object exists...
  if (owObject)
    xv_set (owObject,
	    TEXTSW_STATUS, &loadStatus,
	    TEXTSW_FILE,   file,
	    TEXTSW_FIRST,  0,
	    NULL);

  return (loadStatus);
}

/*F+ void TextDisplay::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the TextDisplay XView object.
*
*    PARAMETER:
*
*    parent - The parent of the TextDisplay.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void TextDisplay::createObject (UIObject *parent)
{
  // If a font is specified...
  if (font) {
    // Create the XView font object
    ((UIObject *) font)->createObject (parent);

    if (border)
      owObject = xv_create (parent->getXViewObject (), TEXTSW, 
			    OPENWIN_SHOW_BORDERS,      TRUE,
			    XV_FONT, font->getXViewObject (),
			    NULL);
    else
      owObject = xv_create (parent->getXViewObject (), TEXTSW,
			    XV_FONT, font->getXViewObject (),
			    NULL);
  }
  // Else, no font specified...
  else {
    if (border)
      owObject = xv_create (parent->getXViewObject (), TEXTSW, 
			    OPENWIN_SHOW_BORDERS,      TRUE,
			    NULL);
    else
      owObject = xv_create (parent->getXViewObject (), TEXTSW, NULL);
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
 
  if (readOnly)
    xv_set (owObject, TEXTSW_BROWSING, TRUE, NULL);
  
  if (!ignoreEdits)
    xv_set (owObject, TEXTSW_IGNORE_LIMIT, 0, NULL);
  else
    xv_set (owObject, TEXTSW_IGNORE_LIMIT, TEXTSW_INFINITY, NULL);
  
  if (file != (char *) NULL)
    xv_set (owObject,
	    TEXTSW_STATUS, &loadStatus,
	    TEXTSW_FILE,   file,
	    TEXTSW_FIRST,  1,
	    NULL);
  
  // If the user specified an event handler...
  if (eventHandler || useDefaultEventHandlerFlag || eventTargets.getSize () ||
      (acceptDrops && (dropHandler || useDefaultDropHandlerFlag)) ||
      draggable || hotRegions.getSize ()) {
    xv_set (owObject, WIN_EVENT_PROC, &UIDisplay::eventProc, NULL);
    xv_set (textsw_first (owObject),
	    WIN_EVENT_PROC, &UIDisplay::eventProc,
	    NULL);

    int noEvents = events.getSize (); 
    for (int i = 0; i < noEvents; i++) 
      xv_set (textsw_first (owObject),
	      WIN_CONSUME_EVENT, (int) events [i],
	      NULL);
  }
  
  xv_set (owObject, XV_SHOW, showFlag, NULL);
 
  setUIDisplayAttributes (parent); 
}
       
#ifdef TEST
#endif /* TEST */
