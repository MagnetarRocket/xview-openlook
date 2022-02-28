/*H+ UIDisplay.h
*
*    SCCS ID: @(#)UIDisplay.h 1.4 93/04/09 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the UIDisplay class.
*
*    AUTHOR: Joe Warzecha and Mark soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/01/91   Preparation for code review
*
*H-*/

/*C+ UIDisplay
*
*    DESCRIPTION:
*
*    The UIDisplay class is the base class for all of the display region
*    classes.  Classes derived off of UIDisplay are used for specifying	
*    a region to display text or grpahics on a window.
*
*    INHERITED CLASSES:
*
*    UIObject - The base class of all UI classes in the UIC.
*
*    ORDINARY DATA MEMBERS:
*
*    Boolean	 extendWidth  - TRUE if width should be extended.
*
*    Boolean	 extendHeight - TRUE if height should be extended.
*
*    Boolean	 border       - TRUE if borders should be displayed.
*
*    XID	 newXParent   - The new XID to reparent this object to.
*
*    int	 newXParentX  - The X coordinate within the new parent.
*
*    int	 newXParentY  - The Y coordinate within the new parent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*             void    setDisplayBorders (Boolean) - Sets whether
*             or not the borders should be displayed around  this
*             display.  Default: Borders not displayed.
*
*             void    setExtendToEdge (Dimension) - Depending on 
*             the value of the Dimension argument (HEIGHT or WIDTH), 
*             determines if that Dimension should be extended to 
*             the border of the window in which it has been placed. 
*             Default: Does NOT extend its height or width to the 
*             window in which it is placed.  Default: Don't extend to edge.
*
*	      void    reparentXWindow (XID, int x, int y) - Allows this object
*	      to be reparented to another X window.  This will work properly
*	      if the XID is from a BaseWindow or a PopUpWindow, even if
*	      the new parent is being displayed by another process.  The x
*	      and y arguments allow positioning of the UIDisplay in the new
*             parent window.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    static void eventProc (Xv_window, Event *) - Internal wrapper
*           around the XView event proc for objects of classes derived
*           from UIDisplay.
*
*   void setUIDisplayAttributes (UIObject *parent) - Set the attributes
*   coomon to all UIDisplay dereived objects.
*
*   virtual void        createDragSite () - Creates a drag site for this
*           object.
*
*   virtual void        createDropSite () - Creates a drop site for this
*           object.
*
*    virtual void       createHotRegionDragSite (HotRegion&) - Creates a
*            drag site for this object using the specified hot region.
*
*    virtual void       createHotRegionDropSite (HotRegion&) - Creates a
*            drop site for this object using the specified hot region.
*
*C-*/
#ifndef _UIDISPLAY_H
#define _UIDISPLAY_H

// Includes
#include "UIObject.h"

#include <sys/param.h>
#include <xview/rect.h>
#include <xview/win_input.h>
#include <xview/window.h>

// Class Definition
class UIDisplay : public UIObject {
public:
    	  	  UIDisplay ();
		  ~UIDisplay ();

    void	  setDisplayBorders (Boolean flag)
		  { border = flag;
		    if (owObject) xv_set (owObject,
					  OPENWIN_SHOW_BORDERS, flag,
		    			  NULL); }

    void	  setExtendToEdge (Dimension);

    void          reparentXWindow (XID xid, int x=0, int y=0)
                  {
                    newXParent = xid;
                    newXParentX = x;
                    newXParentY = y;

                    if (owObject)
                      (void)
		      XReparentWindow ((Display *) xv_get (owObject,
							   XV_DISPLAY),
                                       (XID) xv_get (owObject,
						     XV_XID),
                                       xid, x, y);
                  }

protected:
    Boolean	 extendWidth;
    Boolean	 extendHeight;
    Boolean	 border;
    XID          newXParent;
    int          newXParentX;
    int          newXParentY;

    void setUIDisplayAttributes (UIObject *parent)
      {
        if (newXParent != (XID) UIC_NOT_SET) {
          XReparentWindow ((Display *) xv_get (owObject, XV_DISPLAY),
                           (XID) xv_get (owObject, XV_XID),
                           newXParent, newXParentX, newXParentY);
        }

        setUIObjectAttributes (parent);
      }

    virtual void createDragSite ();
    virtual void createDropSite ();

    virtual void createHotRegionDragSite (HotRegion&);
    virtual void createHotRegionDropSite (HotRegion&);

#ifdef __GNUC__
public:
#endif
    static  void eventProc (Xv_window, Event *);
};

#endif /* _UIDISPLAY_H */
