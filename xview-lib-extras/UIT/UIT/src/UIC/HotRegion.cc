/*H+ HotRegion.cc
*
*    SCCS ID: @(#)HotRegion.cc 1.1 91/10/24 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member function of the HotRegion class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       05/10/91   Preparation for code review. 
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)HotRegion.cc 1.1 91/10/24 SMI"; 
#endif

// Includes
#include "HotRegion.h"

/*F+ void HotRegion::setDetectionFunction (HotRegionDetectionFunction det_func)
*
*    PURPOSE:
*
*    Allows the user to specify a detection function for detecting whether
*    or not an event is in this hot region.
*
*    PARAMETERS:
*
*    det_func - The user defined detection function.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void HotRegion::setDetectionFunction (HotRegionDetectionFunction det_func)
{
  detectionFunction = det_func;
}

/*F+ void HotRegion::setRegionHandler (UICHandlerFunction region_handler)
*
*    PURPOSE:
*
*    Allows the user to specify a function to handle an event that
*    occurs inside of this hot region.
*
*    PARAMETERS:
*
*    region_handler - The user defined region handler.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void HotRegion::setRegionHandler (UICHandlerFunction region_handler)
{
  regionHandler = region_handler;
}

/*F+ Boolean HotRegion::detectHotRegionEvent (UIObject *event_object,
*                                             Event    *xview_event)
*
*    PURPOSE:
*
*    Detects whether or not the specified XView event occurred in
*    this HotRegion object.  If it did, the user supplied region
*    handler is called and a TRUE result is returned.
*
*    PARAMETERS:
*
*    event_object - The UIC object the event occurred on.
*    xview_event  - The XView event.
*
*    RETURN VALUES:
*
*    If the XView event's X-Y location is in the hot region, the return
*    value is TRUE.  Otherwise, it is FALSE.
*
*    SIDE EFFECTS:
*
*    The pointer to the object the event occurred on is saved in
*    a private Hot Region data member, hotRegionObject and the XView
*    event pointer is saved in the data memvber "lastEvent".
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Boolean HotRegion::detectHotRegionEvent (UIObject *event_object,
					 Event    *xview_event)
{
  Boolean result;

  int x = event_x (xview_event);
  int y = event_y (xview_event);
  
  // If the user supplied a detection function...
  if (detectionFunction)
    // Call the user defined detection function
    result = (*detectionFunction) (this, x, y);
  // Else...
  else
    // Call the default detection function (rectangular hot region)
    result = defaultDetectionFunction (x, y);
  
  // If the X-Y location is in the hot region...
  if (result) {
    // Save the pointer to the hot region object
    hotRegionObject = event_object;

    // Save the pointer to the XView event
    lastEvent.setXViewEvent (xview_event);

    // Call the user defined region handler
    if (useDefaultRegionHandlerFlag)
      defaultRegionHandler ();
    else if (regionHandler)
      (*regionHandler) (this); 
  }
  
  return result;
}

#ifdef TEST
#endif /* TEST */
