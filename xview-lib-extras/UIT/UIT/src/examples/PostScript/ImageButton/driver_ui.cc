/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "Button.h"
#include "ImageButton.h"
#include "Notifier.h"

extern void b_handler (UIObject *);
extern void ib_handler (UIObject *);

main (int argc, char **argv)
{
  BaseWindow window1;
  window1.initUI (argc, argv, UIC_OPEN_PS_CONNECTION, TRUE, NULL);
  window1.setWidth (504);
  window1.setHeight (314);
  window1.setLabel ("Image Button Demo");
  window1.setClosed (FALSE);
  window1.setDisplayFooter (TRUE);
  window1.setResizable (TRUE);

  ComponentDisplay controls1 (TRUE);
  controls1.setX (0);
  controls1.setY (0);
  controls1.setExtendToEdge (WIDTH);
  controls1.setExtendToEdge (HEIGHT);
  controls1.setDisplayBorders (FALSE);
  window1.addDisplay (controls1);

  char *openwinhome = getenv ("OPENWINHOME");
  char *name = "/include/images/bind.icon";
  char *image_name = new char [strlen (openwinhome) + strlen (name) + 1];
  (void) strcat (strcpy (image_name, openwinhome), name);
  Image image (image_name);

  Button b (image);
  b.setX (200);
  b.setY (100);
  b.setNotifyHandler (b_handler);
  controls1.addComponent (b);

  ImageButton ib (image);
  ib.setX (200);
  ib.setY (200);
  ib.setNotifyHandler (ib_handler);
  controls1.addComponent (ib);

  b.setObjectData ("IB", ib);

  Notifier notifier;
  notifier.start ();

  exit (0);
}
