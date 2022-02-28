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
  ArrowHotRegion () { setDraggable (TRUE); }

protected:
  virtual Boolean defaultDetectionFunction (int x, int y)
    {
      Boolean result = FALSE;

      UIObject *cd = (UIObject *) getObjectData ("DRAG_CD");
      UIObject *image = (UIObject *) getObjectData ("DRAG_IMAGE");

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
};

#endif
