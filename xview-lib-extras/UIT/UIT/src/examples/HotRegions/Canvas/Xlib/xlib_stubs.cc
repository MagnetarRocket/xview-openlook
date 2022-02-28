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

  Image *image = (Image *) cd->getObjectData ("Image");
  int    x = (int) object->getObjectData ("X");
  int    y = (int) object->getObjectData ("Y");

  cd->drawXImagePlane (*image, x, y);
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
}
