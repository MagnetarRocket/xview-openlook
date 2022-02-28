/*H+ InputEvent.cc
*
*    SCCS ID: @(#)InputEvent.cc 1.4 92/06/04 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the InputEvent class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)InputEvent.cc 1.4 92/06/04 SMI"; 
#endif

// Includes
#include "UIObject.h"

// Static member initialization
short InputEvent::thresholdsInitialized = 0;
int   InputEvent::timeThreshold = 0;
int   InputEvent::dragThreshold = 0;

/*F+ Boolean InputEvent::detectDoubleClick (Event *xview_event)
*
*    PURPOSE:
*
*    Returns TRUE if the given xview event is a double click event.
*    The time and drag thresholds are initialized to the values
*    specified in the .Xdefaults file.
*
*    PARAMETERS:
*
*    Event *xview_event - A pointer to an XView event structure.
*
*    RETURN VALUES:
*
*    Returns TRUE if the event was a double click.  Returns FALSE
*    if it was not.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Boolean InputEvent::detectDoubleClick (Event *xview_event)
{
  int return_value = FALSE;

  if ((event_action (xview_event) == ACTION_SELECT) &&
      (event_is_down (xview_event))) {
    currentSelectTime = event_time (xview_event);
 
    // Make sure we have not moved too far in between clicks.
    if (rect_includespoint (&lastRegion,
                            event_x (xview_event),
                            event_y (xview_event))) {
      if (currentSelectTime.tv_sec == lastSelectTime.tv_sec) {
        // Check if micro seconds are in range
        if ((currentSelectTime.tv_usec - lastSelectTime.tv_usec) <
            timeThreshold)
          return_value = TRUE;
      }
      else if (currentSelectTime.tv_sec == (lastSelectTime.tv_sec + 1)) {
        // Check for rollover condition.
        if ((1000000 - lastSelectTime.tv_usec + currentSelectTime.tv_usec) <
            timeThreshold)
          return_value = TRUE;
      }
    }
 
    lastSelectTime = currentSelectTime;
    lastRegion.r_left = event_x (xview_event) - dragThreshold;
    lastRegion.r_top  = event_y (xview_event) - dragThreshold;
    lastRegion.r_width = lastRegion.r_height = 2 * dragThreshold;
  }
 
  return return_value;
}

#ifdef TEST
#endif /* TEST */
