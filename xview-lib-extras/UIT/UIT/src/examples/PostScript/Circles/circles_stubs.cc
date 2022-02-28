/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "ComponentDisplay.h"

void canvas_repaint_handler (UIObject *object)
{
  GenericList *list = (GenericList *) object->getObjectData ("list");
  int	       num_coords = list->getSize () / 2;

  ((ComponentDisplay *) object)->setPSFocus ();
  ((ComponentDisplay *) object)->setPSGray (0.0);
  for (int i=0; i < num_coords; i++) {
    ps_arc ((int) ((*list) [2 * i]), (int) ((*list) [2 * i + 1]), 50, 0, 360); 
    ps_stroke ();
  }
  ps_flush_PostScript ();

  return;
}

/*
 * Event handler function for `canvas1'.
 */
void canvas_event_handler (UIObject *object)
{
  InputEvent  *event = object->getEvent ();
  GenericList *list = (GenericList *) object->getObjectData ("list");

  if ((event->getAction () == ACTION_SELECT) && event->isButtonUp ()) {
    int x, y;

    ((ComponentDisplay *) object)->setPSFocus ();
    ((ComponentDisplay *) object)->setPSGray (0.0);
    ps_arc (x = event->getX (), y = event->getY (), 50, 0, 360); 
    ps_stroke ();
    ps_flush_PostScript ();

    list->addItem ((Generic *) x);
    list->addItem ((Generic *) y);
  }

  return;
}
