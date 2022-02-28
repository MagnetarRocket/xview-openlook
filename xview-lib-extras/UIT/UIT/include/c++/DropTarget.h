/*H+ DropTarget.h
*
*    SCCS ID: @(#)DropTarget.h 1.1 91/10/24 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the DropTargetclass.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ DropTarget
*
*    DESCRIPTION:
*
*    The DropTarget class provides a site for drop reception on
*    panels from drag and drop actions.
*
*    ORDINARY DATA MEMBERS:
*
*    Image *icon - The DropTarget icon.
*    Image *busyIcon - The icon used when the DropTarget is busy.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*            void setIcon (Image &) - Sets the normal icon of the DropTarget.
*            void setIcon (char *) - Sets the normal icon of the DropTarget.
*
*            void setBusyIcon (Image &) - Sets the busy icon of the DropTarget.
*            void setBusyIcon (char *) - Sets the busy icon of the DropTarget.
*
*    virtual void setDropRectangle (int x, int y, int width, int height) -
*            Specifies the drop rectangle for this object.  Since this is
*            a DropTarget object, this changes the actual size and position
*            of the Drop Target.
*
*    virtual void setDefaultDropSite (Boolean) - Specifies whether or not
*            this object is the default drop dite for the window it is in.
*            Default: FALSE.
*
*    virtual void setAcceptDrops (Boolean) - Specifies whether or not
*            this object accepts drops.  Since this is a DropTarget object,
*            setAcceptDrops will activate or deactivate the DropTarget object
*            (via setActive).
*            Default: TRUE.
*
*    virtual void setDraggable (Boolean) - Specifies whether or not
*            this object is draggable.  Default: FALSE.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView object.
*
*    virtual void createDragSite () - Creates a drag site for this object.
*
*    virtual void createDropSite () - Creates a drop site for this object.
*
*    static  int  notifyProc (Panel_item, int, Event *) - Internal
*    wrapper for the XView PANEL_DROP_TARGET notify proc.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*            void setDropTargetDefaults () - Initializes the data members of
*    the DropTarget object.
*
*C-*/
#ifndef _DROP_TARGET_H
#define _DROP_TARGET_H

// Includes
#include "UIComponent.h"

// Class Definition
class DropTarget : public UIComponent {
public:
  DropTarget ();
  DropTarget (char *);
  DropTarget (Image &);

  ~DropTarget (); 

  void setIcon (Image &icon_image)
    { if (newIcon) delete icon;

      newIcon = FALSE;
      icon = &icon_image;

      if (owObject && icon) {
	((UIObject *) icon)->createObject (this);
	xv_set (owObject, PANEL_DROP_GLYPH, icon->getXViewObject (), NULL);
      }
    }

  void setIcon (char *icon_file)
    { if (newIcon) delete icon; 

      newIcon = TRUE; 
      icon = new Image (icon_file);

      if (owObject && icon) {
	((UIObject *) icon)->createObject (this); 
	xv_set (owObject, PANEL_DROP_GLYPH, icon->getXViewObject (), NULL);           }
    } 
 
  void setBusyIcon (Image &icon_image)
    { if (newBusyIcon) delete busyIcon; 

      newBusyIcon = FALSE; 
      busyIcon = &icon_image;

      if (owObject && busyIcon) {
	((UIObject *) busyIcon)->createObject (this); 
	xv_set (owObject,
		PANEL_DROP_BUSY_GLYPH, busyIcon->getXViewObject (),
		NULL);
      }
    } 
 
  void setBusyIcon (char *icon_file)
    { if (newBusyIcon) delete busyIcon;  

      newBusyIcon = TRUE;  
      busyIcon = new Image (icon_file);

      if (owObject && busyIcon) {
	((UIObject *) busyIcon)->createObject (this);  
	xv_set (owObject,
		PANEL_DROP_BUSY_GLYPH, busyIcon->getXViewObject (),
		NULL);
      }
    }

  virtual void setDropRectangle (int x, int y, int width, int height)
    { setLocation (x, y);
      setSize (width, height);
    }

  virtual void setDefaultDropSite (Boolean flag)
    { defaultDropSite = flag;

      if (owObject)
	xv_set (owObject, PANEL_DROP_SITE_DEFAULT, defaultDropSite, NULL);
    }

  virtual void setAcceptDrops (Boolean);
  virtual void setDraggable (Boolean);

protected:
  Image   *icon;
  Boolean  newIcon;
  Image   *busyIcon;
  Boolean  newBusyIcon;

  virtual void createObject (UIObject *);

  virtual void createDragSite ();
  virtual void createDropSite ();

  static  int notifyProc (Panel_item, int, Event *);

private:
  void setDropTargetDefaults ();
};

#endif /* _DROP_TARGET_H */
