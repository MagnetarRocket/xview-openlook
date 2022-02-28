/*H+ HotRegion.h
*
*    SCCS ID: @(#)HotRegion.h 1.4 93/06/01 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE: 
*
*    Defines the HotRegion class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       02/10/91   Original creation
*    JDZ       04/29/91   Preparation for code review
*
*H-*/

/*C+ HotRegion
*
*    DESCRIPTION:
*
*    The HotRegion class allows specification of geometrical regions
*    that are sensitive to events.  The default geometrical region
*    is a rectangle.  The user can define geometrical regions by specifying
*    a detection function of their own.
*
*    INHERITED CLASSES:
*
*    Generic - Inheriting the Generic class will allow the HotRegion class
*    to be treated as a Generic object. Therefore, a GenericHash or
*    GenericList class can contain a HotRegion.
*
*    ORDINARY DATA MEMBERS:
*
*    int                        hotRegionID       - Holds the user specified
*    ID of the hot region.
*
*    Boolean                    transparent       - FALSE if this region
*    blocks events going to the region below it.
*
*    UIObject                  *hotRegionObject   - Holds the pointer of the 
*    object this region is attached to.
*
*    UICHandlerFunction         regionHandler     - User defined function to 
*    execute when an event occurs in this hot region.
*
*    HotRegionDetectionFunction detectionFunction - User defined function to 
*    detect if an event is in the hot region.  The user's function must
*    receive the X-Y coordinates of the event and return TRUE or FALSE.
*
*    Boolean			useDefaultRegionHandlerFlag - TRUE if the
*    default region handler should be used.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    HotRegion (int     id,
*	        int     default_x,
*	        int     default_y,
*	        int     default_width,
*	        int     default_height,
*	        Boolean transparent_flag = FALSE) - Constructor that
*    allows the setting of an ID, the default rectangle region and whether
*    or not this hot region is transparent.
*
*    void      setID (int) - Sets the ID of the hot region.
*    Default: uiObjectID (a unique ID).
*
*    int       getID () - Gets the ID of the hot region.
*
*    void      setDefaultRectangle (int default_x,
*				    int default_y,
*				    int default_width,
*				    int default_height) - Sets
*    the default rectangle that defines the HotRegion.  Default: Depends
*    on constructor.
*
*    void      getDefaultRectangle (int &default_x,
*				    int &default_y,
*				    int &default_width,
*				    int &default_height) - Gets
*    the default rectangle that defines the HotRegion.
*
*    void      setTransparent (Boolean transparent_flag)         - Sets the 
*    transparent rectangle that defines the HotRegion (i.e., it blocks
*    events going to the region below it).  Default: FALSE.
*
*    void      setDetectionFunction (HotRegionDetectionFunction) - Sets the
*    detectionFunction on this HotRegion object.  Default: Rectangular
*    region detection.
*
*    void      setRegionHandler (UICHandlerFunction)             - Sets the
*    function that when an event occurs in this hot region.  Default: Not set.
*
*    UIObject *getHotRegionObject ()                             - Returns the
*    pointer of the object this hot region is attached to.
*
*    Boolean   isTransparent ()                                  - TRUE if
*    this HotRegion object is transparent (i.e., it blocks events going to the 
*    region below it).
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    void useDefaultRegionHandler (Boolean) - Specifies if the default
*    region handler should be used.
*
*    virtual void defaultRegionHandler () - The default region handler.
*
*    virtual Boolean defaultDetectionFunction (int x, int y) -  Checks if
*    the X-Y coordinates fall within the default rectangular region.  Can
*    be overloaded in derived classes to create new types of hot regions.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    Boolean detectHotRegionEvent (UIObject *, Event *) - TRUE if the specified
*    XView event took place in this HotRegion object.
*
*    void setHotRegionDefaults () - Initializes the data members.
*
*    FRIEND CLASSES:
*
*    CanvasShellDisplay - ComponentDisplay is a friend of HotRegion
*    so that it can access createXViewDropSite and createXViewDragSite.
*
*    ComponentDisplay - ComponentDisplay is a friend of HotRegion
*    so that it can access createXViewDropSite and createXViewDragSite.
*
*    UIComponent - UIComponent is a friend of HotRegion so that it can access
*    createXViewDropSite and createXViewDragSite.
*
*    UIDisplay - UIDisplay is a friend of HotRegion so that it can access
*    createXViewDropSite and createXViewDragSite.
*
*    UIObject - UIObject is a friend of HotRegion so that drag and
*    drop can de fully supported for hot regions.  The actual events
*    that are used to detect drops and initiate drags occur on the
*    object that the hot region is added to.  The addHotRegion
*    member function and hot region event detection is in UIObject.
*
*C-*/
#ifndef _HOTREGION_H
#define _HOTREGION_H

// Includes
#include "UIObject.h"

// Type Definitions
typedef Boolean (* HotRegionDetectionFunction) (HotRegion *, int, int);

// Class Definition
class HotRegion : public UIObject {
public:
  HotRegion ()
    {
      setHotRegionDefaults ();
    }

  HotRegion (int id)
    {
      setHotRegionDefaults ();
      hotRegionID = id;
    }

  HotRegion (int     id,
	     int     default_x,
	     int     default_y,
	     int     default_width,
	     int     default_height,
	     Boolean transparent_flag = FALSE)
    {
      setHotRegionDefaults ();
      xLoc = default_x;
      yLoc = default_y;
      width = default_width;
      height = default_height;
      hotRegionID = id;
      transparent = transparent_flag;
    }

  ~HotRegion () {}

  void      setID (int id) { hotRegionID = id; }
  int       getID () { return hotRegionID; }

  void      setDefaultRectangle (int default_x,
				 int default_y,
				 int default_width,
				 int default_height)
    {
      xLoc = default_x;
      yLoc = default_y;
      width = default_width;
      height = default_height;

      if (dropSite) {
	Rect rectangle;
	rect_construct (&rectangle, xLoc, yLoc, width, height);
        changeDropSiteSize (&rectangle);
      }
    }

  void      getDefaultRectangle (int &default_x,
				 int &default_y,
				 int &default_width,
				 int &default_height)
    {
      default_x = xLoc;
      default_y = yLoc;
      default_width = width;
      default_height = height;
    }

  void      setTransparent (Boolean transparent_flag)
    { transparent = transparent_flag; }

  void      setDetectionFunction (HotRegionDetectionFunction); 
  void      setRegionHandler (UICHandlerFunction);

  UIObject *getHotRegionObject () { return hotRegionObject; }

  Boolean   isTransparent () { return transparent; }

protected:
  int                 hotRegionID;
  Boolean             transparent;
  UIObject           *hotRegionObject;
  UICHandlerFunction  regionHandler;
  Boolean	      useDefaultRegionHandlerFlag;

  void useDefaultRegionHandler (Boolean flag)
    { useDefaultRegionHandlerFlag = flag; }

  virtual void defaultRegionHandler () {}

  HotRegionDetectionFunction detectionFunction;

  virtual Boolean defaultDetectionFunction (int x, int y)
    {
      Boolean result = FALSE;

      if ((xLoc == UIC_NOT_SET) &&
	  (yLoc == UIC_NOT_SET) &&
	  (width == UIC_NOT_SET) &&
	  (height == UIC_NOT_SET) &&
	  (transparent == FALSE))
        result = FALSE;
      else if (((x >= xLoc) && (x <= xLoc + width)) && 
               ((y >= yLoc) && (y <= yLoc + height)))
	result = TRUE;

      return result;
    }

private:
  Boolean detectHotRegionEvent (UIObject *, Event *);

  void setHotRegionDefaults ()
    { 
      hotRegionID = uiObjectID;
      transparent = FALSE;
      hotRegionObject = (UIObject *) NULL;
      regionHandler = (UICHandlerFunction) NULL;
      useDefaultRegionHandlerFlag = FALSE;
      detectionFunction = (HotRegionDetectionFunction) NULL;

      isHotRegion = TRUE;
    }

friend class CanvasShellDisplay;
friend class ComponentDisplay;
friend class UIComponent;
friend class UIDisplay;
friend class UIObject;
};

#endif /* _HOTREGION_H */
