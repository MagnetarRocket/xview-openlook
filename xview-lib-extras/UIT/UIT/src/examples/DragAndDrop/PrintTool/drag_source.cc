/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "AlphanumericInput.h"
#include "ArrowHotRegion.h"
#include "Notifier.h"

void drag_handler (UIObject *);
void repaint_handler (UIObject *);

main (int argc, char **argv)
{
  BaseWindow drag_bw;
  drag_bw.initUI (argc, argv);
  drag_bw.setWidth (300);
  drag_bw.setHeight (120);
  drag_bw.setLabel ("PrintTool Drag and Drop Demo");
  drag_bw.show(TRUE);
  drag_bw.setDisplayFooter (TRUE);
  drag_bw.setResizable (TRUE);

  ComponentDisplay text_cd;
  text_cd.setX (0);
  text_cd.setY (0);
  text_cd.setHeight (50);
  text_cd.setExtendToEdge (WIDTH);
  text_cd.setDisplayBorders (FALSE);
  drag_bw.addDisplay (text_cd);

  AlphanumericInput text;
  text.setUnderline (TRUE);
  text.setX (10);
  text.setY (10);
  text.setInputDisplayLength (20);
  text.setMaxInputLength (80);
  text.setLabel ("File Name:");
  text.setLabelPosition (LEFT);
  text.setReadOnly (FALSE);
  text_cd.addComponent (text);

  ComponentDisplay drag_cd;
  drag_cd.setX (0);
  drag_cd.setY (50);
  drag_cd.setExtendToEdge (WIDTH);
  drag_cd.setExtendToEdge (HEIGHT);
  drag_cd.setDisplayBorders (FALSE);
  drag_cd.setRepaintHandler (repaint_handler);
  drag_bw.addDisplay (drag_cd);

  Image arrow_image ("./arrow.bm");
  arrow_image.setBitMapFile (TRUE);

  Image drop_image ("./drop.icon");

  ArrowHotRegion ahr;
  ahr.setDragCursor (arrow_image, 55, 9);
  ahr.setAcceptDropCursor (drop_image, 32, 32);
  ahr.setDragHandler (drag_handler);
  drag_cd.addHotRegion (ahr);

  ahr.setObjectData ("DRAG_IMAGE", arrow_image);
  ahr.setObjectData ("DRAG_CD", drag_cd);

  ahr.setObjectData ("TEXT", text);

  drag_cd.setObjectData ("DRAG_IMAGE", arrow_image);

  Notifier notifier;
  notifier.start ();

  exit (0);
}

void drag_handler (UIObject *object)
{
  static Atom atom_list [1];

  if (object->getDragStatus () == DRAG_START) {
    AlphanumericInput *text = (AlphanumericInput *)
			      object->getObjectData ("TEXT");
    char *value = text->getValue (value);

    char *message = (value ? value : (char *) NULL);
    object->setDragFileName (message);

    if (value) delete value;
  }
  else if (object->getDragStatus () == DRAG_CONVERT) {
    Xv_server server =
              XV_SERVER_FROM_WINDOW (xv_get (object->getDragConvertSelection (),
				             XV_OWNER));
 
    Atom          type;
    UICGeneric    data;
    unsigned long length;
    int           format;
 
    object->getDragConvertData (type, data, length, format);
    printf ("Atom name: %s\n", xv_get (server, SERVER_ATOM_NAME, type));
 
    if (type == xv_get (server,
                        SERVER_ATOM,
                        "_SUN_ALTERNATE_TRANSPORT_METHODS")) {
      atom_list [0] = (Atom) xv_get (server, SERVER_ATOM, "_SUN_ATM_FILE_NAME"); 
      data = (UICGeneric) atom_list;
      length = 1;
      format = sizeof (Atom) * 8;
 
      object->setDragConvertData (type, data, length, format);
    }
    else if (type == xv_get (server,
                             SERVER_ATOM,
                             "_SUN_DATA_LABEL")) {
      length = 0;
      object->setDragConvertData (type, data, length, format);
    }
    else if (type == xv_get (server,
                             SERVER_ATOM,
                             "TARGETS")) {
      atom_list [0] = XA_ATOM;
 
      data = (UICGeneric) atom_list;
      length = 1;
      format = sizeof (Atom) * 8;
 
      object->setDragConvertData (type, data, length, format);
    }
  }
}

void repaint_handler (UIObject *object)
{
  static GC      gc;
  static Boolean gc_created;

  int x, y, w, h;

  ComponentDisplay *cd = (ComponentDisplay *) object;

  Display *display = cd->getXDisplay ();
  Window   window = cd->getXWindow ();

  Image *image = (Image *) cd->getObjectData ("DRAG_IMAGE");

  if (!gc_created) {
    XGCValues gcv;
    gcv.stipple = (Pixmap) image->getXID ();
    gcv.fill_style = FillStippled;

    gc = XCreateGC (display, window,
		    GCStipple | GCForeground | GCBackground | GCFillStyle,
		    &gcv);
    XSetForeground (display, gc, BlackPixel (display, XDefaultScreen(display)));
    XSetBackground (display, gc, WhitePixel (display, XDefaultScreen(display)));

    gc_created = TRUE;
  } 

  w = image->getWidth ();
  h = image->getHeight ();

  x = (cd->getWidth () - w) / 2;
  y = (cd->getHeight () - h) / 2;

  XClearWindow (display, window);
  XSetTSOrigin (display, gc, x, y);
  XFillRectangle (display, window, gc, x, y, w, h);
}
