/*H+ CanvasShellDisplay.h
*
*    SCCS ID: @(#)CanvasShellDisplay.h 1.2 93/06/01 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CanvasShellDisplay class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CanvasShellDisplay
*
*    DESCRIPTION:
*
*    In addition to having the the XView canvas abilities of a
*    ComponentDisplay, it also provides a display for the various
*    Sling Shot components.
*
*    INHERITED CLASSES:
*
*    UIDisplay - Supplies basic display capabilities.
*
*    LOCAL CLASSES:
*
*    ComponentHotRegion - This class is used to store information about
*    user-defined components that are being displayed in the CanvasShellDisplay.
*
*             ORDINARY DATA MEMBERS:
*
*             UIComponent   *component - Pointer to the UIComponent object 
*             associated with this HotRegion object.
*
*             HotRegion     *hotRegion - Pointer to the HotRegion object
*             associated with this UIComponent object. These two pointers
*             create a two-way link between UIComponent and HotRegion objects.
*
*             Boolean        trapBackgroundEvents - TRUE if background events
*             are to be trapped by this HotRegion object.
*
*    ORDINARY DATA MEMBERS:
*
*    Boolean		 dynamic             - TRUE if this object has a
*    dynamic color map instead of a static color map.
*
*    Boolean		 bufferRepaint	     - TRUE if this object is to
*    buffer its repaints.
*
*    Boolean		 delayRepaint	     - Don't flush draws to display
*    while TRUE.
*
*    int		 bitGravity	     - Sepecifies the X11 bit gravity.
*
*    GenericList       	 componentList       - Lists all components associated
*    with this object.
*
*    GenericList	 repaintComponents   - Lists all user-defined
*    components associated with this CanvasShellDisplay that are to be repainted.
*
*    GenericList 	 componentHotRegions - Lists all HotRegions associated
*    with user-defined components in this CanvasShellDisplay.
*
*    Boolean		 componentHotRegion  - TRUE if the last event
*    occurred in the hot region of a user-defined component that is displayed
*    in this CanvasShellDisplay.
*
*    Boolean		 dropComponents	     - TRUE if there are components
*    on this display that will accept drops.
*
*    int		 numScrollers        - The number of Scrollers
*    associated with this CanvasShellDisplay.
*
*    int		 numCanvasComponents - The number of components
*    associated with display that are drawable on an XView canvas.
*
*    DrawingModel 	 model               - The drawing model of this
*    CanvasShellDisplay.
*
*    ColorMap		*paintColorMap       - The color map used to paint this
*    component object.
*
*    UIMenu             *displayMenu         - The display menu associated
*    with this component object.
*
*    Orientation	 layoutDirection     - The layout direction of
*    components added to this CanvasShellDisplay.
*
*    Boolean             fitHeight           - TRUE if this component's height
*    is to fit the height of the window surrounding it.
*
*    Boolean             fitWidth            - TRUE if this component's width
*    is to fit the width of the window surrounding it.
*
*    int		 paintHeight         - Height of the area to be
*    painted.
*
*    int		 paintWidth          - Width of the area to be painted.
*
*    Boolean		 autoExpand          - TRUE if this component is to 
*    be expanded to fit the size of the window surrounding it.
*
*    Boolean		 eventHandlerSet     - TRUE if the event handler is
*    set for this component.
*
*    Boolean		 repaintHandlerSet   - TRUE if the event handler is set
*    for the component when it is determined that it has to be repainted.
*   
*    UICHandlerFunction	 repaintHandler      - Function that is to be executed 
*    when a "repaint" event is noted in this component.
*
*    Xv_window           paintWindow         - The XView canvas paint window.
*
*    Display            *canvasXDisplay      - The X11 display pointer.
*
*    Window              canvasXWindow       - The X11 XID.
*
*    Xv_xrectlist	*canvasXRectlist     - The pointer to clip rectangles
*    of areas that need to be repainted if the repaint proc is called and
*    the drawing model is X11.
*
*    Rectlist		*canvasRectlist      - The pointer to clip rectangles
*    of areas that need to be repainted if the repaint proc is called and
*    the drawing model is XView.
*
*    Boolean             useDefaultRepaintHandlerFlag - TRUE if the default
*    repaint handler should be used.
*
*    Boolean             useDefaultResizeHandlerFlag - TRUE if the default
*    resize handler should be used.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void 	    addComponent (UIComponent &) - Adds the component to the 
*    list of components that are contained in the CanvasShellDisplay.
*
*    void 	    addComponent (CSComponent &) - Adds the component to the 
*    list of components that are contained in the CanvasShellDisplay.
*
*    void 	    removeComponent (UIComponent &) - Removes the component
*    from the CanvasShellDisplay.
*
*    void 	    removeComponent (CSComponent &) - Removes the component
*    from the CanvasShellDisplay.
*
*    UIComponent   *traverseComponents (Boolean reset_flag) - Traverse
*    the components that belong to this CanvasShellDisplay.
*
*    void	    setDynamic (Boolean) - specifies whether or not this
*    display can have a dynamic color map.  Default: FALSE.
*
*    void	    setBufferRepaint (Boolean) - If set, an offscreen
*    pixmap is used to buffer repaints.  This reduces flashing considerably.
*    Default: FALSE.
*
*    void	    setDelayRepaint (Boolean) - Prohibits repainting until
*    it is set to FALSE again.  This allows multiple draws to the display
*    to be delayed and flushed at one time.
*
*    void	    setBitGravity (int) - Specifies the X11 bit gravity.
*    Default: NortWestGravity.
*
*    void           setPaintColorMap (ColorMap &) - Specifies the color map
*    to be used for drawing on the display.  Default: Use application color
*    map.
*
*    virtual void   setForegroundColor (Color)   - Finds the associated 
*    index of the Color and sets the foregroundColor to this value. 
*    Default: Not set.
*
*    Color is a UIC color data type. It has the following data members:
*    int red - Contains the red color component,
*    int green - Contains the green color component,
*    int blue - Contains the blue color component.
*
*    virtual void   setForegroundColor (char *)  - Finds the associated 
*    index of ther named color and sets the foregroundColor to this value. 
*    Default: Not set.
*
*    virtual void   setBackgroundColor (Color)   -  Finds the associated 
*    index of the Color and sets the backgroundColor to this value. 
*    Default: Not set.
*
*    virtual void   setBackgroundColor (char *)  - Finds the associated 
*    index of ther named color and sets the backgroundColor to this value. 
*    Default: Not set.
*
*    void           setFit (Dimension) - Depending on the value of the 
*    Dimension argument (HEIGHT and WIDTH), this specifies that the 
*    CanvasShellDisplay should be fit to size around the components it 
*    contains in the specified Dimension.  Default: Don't fit the display.
*
*    void           setLayoutDirection (Orientation) - Specifies the layout
*    direction of components added to this display.  Default: HORIZONTAL.
*
*    void	    setPaintHeight (int) - Specifies the height of the window
*    beyond what can be seen.  The rest of the window can be scrolled to
*    with Scrollers.  Default: Not set.
*
*    void	    setPaintWidth (int) - Specifies the width of the window
*    beyond what can be seen.  The rest of the window can be scrolled to
*    with Scrollers.  Default: Not set.
*
*    void	    setAutoExpand (Boolean) - Sets the  CanvasShellDisplay to 
*    autoexpand to any painting that might occur on it.  Default: FALSE.
*
*    void	    setDrawingModel (DrawingModel) - Sets the drawing model 
*    of this  CanvasShellDisplay.  Default: Users XView drawing model. 
*    DrawingModel  is a data type that has the following values: XVIEW - 
*    Specifies the XView drawing model; X11 - Specifies the X11 drawing 
*    model.  Default: XView.
*    
*    void	    setRepaintHandler (UICHandlerFunction) - Sets the function 
*    that is to be executed when the CanvasShellDisplay needs to be repainted.
*    Default: Not set.
*
*    void	    setResizeHandler (UICHandlerFunction) - Sets the function 
*    that is to be executed when the CanvasShellDisplay is resized.
*    Default: Not set.
*
*    void           setMenu (UIMenu&) - Sets the menu that is to appear 
*    when the user selects the right mouse button and the arrow is within 
*    the CanvasShellDisplay boundaries.  Default: No menu associated.
*
*    UIMenu        *getMenu () - Gets the menu that is to appear when 
*    the user selects the right mouse button and the arrow is within
*    the CanvasShellDisplay boundaries.
*
*    Xv_window      getPaintWindow () - Returns the XView Canvas Paint 
*    Window.
*
*    Display       *getXDisplay () - Returns a pointer to an X11 Display 
*    structure for the CanvasShellDisplay.
*
*    Window         getXWindow () - Returns the X11 XID of the CanvasShellDisplay.
*
*    Xv_xrectlist  *getXRectlist () - Gets the clip rectangles list for
*    areas that need to be repainted.  This is supplied if the drawing model
*    is X11 or POSTSCRIPT.
*
*    Rectlist	   *getRectlist () - Gets the clip rectangles list for
*    areas that need to be repainted.  This is supplied if the drawing model
*    is XView.
*
*    void	    drawXImagePlane (Image &image,
*				     int x, int y,
*				     Boolean transparent=FALSE,
*				     GC gc=NULL) - 	
*    Draws one plane of the specified Image at the specified coordinates.
*    If the "transparent" boolean is set, then off bits will be transparent.
*    Also, the caller may specify an X11 graphics context to determine
*    the background and foreground colors of the image.
*
*    void           addRepaintComponent (UIComponent&) - Adds a user-defined
*    component to the RepaintComponent list associated with this
*    CanvasShellDisplay.
*
*    void           addComponentHotRegion (UIComponent&, HotRegion&,
*                                          Boolean) - Adds a user-defined
*    component HotRegion to the Component HotRegion list associated with
*    this CanvasShellDisplay.
*
*    void	    setFont (CharacterFont&) - Sets the font for objects
*    that are children of this cnavas shell.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void  createObject (UIObject *) - Create XView object.
*
*    void	   repaintComponentsProc () - Internal function that calls
*    user defined repaint procs for user-defined components displayed in
*    this CanvasShellDisplay.
*
*    static void   repaintProc (Canvas, Xv_Window, Rectlist *) - Internal
*    wrapper around the XView repaint proc for the XView drawing model.
*
*    void useDefaultRepaintHandler (Boolean) - Specifies whether or not to
*    use the default repaint handler.
*
*    void useDefaultResizeHandler (Boolean) - Specifies whether or not to
*    use the default resize handler.
*
*    virtual void defaultRepaintHandler () - The default repaint handler.
*
*    virtual void defaultResizeHandler () - The default resize handler.
*
*    static void   repaintXProc (Canvas,
*			         Xv_Window,
*				 Display *,
*				 Window, 
*                                Xv_xrectlist *) - Internal wrapper around
*    the XView repaint proc for the X11 drawing model.
*
*    static void   resizeProc (Canvas, int, int) - Internal wrapper around
*    the XView resize proc for a canvas.
*
*    static void eventProc (Xv_window, Event *) - Internal wrapper 
*    around the XView event proc.
*
*    virtual void createDragSite () - Creates a drag site for this object.
*
*    virtual void createDropSite () - Creates a drop site for this object.
*
*    virtual void createHotRegionDragSite (HotRegion&) - Creates a
*            drag site for this object using the specified hot region.
*
*    virtual void createHotRegionDropSite (HotRegion&) - Creates a
*            drop site for this object using the specified hot region.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCanvasShellDisplayDefaults () - Initializes data members.
*
*C-*/

#ifndef _CANVAS_SHELL_H
#define _CANVAS_SHELL_H

// Includes
#include "CharacterFont.h"
#include "ColorMap.h"
#include "GenericList.h"
#include "HotRegion.h"
#include "UIComponent.h"
#include "UIDisplay.h"
#include "UIMenu.h"
#include "CSComponent.h"

#include <xview/win_input.h>
#include <xview/rect.h>
#include <pixrect/pixrect.h>
#include <pixrect/pixfont.h>
#include <xview/frame.h>
#include <xview/scrollbar.h>
#include <xview/xv_xrect.h>
#include <sspkg/canshell.h>

// Local Class Definitions
class CSComponentHotRegion : public Generic {
public:
  UIComponent *component;
  HotRegion   *hotRegion;
  Boolean      trapBackgroundEvents;
};

// Class Definition
class CanvasShellDisplay : public UIDisplay {
public:
    		 	 CanvasShellDisplay ();
    		 	 CanvasShellDisplay (Boolean);
			~CanvasShellDisplay ();

    void 	 	 addComponent (UIComponent &);
    void 	 	 addComponent (CSComponent &);

    void 	 	 removeComponent (UIComponent &);
    void 	 	 removeComponent (CSComponent &);

    UIComponent		*traverseComponents (Boolean reset_flag)
			 { return (UIComponent *)
				  componentList.traverse (reset_flag); }

    void		 setDynamic (Boolean flag)
			 { dynamic = flag; }

    void 		 setBufferRepaint (Boolean flag)
			 {
			   bufferRepaint = flag;
			   if (owObject) {
			     xv_set (owObject,
				     CANVAS_SHELL_BATCH_REPAINT, bufferRepaint,
				     NULL);
			   }
			 }

    void 		 setDelayRepaint (Boolean flag)
			 {
			   delayRepaint = flag;
			   if (owObject) {
			     xv_set (owObject,
				     CANVAS_SHELL_DELAY_REPAINT, delayRepaint,
				     NULL);
			   }
			 }

    void		 setBitGravity (int bit_gravity)
			 { bitGravity = bit_gravity; }

    void		 setPaintColorMap (ColorMap &color_map)
			 { paintColorMap = &color_map;
			   if (owObject) {
			     ((UIObject *) paintColorMap)->createObject (this);

			     xv_set (owObject,
				     WIN_CMS, paintColorMap->getXViewObject (),
				     NULL);

			     if (owObject != paintWindow)
			       xv_set (paintWindow,
				       WIN_CMS,
				       paintColorMap->getXViewObject (),
				       NULL);
			   }
			 }

    virtual void	 setForegroundColor (Color);
    virtual void	 setForegroundColor (char *);

    virtual void	 setBackgroundColor (Color);
    virtual void	 setBackgroundColor (char *);

    void                 setFit (Dimension fit)
			 { if (fit == HEIGHT) {
			     fitHeight = TRUE;
			     if (owObject)
			       window_fit_height (owObject);
			   }
			   else {
			     fitWidth = TRUE;
			     if (owObject)
			       window_fit_width (owObject);
			   }
			 }

    void		 setPaintHeight (int height)
			 { paintHeight = height;
			   if (owObject) {
			     if (resourceName)
			       xv_set (owObject,
				       XV_USE_DB,
				         CANVAS_HEIGHT, paintHeight,
				         NULL,
				       NULL);
			     else
			       xv_set (owObject,
				       CANVAS_HEIGHT, paintHeight,
				       NULL);
			     xv_set (owObject, CANVAS_AUTO_SHRINK, FALSE, NULL);

			     if (!autoExpand || (autoExpand == UIC_NOT_SET))
			       xv_set (owObject,
				       CANVAS_AUTO_EXPAND, FALSE,
				       NULL);
			   }
		         }

    void		 setPaintWidth (int width)
			 { paintWidth = width;
			   if (owObject) {
			     if (resourceName)
			       xv_set (owObject,
				       XV_USE_DB,
				         CANVAS_WIDTH, paintWidth,
				         NULL,
				       NULL);
			     else
			       xv_set (owObject,
				       CANVAS_WIDTH, paintWidth,
				       NULL);
			     xv_set (owObject, CANVAS_AUTO_SHRINK, FALSE, NULL);

			     if (!autoExpand || (autoExpand == UIC_NOT_SET))
			       xv_set (owObject,
				       CANVAS_AUTO_EXPAND, FALSE,
				       NULL);
			   }
			 }

    void		 setAutoExpand (Boolean flag)
			 { autoExpand = flag;
			   if (owObject)
			     xv_set (owObject, CANVAS_AUTO_EXPAND, flag, NULL);
		         }

    void	 	 setDrawingModel (DrawingModel new_model)
			 { if (!owObject) model = new_model; }

    void		 setRepaintHandler (UICHandlerFunction);
    void		 setResizeHandler (UICHandlerFunction);

    void                 setMenu (UIMenu&);
    UIMenu              *getMenu ();

    Xv_window            getPaintWindow () { return paintWindow; }
    Display             *getXDisplay () { return canvasXDisplay; }
    Window               getXWindow () { return canvasXWindow; }

    Xv_xrectlist	*getXRectlist () { return canvasXRectlist; }
    Rectlist		*getRectlist () { return canvasRectlist; }

    void		 drawXImagePlane (Image  &image,
					  int      x,
					  int      y,
					  Boolean  transparent = FALSE,
					  GC       gc = (GC) NULL)
      {
	((UIObject *) &image)->createObject (this);

	int w = image.getWidth ();
	int h = image.getHeight ();

	UICGeneric  xview_image = image.getXViewObject ();
	XID         xid = (XID) xv_get (xview_image, XV_XID);
	Display    *display = getXDisplay ();

	Boolean free_gc = FALSE;
	if (!gc) {
	  XGCValues gc_values;
	  gc_values.foreground = 1;
	  gc_values.background = 2;
	  gc = XCreateGC (display,
		          getXWindow (),
			  GCForeground | GCBackground,
		          &gc_values);

	  free_gc = TRUE;
	} 

	if (transparent) {
	  XImage *x_image = XGetImage (display, xid, 0, 0, w, h, 1, XYPixmap);

	  for (int i=0; i < w; i++)
	    for (int j=0; j < h; j++)
	      if (XGetPixel (x_image, i, j))
	        XDrawPoint (display, getXWindow (), gc, x+i, y+j);
	}
	else
	  XCopyPlane (display, xid, getXWindow (), gc, 0, 0, w, h, x, y, 1);

	if (free_gc) XFreeGC (display, gc);
      }

    void                 addRepaintComponent (UIComponent &component)
			 { repaintComponents.addItem (&component); }

    void		 addComponentHotRegion (UIComponent &component,
						HotRegion   &hot_region,
						Boolean      flag = FALSE)
			 { CSComponentHotRegion *chr = new CSComponentHotRegion;
			   chr->component = &component;
			   chr->hotRegion = &hot_region;
			   chr->trapBackgroundEvents = flag;

			   componentHotRegions.addItem (chr); }

    void		 setFont (CharacterFont&);

protected:
    Boolean		 dynamic;
    Boolean		 bufferRepaint;
    Boolean		 delayRepaint;
    int			 bitGravity;
    GenericList	 	 componentList;
    GenericList	 	 repaintComponents;
    GenericList	 	 componentHotRegions;
    Boolean		 componentHotRegion;
    Boolean		 dropComponents;
    int			 numScrollers;
    int			 numCanvasComponents;
    DrawingModel 	 model;
    ColorMap		*paintColorMap;
    UIMenu              *displayMenu;
    Orientation		 layoutDirection;
    Boolean              fitHeight;
    Boolean              fitWidth;
    int			 paintHeight;
    int			 paintWidth;
    Boolean		 autoExpand;
    Boolean		 eventHandlerSet;
    Boolean		 repaintHandlerSet;
    UICHandlerFunction	 repaintHandler;
    UICHandlerFunction   resizeHandler;
    Xv_window            paintWindow;
    Display             *canvasXDisplay;
    Window               canvasXWindow;
    Xv_xrectlist	*canvasXRectlist;
    Rectlist		*canvasRectlist;
    Boolean		 useDefaultRepaintHandlerFlag;
    Boolean		 useDefaultResizeHandlerFlag;

    virtual void 	 createObject (UIObject *);

    void		 repaintComponentsProc ()
      { int num_components = repaintComponents.getSize ();
	
	for (int i=0; i < num_components; i++)
	  ((UIComponent *)
	   repaintComponents [i])->repaintComponent ((ComponentDisplay *) this);
      }

    void		 useDefaultRepaintHandler (Boolean flag)
			 { useDefaultRepaintHandlerFlag = flag; }

    void		 useDefaultResizeHandler (Boolean flag)
			 { useDefaultResizeHandlerFlag = flag; }

    virtual void defaultRepaintHandler () {}
    virtual void defaultResizeHandler () {}

    virtual void createDragSite ();
    virtual void createDropSite ();

    virtual void createHotRegionDragSite (HotRegion&);
    virtual void createHotRegionDropSite (HotRegion&);

    static  void repaintProc (Canvas, Xv_Window, Rectlist *);
    static  void repaintXProc (Canvas,
			       Xv_Window,
			       Display *, 
			       Window,
			       Xv_xrectlist *);

    static  void resizeProc (Canvas, int, int);
    static  void eventProc (Xv_window, Event *, Canvas_shell);

private:
    void setCanvasShellDisplayDefaults ();
};

#endif /* _CANVAS_SHELL_H */
