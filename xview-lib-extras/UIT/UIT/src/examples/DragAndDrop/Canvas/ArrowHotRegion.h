/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#ifndef _ARROW_HOT_REGION_H
#define _ARROW_HOT_REGION_H

#include "HotRegion.h"

class ArrowHotRegion : public HotRegion {
public:
  // Specifying "setDraggable" in the constructor means that this hot
  // region is draggable.  Therefore, the region on a ComponentDisplay
  // where this hot region is located and detects events will be draggable.
  // By using "setDragCursor" and "setDragHandler" to on a ArrowHotRegion
  // object, a user can specify the specifics of the drag operation.
  ArrowHotRegion (ComponentDisplay &parent_cd,
	          Image            &arrow_image) : HotRegion ()
    {
      setDraggable (TRUE);

      cd = &parent_cd;
      image = &arrow_image;
    }

  ~ArrowHotRegion () {}

protected:
  ComponentDisplay *cd;
  Image            *image;

  // This implements a function that detects a events in a region that is
  // roughly in the shape of an arrow in the center of the ComponentDisplay.
  // If the ComponentDisplay is resized, it will calculate the new region
  // location properly.
  virtual Boolean defaultDetectionFunction (int x, int y)
    {
      Boolean result = FALSE;

      int x_loc = (cd->getWidth () - image->getWidth ()) / 2;
      int y_loc = (cd->getHeight () - image->getHeight ()) / 2;

      if ((x >= x_loc) && (y >= y_loc) &&
	  (x <= x_loc+65) && (y <= y_loc+65)) {
	int x_delta = x - x_loc;
	int y_delta = y - y_loc;

	if ((x_delta + y_delta >= 48) &&
	    (x_delta + y_delta <= 82))
	  result = TRUE;
      }

      return result;
    }

private:
  // "setDraggable" was demoted to a private member function so that it
  // could not be called externally.  The only place it is called is in the
  // constructor of this class.
  void setDragabble (Boolean flag)
    {
      HotRegion::setDraggable (flag);
    }
};

#endif
