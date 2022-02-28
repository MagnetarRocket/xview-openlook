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
#include "ToggleChoice.h"
#include "Notifier.h"

main (int argc, char **argv)
{
  BaseWindow bw;
  bw.initUI (argc, argv);
  bw.setLabel ("Display Specified Images");
  bw.setFit (WIDTH);
  bw.setFit (HEIGHT);

  if (argc < 2) {
    printf ("Usage: choice image_file_name [image_file_name...]\n");
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

  ToggleChoice image_choice;
  image_choice.setX (10);
  image_choice.setY (10);
  for (i=1; i < argc; i++)
    image_choice.addChoice (*images [i-1]);
  controls.addComponent (image_choice);

  Notifier notifier;
  notifier.start ();

  exit (0);
}
