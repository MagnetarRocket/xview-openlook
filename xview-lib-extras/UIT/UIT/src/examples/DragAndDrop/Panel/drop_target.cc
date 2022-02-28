/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "TextItem.h"
#include "Notifier.h"

void drop_handler (UIObject *);

main (int argc, char **argv)
{
  BaseWindow drop_bw;
  drop_bw.initUI (argc, argv);
  drop_bw.setWidth (220);
  drop_bw.setHeight (60);
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

  TextItem drop_text_item;
  drop_text_item.setX (16);
  drop_text_item.setY (24);
  drop_text_item.setLabel ("Drop Here");
  drop_text_item.setAcceptDrops (TRUE);
  drop_text_item.setDropHandler (drop_handler);
  drop_cd.addComponent (drop_text_item);

  Notifier notifier;
  notifier.start ();

  exit (0);
}

void drop_handler (UIObject *object)
{
  char *message = object->getDropMessage ();

  char *text = (message ? message : "No drop message");
  object->setLabel (text);

  if (message) delete message;
}
