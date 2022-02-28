/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "CommandMenu.h"
#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "Button.h"
#include "PopUpWindow.h"
#include "Notifier.h"

main (int argc, char **argv)
{
  BaseWindow window1;
  window1.initUI (argc, argv);
  window1.setLabel ("Main Base Window");
  window1.setClosed (FALSE);
  window1.setDisplayFooter (TRUE);
  window1.setResizable (TRUE);
  window1.setFit (WIDTH);
  window1.setFit (HEIGHT);

  CommandMenu popup_window_menu;

  popup_window_menu.setDisplayedColumns (1);
  popup_window_menu.setLabel ("Pop Up Window Menu");
  (void) popup_window_menu.addItem ("Pop Up Window 1...");
  (void) popup_window_menu.addItem ("Pop Up Window 2...");
  (void) popup_window_menu.addItem ("Pop Up Window 3...");
  popup_window_menu.setItemColor (1, "Aquamarine");
  popup_window_menu.setItemColor (2, "Blue");
  popup_window_menu.setItemColor (3, "Coral");
  popup_window_menu.setPinnable (TRUE);

  ComponentDisplay controls1 (TRUE);
  controls1.setX (0);
  controls1.setY (0);
  controls1.setFit (WIDTH);
  controls1.setFit (HEIGHT);
  controls1.setDisplayBorders (FALSE);
  window1.addDisplay (controls1);

  Button button1;
  button1.setX (28);
  button1.setY (16);
  button1.setWidth (165);
  button1.setHeight (19);
  button1.setLabel ("Auxillary Base Window...");
  controls1.addComponent (button1);

  BaseWindow window2;
  window2.setWidth (200);
  window2.setHeight (100);
  window2.setLabel ("Auxillary Base Window");
  window2.setClosed (FALSE);
  window2.setDisplayFooter (TRUE);
  window2.setResizable (TRUE);
  window2.setFit (WIDTH);
  window2.setFit (HEIGHT);
  window1.addChildWindow (window2);

  ComponentDisplay controls2 (TRUE);
  controls2.setX (0);
  controls2.setY (0);
  controls2.setFit (WIDTH);
  controls2.setFit (HEIGHT);
  controls2.setDisplayBorders (FALSE);
  window2.addDisplay (controls2);

  Button button2;
  button2.setX (28);
  button2.setY (16);
  button2.setWidth (134);
  button2.setHeight (19);
  button2.setLabel ("Pop up Windows");
  button2.setMenu (popup_window_menu);
  controls2.addComponent (button2);

  PopUpWindow popup1;
  popup1.setWidth (400);
  popup1.setHeight (150);
  popup1.setLabel ("Popup Window 1");
  popup1.setDisplayFooter (TRUE);
  popup1.setResizable (TRUE);
  popup1.setPinned (FALSE);
  popup1.show (FALSE);
  window2.addChildWindow (popup1);

  PopUpWindow popup2;
  popup2.setWidth (400);
  popup2.setHeight (150);
  popup2.setLabel ("Popup Window 2");
  popup2.setDisplayFooter (TRUE);
  popup2.setResizable (TRUE);
  popup2.setPinned (FALSE);
  popup2.show (FALSE);
  window2.addChildWindow (popup2);

  PopUpWindow popup3;
  popup3.setWidth (400);
  popup3.setHeight (150);
  popup3.setLabel ("Popup Window 3");
  popup3.setDisplayFooter (TRUE);
  popup3.setResizable (TRUE);
  popup3.setPinned (FALSE);
  popup3.show (FALSE);
  window2.addChildWindow (popup3);

  button1.setWindow (window2);

  popup_window_menu.setItemWindow (1, popup1);
  popup_window_menu.setItemWindow (2, popup2);
  popup_window_menu.setItemWindow (3, popup3);

  Notifier notifier;

  notifier.createXViewObjects ();

  int xLeft = window2.getX ();
  int xRight = xLeft + window2.getWidth () + 10; // +10 for the frame borders
  int yTop = window2.getY ();
  int yBottom = yTop + window2.getHeight () + 50; // +50 for the frame borders

  popup1.setLocation (xRight, yTop);
  popup2.setLocation (xRight, yBottom);
  popup3.setLocation (xLeft, yBottom);

  notifier.start ();

  exit (0);
}
