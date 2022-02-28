/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "Slider.h"
#include "ListChoice.h"
#include "AlphanumericInput.h"
#include "Notifier.h"

void list_target (UIObject *, UIObject *);
void slider_target (UIObject *, UIObject *);

main (int argc, char **argv)
{
  BaseWindow window1;
  window1.initUI (argc, argv);
  window1.setWidth (494);
  window1.setHeight (189);
  window1.setLabel ("Base Window");
  window1.setClosed (FALSE);
  window1.setDisplayFooter (TRUE);
  window1.setResizable (TRUE);

  ComponentDisplay controls1 (TRUE);
  controls1.setX (0);
  controls1.setY (0);
  controls1.setExtendToEdge (WIDTH);
  controls1.setExtendToEdge (HEIGHT);
  controls1.setDisplayBorders (FALSE);
  window1.addDisplay (controls1);

  Slider slider1;
  slider1.setX (36);
  slider1.setY (24);
  slider1.setWidth (313);
  slider1.setHeight (20);
  slider1.setValueX (153);
  slider1.setValueY (24);
  slider1.setSliderWidth (100);
  slider1.setLabel ("Slide to a Value:");
  slider1.setLabelPosition (LEFT);
  slider1.setOrientation (HORIZONTAL);
  slider1.setDisplayEndBoxes (FALSE);
  slider1.setDisplayRange (TRUE);
  slider1.setDisplayValue (TRUE);
  slider1.setMinValue (0);
  slider1.setMaxValue (100);
  slider1.setTicks (0);
  controls1.addComponent (slider1);

  ListChoice list1;
  list1.setX (36);
  list1.setY (60);
  list1.setWidth (200);
  list1.setHeight (74);
  list1.setLabel ("Select a Value");
  list1.setLabelPosition (LEFT);
  list1.setDisplayedRows (3);
  list1.setReadOnly (FALSE);
  list1.setMultipleChoice (FALSE);
  list1.setSelectionRequired (FALSE);
  controls1.addComponent (list1);

  AlphanumericInput textfield1;
  textfield1.setUnderline (TRUE);
  textfield1.setX (36);
  textfield1.setY (152);
  textfield1.setWidth (200);
  textfield1.setHeight (15);
  textfield1.setLabel ("Current List Value:");
  textfield1.setValueX (172);
  textfield1.setValueY (152);
  textfield1.setLabelPosition (LEFT);
  textfield1.setInputDisplayLength (8);
  textfield1.setMaxInputLength (80);
  textfield1.setReadOnly (FALSE);
  controls1.addComponent (textfield1);

  list1.addNotifyTarget (textfield1, list_target);
  slider1.addNotifyTarget (list1, slider_target);

  Notifier notifier;
  notifier.start ();

  exit (0);
}

void list_target (UIObject *event_object, UIObject *target_object)
{
  char *value;

  value = event_object->getValue (value);
  ((AlphanumericInput *) target_object)->setValue (value);
}

void slider_target (UIObject *event_object, UIObject *target_object)
{
  char string [12];
  int  value;

  value = event_object->getValue (value);
  sprintf (string, "%d", value);

  ((ListChoice *) target_object)->addChoice (string);
}
