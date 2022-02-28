/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "AlphanumericInput.h"
#include "Bullseye.h"
#include "Image.h"
#include "Notifier.h"

void drop_handler (UIObject *);
void drag_handler (UIObject *);

main (int argc, char **argv)
{
  BaseWindow drop_bw;
  drop_bw.initUI (argc, argv);
  drop_bw.setWidth (300);
  drop_bw.setHeight (120);
  drop_bw.setLabel ("Drop Target");
  drop_bw.show(TRUE);
  drop_bw.setDisplayFooter (TRUE);
  drop_bw.setResizable (TRUE);

  ComponentDisplay drop_cd (TRUE);
  drop_cd.setX (0);
  drop_cd.setY (0);
  drop_cd.setExtendToEdge (WIDTH);
  drop_cd.setExtendToEdge (HEIGHT);
  drop_cd.setDisplayBorders (FALSE);
  drop_bw.addDisplay (drop_cd);

  AlphanumericInput drop_text_field;
  drop_text_field.setUnderline (TRUE);
  drop_text_field.setX (10);
  drop_text_field.setY (10);
  drop_text_field.setInputDisplayLength (20);
  drop_text_field.setMaxInputLength (80);
  drop_text_field.setLabel ("Drop Message:");
  drop_text_field.setLabelPosition (LEFT);
  drop_text_field.setReadOnly (TRUE);
  drop_cd.addComponent (drop_text_field);

  Image arrow_image ("./arrow.bm");
  arrow_image.setBitMapFile (TRUE);

  Bullseye bullseye;
  bullseye.setX (10);
  bullseye.setY (50);
  bullseye.setDropHandler (drop_handler);
  bullseye.setDragCursor (arrow_image, 55, 9);
  bullseye.setAcceptDropCursor ("./drop.icon", 32, 32);
  bullseye.setDraggable (TRUE);
  bullseye.setDragHandler (drag_handler);
  drop_cd.addComponent (bullseye);

  bullseye.setObjectData ("DROP_TEXT_FIELD", drop_text_field);

  Notifier notifier;
  notifier.start ();

  exit (0);
}

void drop_handler (UIObject *object)
{
  char *message = object->getDropMessage ();

  char *text = (message ? message : "");
  AlphanumericInput *text_field = (AlphanumericInput *)
			          object->getObjectData ("DROP_TEXT_FIELD");
  text_field->setValue (text);

  if (message) delete message;
}

void drag_handler (UIObject *object)
{
  if (object->getDragStatus () == DRAG_START) {
    AlphanumericInput *text_field = (AlphanumericInput *)
			            object->getObjectData ("DROP_TEXT_FIELD");

    char *value = text_field->getValue (value);
    char *message = (value ? value : (char *) NULL);

    object->setDragMessage (message);

    if (value) delete value;
  }
}
