/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "Notifier.h"
#include "Scroller.h"

extern void canvas_event_handler (UIObject *);
extern void canvas_repaint_handler (UIObject *);

main (int argc, char **argv)
{
  BaseWindow window1;
  window1.initUI (argc, argv, UIC_OPEN_PS_CONNECTION, TRUE, NULL);
  window1.setWidth (519);
  window1.setHeight (323);
  window1.setLabel ("Circles");
  window1.setClosed (FALSE);
  window1.setDisplayFooter (TRUE);
  window1.setResizable (TRUE);

  ComponentDisplay canvas1;
  canvas1.setX (0);
  canvas1.setY (0);
  canvas1.setExtendToEdge (WIDTH);
  canvas1.setExtendToEdge (HEIGHT);
  canvas1.setPaintHeight (window1.getRootWindowHeight ());
  canvas1.setPaintWidth (window1.getRootWindowWidth ());
  canvas1.setRepaintHandler (canvas_repaint_handler);
  canvas1.setEventHandler (canvas_event_handler);
  Scroller s1 (HORIZONTAL);
  canvas1.addComponent (s1);
  Scroller s2 (VERTICAL);
  canvas1.addComponent (s2);
  canvas1.setAutoExpand (TRUE);
  window1.addDisplay (canvas1);

  GenericList list;
  canvas1.setObjectData ("list", list);

  Notifier notifier;
  notifier.start ();

  list.dropItems ();
  exit (0);
}
