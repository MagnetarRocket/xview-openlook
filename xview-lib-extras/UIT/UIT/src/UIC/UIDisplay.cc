/*H+ UIDisplay.cc
*
*    SCCS ID: @(#)UIDisplay.cc 1.5 93/04/09 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the UIDisplay class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       05/10/91   Preparation for code review. 
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)UIDisplay.cc 1.5 93/04/09 SMI"; 
#endif

// Includes
#include "HotRegion.h"
#include "UIDisplay.h"

/*F+ UIDisplay::UIDisplay ()
*
*    PURPOSE:
*
*    UIDisplay constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIDisplay::UIDisplay ()
{
  extendHeight = FALSE;
  extendWidth = FALSE;
  border = FALSE;
  newXParent = (XID) UIC_NOT_SET;
  newXParentX = UIC_NOT_SET;
  newXParentY = UIC_NOT_SET;
};

/*F+ UIDisplay::~UIDisplay ()
*
*    PURPOSE:
*
*    UIDisplay destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIDisplay::~UIDisplay ()
{
}

/*F+ void UIDisplay::setExtendToEdge (Dimension dimension)
*
*    PURPOSE:
*
*    Specifies that the display object should be extended to edge
*    of its parent window in the direction of the specified dimension.
*
*    PARAMETERS:
*
*    dimension - The dimension to extend the edge (WIDTH or HEIGHT).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIDisplay::setExtendToEdge (Dimension dimension)
{
  if (dimension == HEIGHT) {
    extendHeight = TRUE;

    if (owObject)
      xv_set (owObject, XV_HEIGHT, WIN_EXTEND_TO_EDGE, NULL);
  }
  else {
    extendWidth = TRUE;

    if (owObject)
      xv_set (owObject, XV_WIDTH, WIN_EXTEND_TO_EDGE, NULL);
  }
}

/*F+ void UIDisplay::eventProc (Xv_window win, Event *event)
*
*    PURPOSE:
*
*    The XView display event proc.  This is called whenever an
*    event occurs on a display object.
*
*    PARAMETERS:
*
*    win   - The XView object the event occurred on.
*    event - The XView event info structure.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIDisplay::eventProc (Xv_window win, Event *event)
{
  UIDisplay *object = (UIDisplay *) xv_get (win, XV_KEY_DATA, UIC_OBJECT_KEY);
  
  // Set the UIC event information
  object->setEventInformation (event);
  
  // Call the user specified event handler
  if (object->useDefaultEventHandlerFlag)  
    object->defaultEventHandler ();  
  else if (object->eventHandler)
    (*object->eventHandler) (object);
  
  // Erase the UIC event information
  object->resetEventInformation ();
}

/*F+ void UIDisplay::createDragSite ()
*
*    PURPOSE:
*
*    Creates the site for sourcing drags from this object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIDisplay::createDragSite ()
{
  if (!dragSite) createXViewDragSite (textsw_first (owObject));
}

/*F+ void UIDisplay::createDropSite ()
*
*    PURPOSE:
*
*    Creates the site for accepting drops on this object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIDisplay::createDropSite ()
{
  Rect *rectangle;

  if (!dropSite) {
    if ((dropRectangle.r_width > 0) && (dropRectangle.r_height > 0))
      rectangle = &dropRectangle;
    else
      rectangle = (Rect *) xv_get (owObject, XV_RECT);

    createXViewDropSite (textsw_first (owObject), rectangle);
  }
}

/*F+ void UIDisplay::createHotRegionDragSite (HotRegion &hot_region)
*
*    PURPOSE:
*
*    Creates the site for sourcing drags on this object using the
*    specified hot region.
*
*    PARAMETERS:
*
*    hot_region - The hot region that specifies the size and location
*                 of the site.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIDisplay::createHotRegionDragSite (HotRegion &hot_region)
{
  if (!hot_region.dragSite)
    hot_region.createXViewDragSite (textsw_first (owObject));
}
 
/*F+ void UIDisplay::createHotRegionDropSite (HotRegion &hot_region)
*
*    PURPOSE:
*
*    Creates the site for accepting drops on this object using the
*    specified hot region.
*
*    PARAMETERS:
*
*    hot_region - The hot region that specifies the size and location
*                 of the site.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIDisplay::createHotRegionDropSite (HotRegion &hot_region)
{
  Rect rectangle;

  if (!hot_region.dropSite) {
    if ((dropRectangle.r_width > 0) && (dropRectangle.r_height > 0)) {
      rectangle = dropRectangle;
    }
    else {
      int rect_x, rect_y, rect_w, rect_h;

      hot_region.getDefaultRectangle (rect_x, rect_y, rect_w, rect_h);
      rect_construct (&rectangle, rect_x, rect_y, rect_w, rect_h);
    }

    hot_region.createXViewDropSite (textsw_first (owObject), &rectangle);
  }
}

#ifdef TEST
#endif /* TEST */
