/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "BaseWindow.h"
#include "Button.h"
#include "ComponentDisplay.h"
#include "Notifier.h"
#include "PropertiesWindow.h"

void property_handler (UIObject *);

main (int argc, char **argv)
{
  // Create the base window and initialize the UI
  BaseWindow bw;
  bw.initUI (argc, argv);
  bw.setLabel ("Sub-Class Example");
  bw.setFit (HEIGHT);
  bw.setFit (WIDTH);

  // Put a component display on the base window
  ComponentDisplay cd;
  cd.setFit (HEIGHT);
  cd.setFit (WIDTH);
  bw.addDisplay (cd);

  // Create a property button for the properties window
  Button pb1 ("Property 1");
  pb1.setX (10);
  pb1.setY (10);

  // Create a second property button for the properties window
  Button pb2 ("Property 2");
  pb2.setX (100);
  pb2.setY (10);

  // Create the properties window and add the property buttons
  PropertiesWindow pw;
  pw.setLabel ("Properties");
  pw.setHeight (100);
  pw.setWidth (300);
  pw.addProperty ("PropertyButton1", pb1);
  pw.addProperty ("PropertyButton2", pb2);
  pw.setNotifyHandler (property_handler);

  // Create a button to invoke the properties window
  Button b ("Properties...");
  b.setWindow (pw);
  cd.addComponent (b);

  // Display the UI
  Notifier n;
  n.start ();

  exit (0);
}

// Handler for the properties window
void property_handler (UIObject *property_window)
{
  NotifyType type = ((PropertiesWindow *) property_window)->getNotifyType ();

  if (type == APPLY)
    printf ("Apply button pressed...\n");
  else if (type == RESET)
    printf ("Reset button pressed...\n");
  else if (type == PROPERTY)
    printf ("Property was accessed, name: %s\n",
	    ((PropertiesWindow *) property_window)->getPropertyName ());
  else
    printf ("Illegal notify type...\n");
}
