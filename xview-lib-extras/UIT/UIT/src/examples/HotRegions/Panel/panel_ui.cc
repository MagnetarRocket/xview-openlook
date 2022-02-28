/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "Notifier.h"
#include "Button.h"
#include "Image.h"
#include "HotRegion.h"

extern void CanvasEventHandler (UIObject *);
extern void CanvasRepaintHandler (UIObject *);
extern void ButtonHandler (UIObject *);

extern Boolean arc_detection_function (HotRegion *, int, int);

main (int argc, char **argv)
{
  BaseWindow window1;
  window1.initUI (argc, argv, UIC_OPEN_PS_CONNECTION, TRUE, NULL);
  window1.setWidth (519);
  window1.setHeight (400);
  window1.setLabel ("Select the blue arc or the un-occluded blue cmdtool icon");
  window1.setClosed (FALSE);
  window1.setDisplayFooter (TRUE);
  window1.setResizable (TRUE);

  ComponentDisplay canvas1 (TRUE);
  canvas1.setX (0);
  canvas1.setY (0);
  canvas1.setExtendToEdge (WIDTH);
  canvas1.setExtendToEdge (HEIGHT);
  canvas1.setEventHandler (CanvasEventHandler);
  canvas1.setRepaintHandler (CanvasRepaintHandler);
  window1.addDisplay (canvas1);

  char path [128];
  (void) strcpy (path, getenv ("OPENWINHOME"));
  (void) strcat (path, "/include/images/cmdtool.icon");
  Image image (path);

  Button b;
  b.setLabel (image);
  b.setX (68);
  b.setY (68);
  b.setNotifyHandler (ButtonHandler);
  canvas1.addComponent (b);

  Button b2;
  b2.setLabel (image);
  b2.setX (300);
  b2.setY (300);
  b2.setNotifyHandler (ButtonHandler);
  canvas1.addComponent (b2);

  canvas1.setObjectData ("Image", image);

  HotRegion hr1 (1, 300, 100, 64, 64, TRUE);
  canvas1.addHotRegion (hr1);

  HotRegion hr2 (2);
  hr2.setDetectionFunction (arc_detection_function);
  canvas1.addHotRegion (hr2);

  Notifier notifier;
  notifier.start ();

  exit (0);
}

Boolean arc_detection_function (HotRegion *hot_region, int x, int y)
{
  const int r12 = 2500;
  const int r22 = 8100;

  int x2  = x * x;
  int y2  = (y - 300) * (y - 300);

  if (((x2 + y2 >= r12) && (x2 + y2 <= r22)) &&
      (x >= 0) && (y <= 300))
    return TRUE;

  return FALSE;
}
