/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "AlphanumericInput.h"
#include "Notifier.h"

void drag_handler (UIObject *);

main (int argc, char **argv)
{
  BaseWindow drag_bw;
  drag_bw.initUI (argc, argv);
  drag_bw.setWidth (220);
  drag_bw.setHeight (60);
  drag_bw.setLabel ("Drag Source");
  drag_bw.show(TRUE);
  drag_bw.setDisplayFooter (TRUE);
  drag_bw.setResizable (TRUE);

  ComponentDisplay drag_cd (TRUE);
  drag_cd.setX (0);
  drag_cd.setY (0);
  drag_cd.setExtendToEdge (WIDTH);
  drag_cd.setExtendToEdge (HEIGHT);
  drag_cd.setDisplayBorders (FALSE);
  drag_bw.addDisplay (drag_cd);

  AlphanumericInput drag_text_field;
  drag_text_field.setUnderline (TRUE);
  drag_text_field.setX (16);
  drag_text_field.setY (24);
  drag_text_field.setInputDisplayLength (8);
  drag_text_field.setMaxInputLength (80);
  drag_text_field.setLabel ("Drag from here:");
  drag_text_field.setLabelPosition (LEFT);
  drag_text_field.setReadOnly (FALSE);
  drag_text_field.setDraggable (TRUE);
  drag_text_field.setDragCursor ("./text_data.icon", 32, 32);
  drag_text_field.setAcceptDropCursor ("./drop.icon", 32, 32);
  drag_text_field.setDragHandler (drag_handler);
  drag_cd.addComponent (drag_text_field);

  Notifier notifier;
  notifier.start ();

  exit (0);
}

void drag_handler (UIObject *object)
{
  if (object->getDragStatus () == DRAG_START) {
    char *value = object->getValue (value);

    char *message = (value ? value : (char *) NULL);
    object->setDragMessage (message);

    if (value) delete value;
  }
}
