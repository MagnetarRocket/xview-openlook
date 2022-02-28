/*H+ Scroller.h
*
*    SCCS ID: @(#)Scroller.h 1.2 93/02/08 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the Scroller class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/10/91   Preparation for code review
*    MGS       08/24/92   Added event handling code and other features
*                         provided by Terry Glanfield.
*
*H-*/

/*C+ Scroller
*
*    DESCRIPTION:
*
*    A Scroller object is a scrollbar that contains directional 
*    arrows and a box that users can drag up and down (or back 
*    and forth).  A user may click on either  of the arrows, 
*    which moves what is displayed in the ComponentDisplay by a 
*    fixed amount. If the user instead uses the mouse pointer 
*    to drag the box, then the inside of the ComponentDisplay is 
*    changed by the same amount that the box moves.  A Scroller 
*    can be placed on a ComponentDisplay object to allow a user 
*    to change what is visible.
*
*    INHERITED CLASSES:
*
*    UIComponent - Defines the general characteristics of
*    components in the UIC.
*
*    ORDINARY DATA MEMBERS:
*
*    Orientation     orientation - Horizontal or Vertical.
*
*    int	     scrollAmount - The number of pixels that 
*    the view should scroll when the user selects one of 
*    the arrows.
*
*    int             unitLength - The unit length of the object that
*    owns this scrollbar.
*
*    int	     viewLength - The size of the height or width 
*    of the ComponentDisplay that the scrollbar is associated with.
*
*    int             pageLength - The page length of what is in 
*    the view window.
*
*    int	     objectLength - The length of the object 
*    registered with the scrollbar.
*
*    int	     viewStart - The current location of 
*    the scrollbar relative to what is displayed in the view window.
*
*    Boolean	     splittable - TRUE if the view window is 
*    splittable.
*
*    Boolean	     eventHandlerSet - TRUE if an event handler has
*    already been set.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*           void	 setScrollAmount (int) - Sets the number of 
*           pixels that the view should scroll when the user selects 
*           one of the arrows.  Default: Not set.
*
*           void         setUnitLength (int) - Sets the unit length of the
*           object registered with the scrollbar. Default: Not set.
*
*           void	 setViewLength (int) - Sets the size of the 
*           height or width of the object that the scrollbar
*           is associated with. Default: Not set.
*
*           void	 setPageLength (int) - Sets the page length of 
*           what is in the view window. Default: Not set.
*
*           void	 setObjectLength (int) - Sets the length of the 
*           object registered with the scrollbar. Default: Not set.
*
*           void	 setViewStart (int) - Sets the current 
*           location of the scrollbar relative to what is displayed 
*           in the view window. Default: Not set.
*
*           void	 setOrientation (Orientation) - Sets the 
*           orientation (VERTICAL or HORIZONTAL) of the Scroller.
*           Default: VERTICAL.
*
*           void	 setSplittable (Boolean) - Specifies that the view
*           window is splittable.  Default: FALSE.
*
*           int          getScrollAmount () - Gets the number of
*           pixels that the view should scroll when the user selects
*           one of the arrows.
*
*           int          getUnitLength () - Gets the unit length of the
*           object registered with the scrollbar. Default: Not set.
*
*
*           int          getViewLength () - Gets the size of the
*           height or width of the object that the scrollbar
*           is associated with.
*
*           int          getPageLength () - Gets the page length of
*           what is in the view window.
*
*           int          getObjectLength () - Gets the length of the
*           object registered with the scrollbar. Default: Not set.
*
*           int	         getViewStart () - Gets the current 
*           location of the scrollbar relative to what is displayed 
*           in the view window.
*
*           int          getLastViewStart () - Gets the previous
*           location of the scrollbar relative to what is displayed
*           in the view window prior to scroll.
*
*           int		 getCurrentLocation () - Gets the current 
*           location of the scrollbar relative to what is displayed 
*           in the view window.
*
*           void         setEventHandler () - Sets the Scroller event
*           handler.  The handler is only called if the event is a
*           SCROLLBAR_REQUEST event.
*
*   PROTECTED MEMBER FUNCTIONS:
*
*   virtual void 	 createObject (UIObject *) - Create XView object.
*
*   static Notify_value  eventProc (Xv_window,
*                                   Event *,
*                                   Notify_arg,
*                                   Notify_event_type) - XView event
*	    proc use by notify_interpose_event_func.
*
*   PRIVATE MEMBER FUNCTIONS:
*
*           void	 setScrollerDefaults () - Initializes the data members.
*
*C-*/
#ifndef _SCROLLER_H
#define _SCROLLER_H

// Includes
#include "UIComponent.h"

#include <xview/frame.h>
#include <xview/scrollbar.h>

// Class Definition
class Scroller : public UIComponent {
public:
    		 	 Scroller ();
    		 	 Scroller (Orientation);
			~Scroller ();

    void		 setScrollAmount (int amount)
			 { scrollAmount = amount;
			   if (owObject) xv_set (owObject,
						 SCROLLBAR_PIXELS_PER_UNIT,
			   			 scrollAmount,
						 NULL); }

    void                 setUnitLength (int length)
                         { unitLength = length;
                           if (owObject)
                             xv_set (owObject,
                                     SCROLLBAR_PIXELS_PER_UNIT, unitLength,
                                     NULL); }

    void		 setViewLength (int length)
			 { viewLength = length;
			   if (owObject) xv_set (owObject,
						 SCROLLBAR_VIEW_LENGTH,
			   			 viewLength,
						 NULL); }

    void		 setPageLength (int length)
			 { pageLength = length;
			   if (owObject) xv_set (owObject,
						 SCROLLBAR_PAGE_LENGTH,
			   			 pageLength,
						 NULL); }

    void		 setObjectLength (int length)
			 { objectLength = length;
			   if (owObject) xv_set (owObject,
						 SCROLLBAR_OBJECT_LENGTH,
			   			 objectLength,
						 NULL); }

    void		 setViewStart (int start)
			 { viewStart = start;
			   if (owObject) xv_set (owObject,
						 SCROLLBAR_VIEW_START,
			   			 viewStart,
						 NULL); }

    void		 setOrientation (Orientation direction)
			 { orientation = direction;
			   if (owObject) {
			     if (direction == HORIZONTAL)
			       xv_set (owObject,
				       SCROLLBAR_DIRECTION,
				       SCROLLBAR_HORIZONTAL,
				       NULL);
			     else
			       xv_set (owObject,
				       SCROLLBAR_DIRECTION,
				       SCROLLBAR_VERTICAL,
				       NULL);
			   }
			 }

    void		 setSplittable (Boolean flag)
			 { splittable = flag; }

    int			 getScrollAmount ()
			 { if (owObject)
			     scrollAmount =
			       (int) xv_get (owObject,
					     SCROLLBAR_PIXELS_PER_UNIT);
			   return scrollAmount;
			 }

    int                  getUnitLength ()
                         { if (owObject)
                             unitLength = (int)
					  xv_get (owObject,
                                                  SCROLLBAR_PIXELS_PER_UNIT);
                           return unitLength; }

    int                  getViewLength () 
                         { if (owObject)  
                             viewLength = 
                               (int) xv_get (owObject,
                                             SCROLLBAR_VIEW_LENGTH);
                           return viewLength; 
                         } 
 
    int                  getPageLength () 
                         { if (owObject)  
                             pageLength = 
                               (int) xv_get (owObject,
                                             SCROLLBAR_PAGE_LENGTH);
                           return pageLength; 
                         } 
 
    int                  getObjectLength () 
                         { if (owObject)  
                             objectLength = 
                               (int) xv_get (owObject,
                                             SCROLLBAR_OBJECT_LENGTH);
                           return objectLength; 
                         } 
 
    int		         getViewStart ()
			 { if (owObject)
			     viewStart =
			       (int) xv_get (owObject, 
					     SCROLLBAR_VIEW_START);
    			   return (viewStart); }

    int                  getLastViewStart ()
                         { if (owObject)
                             lastViewStart = (int)
					     xv_get (owObject,
                                                     SCROLLBAR_LAST_VIEW_START);
                           return (lastViewStart); }

    int			 getCurrentLocation ()
			 { return getViewStart (); }

    void	         setEventHandler (UICHandlerFunction);

protected:
    Orientation 	 orientation;
    int			 scrollAmount;
    int			 viewLength;
    int			 pageLength;
    int			 objectLength;
    int                  unitLength;
    int			 viewStart;
    int                  lastViewStart;
    Boolean		 splittable;
    Boolean		 eventHandlerSet;

    virtual void 	 createObject (UIObject *);

    static Notify_value  eventProc (Xv_window,
                                    Event *,
                                    Notify_arg,
                                    Notify_event_type);
 
private:
    void		 setScrollerDefaults ();
};

#endif /* _SCROLLER_H */
