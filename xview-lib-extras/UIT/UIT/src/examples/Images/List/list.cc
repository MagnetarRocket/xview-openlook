/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

// Given a list of icon files to display...

#include "BaseWindow.h"
#include "Image.h"
#include "ComponentDisplay.h"
#include "ListChoice.h"
#include "Button.h"
#include "Notifier.h"

void list_target (UIObject *, UIObject *);

main (int argc, char **argv)
{
  BaseWindow bw;
  bw.initUI (argc, argv);
  bw.setLabel ("Display Specified Images");
  bw.setFit (WIDTH);
  bw.setFit (HEIGHT);

  if (argc < 2) {
    printf ("Usage: list image_file_name [image_file_name...]\n"); 
    exit (1);
  }

  ComponentDisplay controls;
  controls.setX (0);
  controls.setY (0);
  controls.setFit (WIDTH);
  controls.setFit (HEIGHT);
  bw.addDisplay (controls);

  Image **images = new Image* [argc];
  for (int i=1; i < argc; i++) {
    Image *image = new Image (argv [i]);
    images [i-1] = image;
  }

  ListChoice image_choice;
  image_choice.setX (10);
  image_choice.setY (10);
  image_choice.setWidth (380);
  image_choice.setDisplayedRows (5);
  image_choice.setRowHeight (64);
  image_choice.setReadOnly (TRUE);
  image_choice.setSelectionRequired (TRUE);

  for (i=1; i < argc; i++)
    image_choice.addChoice (argv [i], *images [i-1]);
  controls.addComponent (image_choice);

  Button image_button (*images [0]);
  image_button.setX (410);
  image_button.setY (10);
  controls.addComponent (image_button);

  image_choice.addNotifyTarget (image_button, list_target);

  Notifier notifier;
  notifier.start ();

  exit (0);
}

void list_target (UIObject *event_object, UIObject *target_object)
{
  ListChoice *l = (ListChoice *) event_object;

  if (l->getOperation () == SELECT) {
    Image *image;

    image = l->getChoice (image);
    ((Button *) target_object)->setLabel (*image);
  }
}
