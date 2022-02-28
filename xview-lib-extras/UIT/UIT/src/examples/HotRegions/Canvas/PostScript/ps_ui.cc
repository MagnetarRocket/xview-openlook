/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "Notifier.h"
#include "Image.h"
#include "Scroller.h"
#include "HotRegion.h"

extern void CanvasEventHandler (UIObject *);
extern void CanvasRepaintHandler (UIObject *);

main (int argc, char **argv)
{
  BaseWindow window1;
  window1.initUI (argc, argv, UIC_OPEN_PS_CONNECTION, TRUE, NULL);
  window1.setWidth (519);
  window1.setHeight (323);
  window1.setLabel ("Base Window");
  window1.setClosed (FALSE);
  window1.setDisplayFooter (TRUE);
  window1.setResizable (TRUE);

  ComponentDisplay canvas1;
  canvas1.setX (0);
  canvas1.setY (0);
  canvas1.setExtendToEdge (WIDTH);
  canvas1.setExtendToEdge (HEIGHT);
  canvas1.setEventHandler (CanvasEventHandler);
  canvas1.setRepaintHandler (CanvasRepaintHandler);
  canvas1.setPaintHeight (1000);
  canvas1.setPaintWidth (1000);
  canvas1.setDrawingModel (X11); // For X and PS, combined, use the X11 model
  Scroller s1 (VERTICAL);
  canvas1.addComponent (s1);
  Scroller s2 (HORIZONTAL);
  canvas1.addComponent (s2);
  window1.addDisplay (canvas1);

  char *openwinhome = getenv ("OPENWINHOME");
  char *name = "/include/images/cmdtool.icon";
  char *image_name = new char [strlen (openwinhome) + strlen (name) + 1];
  (void) strcat (strcpy (image_name, openwinhome), name);
  Image image (image_name);

  canvas1.setObjectData ("Image", image);

  int x = 300;
  int y = 100;

  canvas1.setObjectData ("X", (Generic *) x);
  canvas1.setObjectData ("Y", (Generic *) y);

  HotRegion hr (1, x, y, 64, 64, TRUE);
  (void) canvas1.addHotRegion (hr);

  Notifier notifier;
  notifier.start ();

  exit (0);
}
