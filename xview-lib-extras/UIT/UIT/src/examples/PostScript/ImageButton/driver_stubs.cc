/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "CommandMenu.h"
#include "ImageButton.h"

/*
 * Menu handler for `button_menu'.
 */
void button_menu_handler (UIObject *object)
{
  int choice;

  fprintf (stderr, "ext: button_menu_handler: Item %d\n", object->getChoice (choice));

  return;
}

void b_handler (UIObject *object)
{
  static Boolean flag = FALSE;

  ImageButton *ib = (ImageButton *) object->getObjectData ("IB");
  ib->show (flag);
  flag = !flag;

  fprintf (stderr, "ext: b_handler\n");
  return;
}

void ib_handler (UIObject *object)
{
  fprintf (stderr, "ext: ib_handler\n");
  return;
}
