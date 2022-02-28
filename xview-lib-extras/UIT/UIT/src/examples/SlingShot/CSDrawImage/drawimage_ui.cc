#include "BaseWindow.h"
#include "CanvasShellDisplay.h"
#include "CSDrawImage.h"
#include "Notifier.h"

extern void drag_handler (UIObject *);

int main (int argc, char **argv)
{
  BaseWindow window1;
  window1.initUI (argc, argv);
  window1.setWidth (400);
  window1.setHeight (150);
  window1.setLabel ("Try dragging the image");
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

  Image image ("./cmdtool.icon");

  CSDrawImage drawimage;
  drawimage.setSelectedImage (image);
  drawimage.setNotSelectedImage (image);
  drawimage.setX (40);
  drawimage.setY (20);
  drawimage.setDraggable (TRUE);
  drawimage.setDragHandler (drag_handler);
  drawimage.setDragCursor (image);
  controls1.addComponent (drawimage);

  Notifier notifier;
  notifier.start ();

  exit (0);
}
