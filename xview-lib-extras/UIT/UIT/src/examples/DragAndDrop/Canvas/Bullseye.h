/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#ifndef _BULLSEYE_H
#define _BULLSEYE_H

#include "ComponentDisplay.h"
#include "UIComponent.h"

class Bullseye : public UIComponent {
public:
  Bullseye ()
    {
      // "isDrawableOnCanvas" is a special attribute variable provided
      // for creation of new UIT components.  If set to TRUE, then the
      // component can be drawn on a ComponentDisplay without forcing the
      // ComponentDisplay to be created as an XView panel.  This allows the
      // the creation of components that can be used on an XView canvas.
      isDrawableOnCanvas = TRUE;

      // "dropSiteRegion" and "dragSiteRegion" are special attribute
      // variables that specify the HotRegion objects that are to
      // govern the draggable and droppable regions of the new component.
      // Usually they will refer to the exact same HotRegion object and
      // the HotRegion object will be the same size and at the smae position
      // of any images that are rendered as part of this component.
      dropSiteHotRegion = &hotRegion;
      dragSiteHotRegion = &hotRegion;

      // Since this component should always accept drops, "setAcceptDrops"
      // is demoted to a private member function and is called in the
      // constructor.
      setAcceptDrops (TRUE);

      // Creates an Image object that contains an X bit map file
      // representing the bullseye image under normal conditions.
      bullseyeImage = new Image ("./bullseye.bm");
      bullseyeImage->setBitMapFile (TRUE);

      // Creates an Image object that contains an X bit map file
      // representing the bullseye image while a user is interacting
      // with the component.
      inverseBullseyeImage = new Image ("./inverse_bullseye.bm");
      inverseBullseyeImage->setBitMapFile (TRUE);

      // By setting the X and Y values of UIObject accurately, a user may
      // have access to their values via the "getX" and "getY" member
      // functions without having to define "getX" and "getY" at this level.
      // This class will not provide accurate width and height information
      // via "getWidth" and "getHeight", but could by using a similar
      // technique.
      UIObject::setX (dropX = 0);
      UIObject::setY (dropY = 0);

      inverted = FALSE;
      lastMode = inverted;
      gc = (GC) NULL;
    }

  ~Bullseye ()
    {
      if (bullseyeImage) delete bullseyeImage;
      if (inverseBullseyeImage) delete inverseBullseyeImage;

      if (gc) {
	Display *display = ((ComponentDisplay *) parentObject)->getXDisplay ();
	if (display) XFreeGC (display, gc);
      }
    }

  void setX (int x)
    {
      int old_x = dropX;
      UIObject::setX (dropX = x);

      if (showFlag) {
	// If this object has been created (createObject was called),
	// then actually render the change in the X coordinate of the
        // object.
	if (objectCreated) {
	  Display *display = ((ComponentDisplay *)
			      parentObject)->getXDisplay ();
	  Window   window = ((ComponentDisplay *)
			     parentObject)->getXWindow ();

	  // Clear the old location of the object
	  XClearArea (display,   window,
		      old_x,     dropY,
		      dropWidth, dropHeight,
		      False);
	  
	  // Render the object at the new location
	  moveComponent ();
	}
      }
    }

  void setY (int y) 
    {
      int old_y = dropY;
      UIObject::setY (dropY = y);

      if (showFlag) {
	// If this object has been created (createObject was called),
	// then actually render the change in the Y coordinate of the
        // object.
	if (objectCreated) {
	  Display *display = ((ComponentDisplay *)
			      parentObject)->getXDisplay ();
	  Window   window = ((ComponentDisplay *)
			     parentObject)->getXWindow ();

	  // Clear the old location of the object
	  XClearArea (display,   window,
		      dropX,     old_y,
		      dropWidth, dropHeight,
		      False);

	  // Render the object at the new location
	  moveComponent ();
	}
      }
    }

  // If the user of the object tries to change the drop rectangle location,
  // reflect the change by moving the component to the new location specified.
  // Ignore the height and width information becuase we want the drop
  // rectangle to always be the size of the component.  Since the component
  // contains the drag and drop information in its hot region, all we really
  // need to do is make sure that "setX" and "setY" are called properly, which
  // is what "setLocation" does.
  void setDropRectangle (int x, int y, int width, int height)
    {
      setLocation (x, y);
    }

  void show (Boolean flag)
    {
      showFlag = flag;

      if (objectCreated) {
        if (showFlag) {
          repaintComponent ((ComponentDisplay *) parentObject);
        }
        else {
          Display *display = ((ComponentDisplay *)
			      parentObject)->getXDisplay ();
          Window   window = ((ComponentDisplay *) parentObject)->getXWindow ();

	  // Clear the old location of the object
          XClearArea (display,   window,
		      dropX,     dropY,
 		      dropWidth, dropHeight,
		      False);
        }
      }
    }

protected:
  // Special data member that defines the component's hot region.  The hot
  // region specifies the ComponentDisplay real estate that is of interest to
  // the component.
  HotRegion  hotRegion;

  Image     *bullseyeImage;
  Image     *inverseBullseyeImage;

  int	     dropX;
  int	     dropY;
  int	     dropWidth;
  int	     dropHeight;
  Boolean    inverted;
  Boolean    lastMode;
  GC	     gc;

  // This is a special member function that is called by the UIT whenever the
  // component needs to rendered.
  void repaintComponent (ComponentDisplay *parent)
    {
      if (showFlag) {
	// Get the display and window information about the parent
	// ComponentDisplay.
	Display *display = parent->getXDisplay ();
	Window   window = parent->getXWindow ();
	
	// Only set the stipple if we absolutely need to...  If the last mode
	// of the object does not match the current state of the object...
	if (lastMode != inverted) {
	  if (inverted)
	    XSetStipple (display, gc, (Pixmap) inverseBullseyeImage->getXID ());
	  else  
	    XSetStipple (display, gc, (Pixmap) bullseyeImage->getXID ());
	  
	  lastMode = inverted;
	}
	
	// Clear the area where the object is located and render the object
	XClearArea (display, window, dropX, dropY, dropWidth, dropHeight, False);
	XSetTSOrigin (display, gc, dropX, dropY);
	XFillRectangle (display, window, gc, dropX, dropY, dropWidth, dropHeight);
      }
    }

  // This is a special member function that is called by the UIT whenever
  // an event occurs on top of the component's hot region.
  void componentHotRegionEventProc (ComponentDisplay *parent)
    {
      if (showFlag) {
	InputEvent *event = parent->getEvent ();
	
	switch (event->getAction ()) {
	  // If the event action is ACTION_DRAG_PREVIEW, then we are getting
	  // a chance to preview a drag and drop operation before it starts.
	case ACTION_DRAG_PREVIEW:
	  switch (event->getID ()) {
	    // The user has entered this component's hot region, so invert
	    // the component's image.
	  case LOC_WINENTER:
	    inverted = TRUE;
	    break;
	    
	    // The user has exited the component's parent ComponentDisplay
	    // and somehow the event was detected by the hot region.  This
	    // case should never occur, but better safe then sorry.
	  case LOC_WINEXIT:
	    inverted = FALSE;
	    break;
	    
	  default:
	    break;
	  }
	  break;
	  
	default:
	  inverted = FALSE;
	  break;
	}
	
	// If the event causes the component's image to change from what it
	// currently is, render the new image.
	if (lastMode != inverted) repaintComponent (parent);
      }
    }

  // This is a special member function that is called by the UIT whenever
  // an event occurs in the background of this component's parent
  // ComponentDisplay, but not on the component itself.
  void componentBackgroundEventProc (ComponentDisplay *parent)
    {
      if (showFlag) {
	// We are not on top of the component, make sure the component image
	// is not inverted.
	inverted = FALSE;
	
	// If the event causes the component's image to change from what it
	// currently is, render the new image.
	if (lastMode != inverted) repaintComponent (parent);
      }
    }

  // This is a special member function that is called by the UIT when
  // the XView objects are being created.  This createObject in this
  // component is called when the objects in the parent ComponentDisplay
  // are being created.
  void createObject (UIObject *parent)
    {
      ComponentDisplay *cd = (ComponentDisplay *) parent;

      Display *display = cd->getXDisplay ();
      Window   window = cd->getXWindow ();

      // Create the XView images.  This needed to be done by this component
      // because they velong to this component.  If the images belonged
      // to another component or object that the UIT governed, then the
      // images would be created automatically.
      ((UIObject *) bullseyeImage)->createObject (parent);
      ((UIObject *) inverseBullseyeImage)->createObject (parent);

      // Create the graphics context that will be used for rendering
      // the images.
      XGCValues gcv;
      gcv.stipple = (Pixmap) bullseyeImage->getXID ();
      gcv.foreground = BlackPixel (display, XDefaultScreen (display));
      gcv.background = WhitePixel (display, XDefaultScreen (display));
      gcv.fill_style = FillStippled;

      gc = XCreateGC (display, window,
		      GCStipple | GCForeground | GCBackground | GCFillStyle,
		      &gcv);

      // Initialize all of the internal location and size information.  The
      // width and height were not known until the XView images were created.
      UIObject::setX (dropX);
      UIObject::setY (dropY);
      dropWidth  = bullseyeImage->getWidth ();
      dropHeight = bullseyeImage->getHeight ();
      hotRegion.setDefaultRectangle (dropX, dropY, dropWidth, dropHeight);

      // Register the component's HotRegion with its parent ComponentDisplay
      cd->addComponentHotRegion (*this, hotRegion, TRUE);

      // Register the component to be repainted by its parent ComponentDisplay
      cd->addRepaintComponent (*this);

      // This should usually be called by all components.  It processes
      // any attributes that were set on this object by member functions
      // that were inherited from superclasses.  It also sets information
      // Required to do proper drag and drop operations.
      setUIObjectAttributes (parent);
    }

  private:
    // Helper function for moving the component after a location change has
    // been made.
    void moveComponent ()
    {
      // Render the component at its new location.
      repaintComponent ((ComponentDisplay *) parentObject);

      // Move the component's hot region to the new location
      hotRegion.setDefaultRectangle (dropX, dropY, dropWidth, dropHeight);
    }

    // To make sure the user of this class cannot accidently call
    // setAcceptDrops, make it a private member function.
    void setAcceptDrops (Boolean flag)
    {
      UIObject::setAcceptDrops (flag);
    }
};

#endif
