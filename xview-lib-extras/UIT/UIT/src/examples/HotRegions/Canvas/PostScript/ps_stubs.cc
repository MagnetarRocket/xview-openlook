/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "ComponentDisplay.h"
#include "Image.h"

void CanvasRepaintHandler (UIObject *object)
{
  ComponentDisplay *cd = (ComponentDisplay *) object;

  static char    *draw_ps_image;
  static Boolean  initialized = FALSE;

  if (!initialized) {
    initialized = TRUE;

    Image *image = (Image *) cd->getObjectData ("Image");
    int    x = (int) object->getObjectData ("X");
    int    y = (int) object->getObjectData ("Y");

    cd->storePSImageMaskProc ("draw_cmdtool_image", *image, TRUE);

    draw_ps_image = new char [35];
    sprintf (draw_ps_image, "%d %d draw_cmdtool_image ", x, y);
  }

  cd->setPSFocus ();
  cd->setPSColor (0.0, 0.0, 1.0);
  POSTSCRIPT_STRING (draw_ps_image);
  ps_flush_PostScript ();

  return;
}

/*
 * Event handler function for `canvas1'.
 */
void CanvasEventHandler (UIObject *object)
{
  InputEvent *event;

  event = object->getEvent ();

  if (event->getAction () == ACTION_SELECT) {
    if (object->isHotRegionEvent ())
      printf ("CMDTOOL pressed...\n");
  }

  return;
}

/*
 * Notify handler function for `button1'.
 */
void ButtonHandler (UIObject *object)
{
  printf ("Button pressed...\n");
  return;
}
