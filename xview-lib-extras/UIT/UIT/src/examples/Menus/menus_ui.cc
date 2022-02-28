/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "CommandMenu.h"
#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "AlphanumericInput.h"
#include "ListChoice.h"
#include "Button.h"
#include "NoticePrompt.h"
#include "Notifier.h"

#define FONT_MENU_BUTTON "Font Menu Button"
#define NEW_FONT_NAME    "New Font Name"
#define SELECTED_FONTS   "Selected Fonts"

void new_font_target (UIObject *, UIObject *);
void selected_fonts_target (UIObject *, UIObject *);

main (int argc, char **argv)
{
  BaseWindow base_window;
  base_window.initUI (argc, argv);
  base_window.setWidth (400);
  base_window.setHeight (150);
  base_window.setLabel ("Menus and Fonts");
  base_window.setClosed (FALSE);
  base_window.setDisplayFooter (TRUE);
  base_window.setResizable (TRUE);

  CommandMenu font_menu;
  CommandMenu font_size_menu;

  font_menu.setDisplayedColumns (1);
  font_menu.setLabel ("Font Menu");

  ComponentDisplay component_display (TRUE);
  component_display.setX (0);
  component_display.setY (0);
  component_display.setExtendToEdge (WIDTH);
  component_display.setExtendToEdge (HEIGHT);
  component_display.setDisplayBorders (FALSE);
  base_window.addDisplay (component_display);

  AlphanumericInput new_font;
  new_font.setUnderline (TRUE);
  new_font.setX (16);
  new_font.setY (24);
  new_font.setWidth (369);
  new_font.setHeight (15);
  new_font.setLabel ("New Font:");
  new_font.setValueX (89);
  new_font.setValueY (24);
  new_font.setLabelPosition (LEFT);
  new_font.setInputDisplayLength (37);
  new_font.setMaxInputLength (80);
  new_font.setReadOnly (FALSE);
  component_display.addComponent (new_font);

  ListChoice selected_fonts;
  selected_fonts.setX (172);
  selected_fonts.setY (48);
  selected_fonts.setWidth (200);
  selected_fonts.setHeight (91);
  selected_fonts.setLabel ("Selected Fonts:");
  selected_fonts.setLabelPosition (ABOVE);
  selected_fonts.setDisplayedRows (3);
  selected_fonts.setReadOnly (FALSE);
  selected_fonts.setMultipleChoice (FALSE);
  selected_fonts.setSelectionRequired (FALSE);
  component_display.addComponent (selected_fonts);

  Button font_menu_button;
  font_menu_button.setX (16);
  font_menu_button.setY (60);
  font_menu_button.setAbbreviated (TRUE);
  font_menu_button.setWidth (99);
  font_menu_button.setHeight (15);
  font_menu_button.setLabel ("Font Menu");
  font_menu_button.setMenu (font_menu);
  component_display.addComponent (font_menu_button);

  new_font.addNotifyTarget (font_menu, new_font_target);
  new_font.setObjectData (SELECTED_FONTS, selected_fonts);
  new_font.setObjectData (FONT_MENU_BUTTON, font_menu_button);

  Notifier notifier;
  notifier.start ();

  exit (0);
}

void new_font_target (UIObject *event_object, UIObject *target_object)
{
  char *new_font;
  new_font = event_object->getValue (new_font);

  char *font_notice_message = new char [strlen (new_font) + 15];
  sprintf (font_notice_message, "Add the %s font?", new_font);

  NoticePrompt font_notice;
  font_notice.setMessage (font_notice_message); 

  if (font_notice.show (event_object) == NOTICE_YES) {
    ListChoice *selected_fonts = (ListChoice *)
  			         event_object->getObjectData (SELECTED_FONTS);

    CommandMenu *font_size_menu = new CommandMenu;
    font_size_menu->setLabel (new_font);
    font_size_menu->addItem ("8");
    font_size_menu->addItem ("10");
    font_size_menu->addItem ("12");
    font_size_menu->addItem ("14");
    font_size_menu->setPinnable (TRUE);
    font_size_menu->addNotifyTarget (*selected_fonts, selected_fonts_target);
    font_size_menu->setObjectData (NEW_FONT_NAME, (Generic *) new_font);

    CommandMenu *font_menu = (CommandMenu *) target_object;
    font_menu->addItem (new_font, *font_size_menu);

    Button *button = (Button *) event_object->getObjectData (FONT_MENU_BUTTON);
    button->setMenu (*font_menu);

    ((AlphanumericInput *) event_object)->setValue ("");
  }

  delete font_notice_message;
}

void selected_fonts_target (UIObject *event_object, UIObject *target_object)
{
  int choice;
  choice = event_object->getChoice (choice);
  
  char *family = (char *) event_object->getObjectData (NEW_FONT_NAME);
  int   size = 6 + choice * 2;

  CharacterFont *font = new CharacterFont (family, FONT_STYLE_DEFAULT, size);

  char *font_name = new char [strlen (family) + 12];
  sprintf (font_name, "%s%d", family, size);

  ListChoice *list = (ListChoice *) target_object;
  list->setListFont (*font);
  list->addChoice (font_name);

  delete font_name;
}
