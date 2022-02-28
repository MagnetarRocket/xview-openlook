/*H+ Scroller.cc
*
*    SCCS ID: @(#)Scroller.cc 1.2 93/02/08 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the Scroller class.
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
static char sccsid [] = "@(#)Scroller.cc 1.2 93/02/08 SMI"; 
#endif

// Includes
#include "Scroller.h"
#include "ColorMap.h"

/*F+ Scroller::Scroller ()
*
*    PURPOSE:
*
*    Scroller constructor.  Default direction is VERTICAL.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Scroller::Scroller ()
{
  setScrollerDefaults ();
}

/*F+ Scroller::Scroller (Orientation direction)
*
*    PURPOSE:
*
*    Scroller constructor.
*
*    direction - Specifies the orientationof the Scroller, VERTICAL or
*                HORIZONTAL.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Scroller::Scroller (Orientation direction)
{
  setScrollerDefaults ();
  orientation = direction;
}

/*F+ void Scroller::setScrollerDefaults ()
*
*    PURPOSE:
*
*    Initializes Scroller data members.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Scroller::setScrollerDefaults ()
{
  orientation = VERTICAL;
  scrollAmount = UIC_NOT_SET;
  viewLength = UIC_NOT_SET;
  pageLength = UIC_NOT_SET;
  objectLength = UIC_NOT_SET;
  unitLength = UIC_NOT_SET;
  viewStart = UIC_NOT_SET;
  splittable = FALSE;
  eventHandlerSet = FALSE;
  isScroller = TRUE;
}

/*F+ Scroller::~Scroller ()
*
*    PURPOSE:
*
*    Scroller destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Scroller::~Scroller ()
{
}

/*F+ void Scroller::setEventHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Specifies the Scroller event handler function
*
*    PARAMETERS:
*
*    handler - The function to call to handle the events.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void Scroller::setEventHandler (UICHandlerFunction handler)
{
  if (owObject && !eventHandlerSet) {
    notify_interpose_event_func (xv_get (owObject, SCROLLBAR_NOTIFY_CLIENT),
                                 (Notify_func) &Scroller::eventProc,
                                 NOTIFY_SAFE);
    eventHandlerSet = TRUE;
  }

  eventHandler = handler;
}

/*F+ Notify_value Scroller::eventProc (Notify_client  client,
*                                      Event         *event,
*                                      Scrollbar      xview_scrollbar,
*                                      Notify_type    type)
*
*    PURPOSE:
*
*    The scroller event proc.  This was set with a notify interpose and
*    is called when ever an event occurs on a scroller.
*
*    PARAMETERS:
*
*    client - The XView object the event occurred on.
*    event  - The XView event info structure.
*    arg    - XView client data.
*    type   - The XView notifier event type: SAFE or IMMEDIATE.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
Notify_value Scroller::eventProc (Notify_client      client,
                                  Event             *event,
                                  Scrollbar          xview_scrollbar,
                                  Notify_event_type  type)
{
  Scroller *object = (xview_scrollbar ? (Scroller *) xv_get (xview_scrollbar,
					                     XV_KEY_DATA,
					                     UIC_OBJECT_KEY) :
				        (Scroller *) NULL);

  if (object && (event_id (event) == SCROLLBAR_REQUEST)) {
    // Set the UIC event information
    object->setEventInformation (event);

    // Call the user specified event handler
    if (object->useDefaultEventHandlerFlag)
      object->defaultEventHandler ();
    else if (object->eventHandler)
      (*object->eventHandler) (object);

    // Erase the UIC event information
    object->resetEventInformation ();

    if (object->eventResultSet) {
      object->eventResultSet = FALSE;
      return (object->eventResult);
    }
  }

  return (notify_next_event_func (client,
				  (Notify_event) event,
				  xview_scrollbar,
				  type));
}

/*F+ void Scroller::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object of the Scroller.
*
*    PARAMETERS:
*
*    parent - The parent of the Scroller or who the Scroller is attached to.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Scroller::createObject (UIObject *parent)
{
  if (orientation == VERTICAL)
    owObject = xv_create (parent->getXViewObject (), SCROLLBAR,
			  SCROLLBAR_DIRECTION, SCROLLBAR_VERTICAL,
			  NULL);
  else
    owObject = xv_create (parent->getXViewObject (), SCROLLBAR,
			  SCROLLBAR_DIRECTION, SCROLLBAR_HORIZONTAL,
			  NULL);
  
  /* Save the "this" pointer as key data on the Scroller object */
  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  // Set the XView help data
  setHelpData ();
  
  initializeColorMap (parent);

  if (foregroundColor != UIC_NOT_SET)
    xv_set (owObject,
	    WIN_CMS,              colorMap->getXViewObject (),
	    WIN_FOREGROUND_COLOR, foregroundColor,
	    NULL);
  
  if (backgroundColor != UIC_NOT_SET)
    xv_set (owObject,
	    WIN_CMS,              colorMap->getXViewObject (),
	    WIN_BACKGROUND_COLOR, backgroundColor,
	    NULL);
    
  if (scrollAmount != UIC_NOT_SET)
    xv_set (owObject, SCROLLBAR_PIXELS_PER_UNIT, scrollAmount, NULL);
  
  if (viewLength != UIC_NOT_SET)
    xv_set (owObject, SCROLLBAR_VIEW_LENGTH, viewLength, NULL);
  
  if (pageLength != UIC_NOT_SET)
    xv_set (owObject, SCROLLBAR_PAGE_LENGTH, pageLength, NULL);
  
  if (objectLength != UIC_NOT_SET)
    xv_set (owObject, SCROLLBAR_OBJECT_LENGTH, objectLength, NULL);
  
  if (unitLength != UIC_NOT_SET)
    xv_set (owObject, SCROLLBAR_PIXELS_PER_UNIT, unitLength, NULL);

  if (viewStart != UIC_NOT_SET)
    xv_set (owObject, SCROLLBAR_VIEW_START, viewStart, NULL);
  
  if (splittable)
    xv_set (owObject, SCROLLBAR_SPLITTABLE, TRUE, NULL);

  if (eventHandler || useDefaultEventHandlerFlag) {
    notify_interpose_event_func (xv_get (owObject, SCROLLBAR_NOTIFY_CLIENT),
                                 (Notify_func) &Scroller::eventProc,
                                 NOTIFY_SAFE);
    eventHandlerSet = TRUE;
  }

  xv_set (owObject, SCROLLBAR_INACTIVE, (active ? FALSE : TRUE), NULL);

  UIObject::setUIObjectAttributes (parent);
}

#ifdef TEST
#endif /* TEST */
