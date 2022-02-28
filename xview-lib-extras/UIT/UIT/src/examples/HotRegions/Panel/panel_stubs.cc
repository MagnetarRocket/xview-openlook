/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "ComponentDisplay.h"
#include "Image.h"
#include "HotRegion.h"

void CanvasEventHandler (UIObject *object)
{
  InputEvent *event = object->getEvent ();

  if ((event->getAction () == ACTION_SELECT) && event->isButtonDown ()) {
    if (object->isHotRegionEvent ()) {
      HotRegion *hr = object->getActiveHotRegion (1);

      if (hr->getID () == 1)
	printf ("Cmdtool Pressed...\n");
      else
	printf ("Blue Arc Pressed...\n");
    }
  }
}

void CanvasRepaintHandler (UIObject *object)
{
  static Boolean  initialized = FALSE;

  Image *image = (Image *) object->getObjectData ("Image");
  ComponentDisplay *cd = (ComponentDisplay *) object;

  if (!initialized)
    cd->storePSImageMaskProc ("draw_cmdtool_image", *image, FALSE);

  cd->drawXImagePlane (*image, 275, 275);

  cd->setPSFocus ();
  cd->setPSColor (0.0, 0.0, 1.0);
  POSTSCRIPT_STRING ("300 100 draw_cmdtool_image ");
  ps_flush_PostScript ();

  cd->setPSColor (1.0, 0.0, 1.0);
  POSTSCRIPT_STRING ("5 rotate ");
  POSTSCRIPT_STRING ("175 100 draw_cmdtool_image ");
  POSTSCRIPT_STRING ("-5 rotate ");

  cd->setPSColor (0.0, 0.0, 1.0);
  POSTSCRIPT_STRING ("newpath ");
  POSTSCRIPT_STRING ("0 300 50 270 360 arc ");
  POSTSCRIPT_STRING ("0 300 90 360 270 arcn ");
  POSTSCRIPT_STRING ("closepath ");
  POSTSCRIPT_STRING ("fill ");
  cd->setPSColor (1.0, 0.0, 0.0);
  POSTSCRIPT_STRING ("newpath ");
  POSTSCRIPT_STRING ("0 300 50 0 90 arc ");
  POSTSCRIPT_STRING ("0 300 90 90 0 arcn ");
  POSTSCRIPT_STRING ("closepath ");
  POSTSCRIPT_STRING ("fill ");
  ps_flush_PostScript ();
}

void ButtonHandler (UIObject *object)
{
  printf ("Button pressed...\n");
  return;
}
