#include "BaseWindow.h"
#include "CanvasShellDisplay.h"
#include "CSDrawText.h"
#include "Notifier.h"

extern void text_handler (UIObject *);

int main (int argc, char **argv)
{
  BaseWindow window1;
  window1.initUI (argc, argv);
  window1.setWidth (400);
  window1.setHeight (150);
  window1.setLabel ("CSDrawText click handlers");
  window1.show (TRUE);
  window1.setDisplayFooter (TRUE);
  window1.setResizable (TRUE);

  CanvasShellDisplay controls1;
  controls1.setX (0);
  controls1.setY (0);
  controls1.setExtendToEdge (WIDTH);
  controls1.setExtendToEdge (HEIGHT);
  controls1.setDisplayBorders (FALSE);
  window1.addDisplay (controls1);

  CSDrawText textfield1 ("textfield1: double click here");
  textfield1.setX (40);
  textfield1.setY (20);
  textfield1.setDoubleClickHandler (text_handler);
  controls1.addComponent (textfield1);

  CSDrawText textfield2 ("textfield2: single click here");
  textfield2.setX (40);
  textfield2.setY (60);
  textfield2.setSingleClickHandler (text_handler);
  controls1.addComponent (textfield2);

  Notifier notifier;
  notifier.start ();

  exit (0);
}
