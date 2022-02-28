/*H+ CommandMenu.h
*
*    SCCS ID: @(#)CommandMenu.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CommandMenu class.
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

/*C+ CommandMenu
*
*    DESCRIPTION:
*
*    A menu that displays a choice of commands.  Only one command may
*    be selected.
*
*    INHERITED CLASSES:
*
*    UIMenu - Generic menu class.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *)
*            - Creates the XView object.
*
*    virtual void setXViewMenuHandler ()
*            - Sets the handler on an XView menu.
*
*    virtual void setXViewMenuItemHandler (Menu_ item)
*            - Sets the handler on an XView menu item.
*
*    static  void menuNotifyProc (Menu, Menu_item)
*            - XView menu notify handler.
*
*    static  void menuItemProc (Menu, Menu_item)
*            - XView menu item notify handler.
*
*C-*/
#ifndef _COMMANDMENU_H
#define _COMMANDMENU_H

// Includes
#include "UIMenu.h"

// Class Definition
class CommandMenu : public UIMenu {
public:
  CommandMenu () {}
  ~CommandMenu () {}

protected:
  virtual void createObject (UIObject *);
  virtual void setXViewMenuHandler ();
  virtual void setXViewMenuItemHandler (Menu_item);

  static void menuNotifyProc (Menu, Menu_item);
  static void menuItemProc (Menu, Menu_item);
};

#endif /* _COMMANDMENU_H */
