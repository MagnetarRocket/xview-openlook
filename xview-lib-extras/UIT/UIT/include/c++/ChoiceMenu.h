/*H+ ChoiceMenu.h
*
*    SCCS ID: @(#)ChoiceMenu.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the ChoiceMenu class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       10/07/90   Original creation.
*
*H-*/

/*C+ ChoiceMenu
*
*    DESCRIPTION:
*
*    A menu that displays choices.  A single choice or multiple choice menu
*    may be created.
*
*    INHERITED CLASSES:
*
*    UIMenu - Generic menu class.
*
*    ORDINARY DATA MEMBERS:
*
*    multipleChoice - Specifies whether or not multiple selections may be
*                     made.
*    selected       - A list of the selections that have been made.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setMultipleChoice (Boolean)
*         - Sets a flag indicating whether multiple selections are
*           allowed or not.  Default: FALSE.
*
*    int getNumberSelected ()
*        - Returns the number of selections made.
*
*    int    getChoice (int, int&)
*    char  *getChoice (int, char*&)
*    Image *getChoice (int, Image*&)
*           - Get the specified choice from the list of selected choices.
*
*    void setSelectedItem (int, Boolean)
*	  - Sets the specified choice as selected or not selected.
*
*    PROTECTED MEMBER FUCNTIONS:
*
*    virtual void createObject (UIObject *)
*            - Creates the XView object.
*
*    virtual void updateSelected (int, int)
*	     - Used to update the list of selections when an item has
*              been deleted from the menu.
*
*    virtual void setXViewMenuHandler ()
*            - Sets the handler on an XView menu.
*
*    virtual void setXViewMenuItemHandler (Menu_item)
*            - Sets the handler on an XView menu item.
*
*    static  void menuNotifyProc (Menu, Menu_item)
*            - XView menu notify handler.
*
*    static  void menuItemProc (Menu, Menu_item)
*            - XView menu item notify handler.
*
*C-*/
#ifndef _CHOICEMENU_H
#define _CHOICEMENU_H

// Includes
#include "UIMenu.h"

// Class Definition
class ChoiceMenu : public UIMenu {
public:
  ChoiceMenu () { multipleChoice = FALSE; }
  ~ChoiceMenu () { while (selected.removeItem (0)) ; }

  void          setMultipleChoice (Boolean flag) { multipleChoice = flag; }
  int           getNumberSelected () { return selected.getSize (); }

  int           getChoice (int, int&);
  char         *getChoice (int, char*&);
  Image        *getChoice (int, Image*&);

  void          setSelectedItem (int, Boolean);

protected:
  Boolean      multipleChoice;
  GenericList  selected;

  virtual void createObject (UIObject *);
  virtual void updateSelected (int, int);
  virtual void setXViewMenuHandler ();
  virtual void setXViewMenuItemHandler (Menu_item);

  static void menuNotifyProc (Menu, Menu_item);
  static void menuItemProc (Menu, Menu_item);
};

#endif /* _CHOICEMENU_H */
