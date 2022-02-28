/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ColorChooserButton.h"
#include "ComponentDisplay.h"
#include "FileChooserButton.h"
#include "AlphanumericInput.h"
#include "Notifier.h"

extern void color_chooser_handler (UIObject *);
extern void file_chooser_handler (UIObject *);

void main (int argc, char **argv)
{
  BaseWindow bw;
  bw.initUI (argc, argv);
  bw.setWidth (346);
  bw.setHeight (200);
  bw.setLabel ("Chooser Demo");
  bw.show (TRUE);
  bw.setDisplayFooter (TRUE);
  bw.setResizable (TRUE);

  ComponentDisplay cd (TRUE);
  cd.setX (0);
  cd.setY (0);
  cd.setExtendToEdge (WIDTH);
  cd.setExtendToEdge (HEIGHT);
  cd.setDisplayBorders (FALSE);
  bw.addDisplay (cd);

  FileChooserButton fcb;
  fcb.setX (16);
  fcb.setY (16);
  fcb.setLabel ("File Chooser...");
  fcb.setFileChooserTitle ("File Chooser Demo");
  fcb.setFileChooserButtonLabel ("Select for demo");
  fcb.setSelectionHandler (file_chooser_handler);
  cd.addComponent (fcb);

  AlphanumericInput dt;
  dt.setX (16);
  dt.setY (56);
  dt.setInputDisplayLength (30);
  dt.setMaxInputLength (256);
  dt.setLabel ("Directory:");
  dt.setLabelPosition (LEFT);
  dt.setReadOnly (TRUE);
  cd.addComponent (dt);

  AlphanumericInput ft;
  ft.setX (54);
  ft.setY (80);
  ft.setInputDisplayLength (30);
  ft.setMaxInputLength (256);
  ft.setLabel ("File:");
  ft.setLabelPosition (LEFT);
  ft.setReadOnly (TRUE);
  cd.addComponent (ft);

  fcb.setObjectData ("DIR", dt);
  fcb.setObjectData ("FILE", ft);
  fcb.setObjectData ("BW", bw);

  ColorChooserButton ccb;
  ccb.setX (16);
  ccb.setY (140);
  ccb.setLabel ("Color Chooser...");
  ccb.setColorChooserTitle ("Color Chooser Demo");
  ccb.setSelectionHandler (color_chooser_handler);
  cd.addComponent (ccb);

  AlphanumericInput ct;
  ct.setX (16);
  ct.setY (180);
  ct.setInputDisplayLength (30);
  ct.setMaxInputLength (256);
  ct.setLabel ("Color:");
  ct.setLabelPosition (LEFT);
  ct.setReadOnly (TRUE);
  ct.setForegroundColor ("Foreground");
  cd.addComponent (ct);

  ccb.setObjectData ("COLOR", ct);
  ccb.setObjectData ("BW", bw);

  Notifier notifier;
  notifier.start ();

  exit (0);
}
