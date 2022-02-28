/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

// Given a list of color names to display...

#include "BaseWindow.h"
#include "ColorMap.h"
#include "ComponentDisplay.h"
#include "ToggleChoice.h"
#include "Notifier.h"

void display_color (UIObject *, UIObject *);
void set_display_width (UIObject *, UIObject *);

main (int argc, char **argv)
{
  BaseWindow bw;
  bw.initUI (argc, argv);
  bw.setLabel ("Display Specified Colors");
  bw.setFit (WIDTH);
  bw.setHeight (250);

  if (argc < 2) {
    printf ("Usage: display color_name [color_name...]\n");
    exit (1);
  }

  ComponentDisplay controls;
  controls.setX (0);
  controls.setY (0);
  controls.setHeight (50);
  controls.setFit (WIDTH);
  controls.addEventTarget (bw, set_display_width);
  bw.addDisplay (controls);

  ComponentDisplay canvas;
  canvas.setX (0);
  canvas.setY (50);
  canvas.setExtendToEdge (WIDTH);
  canvas.setExtendToEdge (HEIGHT);
  bw.addDisplay (canvas);

  ColorMap color_map;
  for (int i=1; i < argc; i++)
    color_map.addColor (argv [i]);
  bw.setColorMap (color_map);

  ToggleChoice color_choice;
  color_choice.setX (10);
  color_choice.setY (10);
  for (i=1; i < argc; i++) {
    color_choice.addChoice (argv [i]);
    color_choice.setChoiceColor (i, argv [i]);
  }
  color_choice.addNotifyTarget (canvas, display_color);
  controls.addComponent (color_choice);

  canvas.setBackgroundColor (argv [1]);

  Notifier notifier;
  notifier.start ();

  exit (0);
}

void display_color (UIObject *event_object, UIObject *target_object)
{
  char *value;
  value = event_object->getChoice (value);

  target_object->setBackgroundColor (value);

  delete value;
}

void set_display_width (UIObject *event_object, UIObject *target_object)
{
  static Boolean event_occurred;

  if (!event_occurred) {
    int width = target_object->getWidth ();
    event_object->setWidth (width);

    event_occurred = TRUE;
  }
}
