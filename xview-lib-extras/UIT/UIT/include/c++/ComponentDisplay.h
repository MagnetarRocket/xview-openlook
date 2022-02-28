/*H+ ComponentDisplay.h
*
*    SCCS ID: @(#)ComponentDisplay.h 1.5 93/04/09 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the ComponentDisplay class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/08/91   Preparation for code review
*
*H-*/

/*C+ ComponentDisplay
*
*    DESCRIPTION:
*
*    Provides functionality that allows user to cluster and hold all 
*    UIComponents, such as Buttons, Sliders, and Gauges.  It may be useful
*    to think of this object as cork board.
*
*    INHERITED CLASSES:
*
*    UIDisplay - Supplies basic display capabilities.
*
*    MACRO DEFINITIONS: 
*
*    POSTSCRIPT_STRING(string) pprintf (PostScript, \
*					(unsigned char *) string, \
*					strlen (string));
*                              - Writes the string to the NeWS Server.
*
*    LOCAL CLASSES:
*
*    ComponentHotRegion - This class is used to store information about
*    user-defined components that are being displayed in the ComponentDisplay.
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
*    Boolean             isPanel             - TRUE if this object is
*    an XViewn panel.
*
*    Button             *defaultButton       - Contains this object's default
*    button.
*
*    UIComponent        *focusComponent	     - The component that has the
*    input focus.
*
*    Boolean		 dynamic             - TRUE if this object has a
*    dynamic color map instead of a static color map.
*
*    int		 bitGravity	     - Sepecifies the X11 bit gravity.
*
*    GenericList       	 componentList       - Lists all components associated
*    with this object.
*
*    GenericList	 repaintComponents   - Lists all user-defined
*    components associated with this ComponentDisplay that are to be repainted.
*
*    GenericList 	 componentHotRegions - Lists all HotRegions associated
*    with user-defined components in this ComponentDisplay.
*
*    Boolean		 componentHotRegion  - TRUE if the last event
*    occurred in the hot region of a user-defined component that is displayed
*    in this ComponentDisplay.
*
*    Boolean		 dropComponents	     - TRUE if there are components
*    on this display that will accept drops.
*
*    int		 numScrollers        - The number of Scrollers
*    associated with this ComponentDisplay.
*
*    int		 numCanvasComponents - The number of components
*    associated with display that are drawable on an XView canvas.
*
*    DrawingModel 	 model               - The drawing model of this
*    ComponentDisplay.
*
*    ColorMap		*paintColorMap       - The color map used to paint this
*    component object.
*
*    UIMenu             *displayMenu         - The display menu associated
*    with this component object.
*
*    Orientation	 layoutDirection     - The layout direction of
*    components added to this ComponentDisplay.
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
*    NeWStoken		 canvasPSWindow      - The NeWS token for this object.
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
*    UIComponent   *focusNextComponent () - Sets the focus to the next
*    component that can accept input focus.  Returns a pointer to the
*    component with the input focus.  NULL is returned if there is no
*    input focus.
*
*    UIComponent   *focusNextComponent () - Sets the focus to the previous
*    component that can accept input focus.  Returns a pointer to the
*    component with the input focus.  NULL is returned if there is no
*    input focus.
*
*    void 	    addComponent (UIComponent &) - Adds the component to the 
*    list of components that are contained in the ComponentDisplay.
*
*    void 	    removeComponent (UIComponent &) - Removes the component
*    from the ComponentDisplay.
*
*    UIComponent   *traverseComponents (Boolean reset_flag) - Traverse
*    the components that belong to this ComponentDisplay.
*
*    void           setDefaultButton (Button &) - Specifies the default
*    button for this ComponentDisplay.
*
*    void           setFocusComponent (UIComponent &component) - Specifies which
*    component should have the input focus.
*
*    UIComponent   *getFocusComponent () - Returns the component that has the
*    input focus.
*
*    void	    setDynamic (Boolean) - specifies whether or not this
*    display can have a dynamic color map.  Default: FALSE.
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
*    ComponentDisplay should be fit to size around the components it 
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
*    void	    setAutoExpand (Boolean) - Sets the  ComponentDisplay to 
*    autoexpand to any painting that might occur on it.  Default: FALSE.
*
*    void	    setDrawingModel (DrawingModel) - Sets the drawing model 
*    of this  ComponentDisplay.  Default: Users XView drawing model. 
*    DrawingModel  is a data type that has the following values: XVIEW - 
*    Specifies the XView drawing model; X11 - Specifies the X11 drawing 
*    model. POSTSCRIPT - Specifies that an XView PostScript canvas should
*    be created (It is not recommended that the POSTSCRIPT model is used.
*    The X11 drawing model should normally be used, and then the NeWS
*    token can then be obtained via getPSWindow ().  Also, the refresh
*    behavior of the X11 model are more compatible with PostScript then
*    the refresh behavior of the XView drawing model.  Default: XView.
*    
*    void	    setRepaintHandler (UICHandlerFunction) - Sets the function 
*    that is to be executed when the ComponentDisplay needs to be repainted.
*    Default: Not set.
*
*    void	    setResizeHandler (UICHandlerFunction) - Sets the function 
*    that is to be executed when the ComponentDisplay is resized.
*    Default: Not set.
*
*    void	    setPSCanvasClipRects (Xv_xrectlist *) - Sets the list
*    of PostScript clipping rectangles for the display if it is an
*    XView PostScript Cnavas.  Default: Not set.
*
*    void           setMenu (UIMenu&) - Sets the menu that is to appear 
*    when the user selects the right mouse button and the arrow is within 
*    the ComponentDisplay boundaries.  Default: No menu associated.
*
*    UIMenu        *getMenu () - Gets the menu that is to appear when 
*    the user selects the right mouse button and the arrow is within
*    the ComponentDisplay boundaries.
*
*    Xv_window      getPaintWindow () - Returns the XView Canvas Paint 
*    Window.  This is only useful if the user will allow only drawing on
*    ComponentDisplay that has no components on it and will never permit
*    components to be added to it.
*
*    Display       *getXDisplay () - Returns a pointer to an X11 Display 
*    structure for the ComponentDisplay.
*
*    Window         getXWindow () - Returns the X11 XID of the ComponentDisplay.
*
*    NeWStoken	    getPSWindow () - Returns the NeWS token of the
*    ComponentDisplay (regardless of the drawing model).  Because of image
*    refresh properties, the X11 drawing model should be used for combining
*    XLib and PostScript.
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
*    void           setPSFocus ()
*    void           setPSFocus (NeWStoken) - Executes a NeWS setcanvas (focus)
*    command on the specified NeWS token.  If no NeWS token is supplied,
*    then the focus is specified on this object.
*
*    void	    loadPSFile (char*) - Loads the specified PostScript file
*    into this object.
*
*    void	    clearPSWindow () - Clears the PostScript graphics from
*    this object.
*
*    void	    setPSGray (float) - Sets the PostScript gray level.
*
*    void	    setPSColor (float red, float green, float blue) -
*    Sets the current PostScript color to the specified RGB values.
*
*    void	    setPSColor (Color color) - Sets the current PostScript
*    color to the RGB components contained in the UIC color structure.
*
*    void	    drawPSTransparentImagePlane (Image &image, int x, int y) - 
*    Draws one plane of the specified image at the specified location using
*    PostScript.  The off bits in the plane are transparent.
*
*    void	    storePSDrawImageProc (char    *name,
*					  Image   &image,
*					  Boolean  transparent=FALSE,
*					  Boolean  invert=FALSE) - 
*    Stores a PostScript procedure on the NeWS server to draw the specified
*    image.  If "transparent" is TRUE and the image is a multi-plane image,
*    the off bits are transparent (all single-plane images are
*    transparent).  If "invert" is TRUE and the image is a single-plane image,
*    the image is inverted.  If the image is multi-plane, "invert" causes the
*    pixels to be XOR'd with white.  The image is drawn using moveto and
*    rlineto commands and the PostScript procedure has the following form:
*
*		x y name
*
*    Where x and y are the coordinates of where the image is to be rendered.	
*
*    void	    storePSImageProc (char *name, Image &image) - Stores
*    a PostScript procedure on the NeWS server to render the specified image
*    using thePostScript "image" command.  The procedure has the following
*    form:
*
*		x y name
*
*    Where x and y are the coordinates of where the image is to be rendered.	
*
*    void	    storePSImageMaskProc (char    *name,
*					  Image   &image,
*					  Boolean  invert=TRUE) - Stores	
*    a PostScript procedure on the NeWS server to render the specified image
*    using the PostScript "imagemask" command.  The PostScript procedure
*    has the following form:
*
*		x y name
*
*    Where x and y are the coordinates of where the image is to be rendered.	
*
*    void	    removePSProc (char *) - Removes the named PostScript
*    procedure from the NeWS server.
*
*    void           addRepaintComponent (UIComponent&) - Adds a user-defined
*    component to the RepaintComponent list associated with this
*    ComponentDisplay.
*
*    void           addComponentHotRegion (UIComponent&, HotRegion&,
*                                          Boolean) - Adds a user-defined
*    component HotRegion to the Component HotRegion list associated with
*    this ComponentDisplay.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void  createObject (UIObject *) - Create XView object.
*
*    void	   repaintComponentsProc () - Internal function that calls
*    user defined repaint procs for user-defined components displayed in
*    this ComponentDisplay.
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
*    static void   repaintPSProc (PScanvas,
*				  NeWStoken,
*				  Display *,
*				  Window,
*                                 Xv_xrectlist *) - Internal wrapper around
*    the XView repaint proc for the PostScript drawing model if the object
*    is an XView PostScript Canvas.
*
*    static void   resizeProc (Canvas, int, int) - Internal wrapper around
*    the XView resize proc for a canvas.
*
*    static void   backgroundEventProc (Panel, Event *) - Internal wrapper
*    around the XView background proc.
*
*    static void eventProc (Xv_window, Event *) - Internal wrapper 
*    around the XView event proc.
*
*    static Notify_value panelEventProc (Xv_window,
*				         Event *,
*				         Notify_arg,
*				         Notify_event_type) - Internal wrapper 
*    around the XView panel notify interpose event proc.
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
*    void setComponentDisplayDefaults () - Initializes data members.
*
*C-*/
#ifndef _COMPONENT_DISPLAY_H
#define _COMPONENT_DISPLAY_H

// Includes
#include "Button.h"
#include "ColorMap.h"
#include "GenericList.h"
#include "HotRegion.h"
#include "UIComponent.h"
#include "UIDisplay.h"
#include "UIMenu.h"

#include <xview/win_input.h>
#include <xview/rect.h>
#include <pixrect/pixrect.h>
#include <pixrect/pixfont.h>
#include <xview/panel.h>
#include <xview/frame.h>
#include <xview/scrollbar.h>
#include <xview/xv_xrect.h>
#include <xview/xview.h>

#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
#include "private/postscript_canvas.h"
#endif
#endif

// Macro Definitions
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
#define POSTSCRIPT_STRING(string) pprintf (PostScript, \
					   (unsigned char *) string, \
					   strlen (string));
#endif
#endif

// Local Class Definitions
class ComponentHotRegion : public Generic {
public:
  UIComponent *component;
  HotRegion   *hotRegion;
  Boolean      trapBackgroundEvents;
};

// Class Definition
class ComponentDisplay : public UIDisplay {
public:

    		 	 ComponentDisplay ();
    		 	 ComponentDisplay (Boolean);
			~ComponentDisplay ();

    UIComponent		*focusNextComponent ()
			 {
			   UIComponent *component = (UIComponent *) NULL;
			   UICGeneric   xview_object;

			   if (owObject && isPanel) {
			     xview_object = panel_advance_caret (owObject);
			     if (xview_object) {
			       component = (UIComponent *)
					   xv_get (xview_object,
						   XV_KEY_DATA,
						   UIC_OBJECT_KEY);
			     }
			   }

			   return component;
			 }

    UIComponent         *focusPreviousComponent () 
                         { 
                           UIComponent *component = (UIComponent *) NULL;
                           UICGeneric   xview_object; 
 
                           if (owObject && isPanel) { 
                             xview_object = panel_backup_caret (owObject);
                             if (xview_object) { 
                               component = (UIComponent *)
                                           xv_get (xview_object,
                                                   XV_KEY_DATA,
                                                   UIC_OBJECT_KEY);
                             }   
                           }    
 
                           return component; 
                         } 

    void 	 	 addComponent (UIComponent &);

    void 	 	 removeComponent (UIComponent &);

    UIComponent		*traverseComponents (Boolean reset_flag)
			 { return (UIComponent *)
				  componentList.traverse (reset_flag); }

    void		 setDefaultButton (Button &);

    void                 setFocusComponent (UIComponent &);
    UIComponent         *getFocusComponent ();

    void		 setDynamic (Boolean flag)
			 { dynamic = flag; }

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

    void                 setLayoutDirection (Orientation direction)
			 { layoutDirection = direction;
			   if (owObject && isPanel) {
			     if (direction == HORIZONTAL)
			       xv_set (owObject,
				       PANEL_LAYOUT, PANEL_HORIZONTAL,
				       NULL);
			     else if (direction == VERTICAL)
			       xv_set (owObject,
				       PANEL_LAYOUT, PANEL_VERTICAL,
				       NULL);
			   }
			 }

    void		 setPaintHeight (int height)
			 { paintHeight = height;
			   if (owObject && (!isPanel || (numScrollers > 0))) {
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
			   if (owObject && (!isPanel || (numScrollers > 0))) {
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
			   if (owObject && (!isPanel || (numScrollers > 0)))
			     xv_set (owObject, CANVAS_AUTO_EXPAND, flag, NULL);
		         }

    void	 	 setDrawingModel (DrawingModel new_model)
			 { if (!owObject) model = new_model; }

    void		 setRepaintHandler (UICHandlerFunction);
    void		 setResizeHandler (UICHandlerFunction);

#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
    void		 setPSCanvasClipRects (Xv_xrectlist *rects)
			 { if ((model == POSTSCRIPT) && !isPanel)
			     xv_set (owObject,
				     PSCANVAS_CLIPRECTS, rects,
				     NULL); }
#endif
#endif

    void                 setMenu (UIMenu&);
    UIMenu              *getMenu ();

    Xv_window            getPaintWindow () { return paintWindow; }
    Display             *getXDisplay () { return canvasXDisplay; }
    Window               getXWindow () { return canvasXWindow; }

#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
    NeWStoken	         getPSWindow ()
			 {
			   if (canvasPSWindow == UIC_NOT_SET) {
			     if (model == POSTSCRIPT)
			       canvasPSWindow = (NeWStoken)
						xv_get (owObject,
							PSCANVAS_NEWSTOKEN);
			     else
			       ps_token_from_xid (canvasXWindow,
						  &canvasPSWindow);
		           }

			   return canvasPSWindow;
			 }
#endif
#endif

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

#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
    void                 setPSFocus ()
      {
	ps_setcanvas (getPSWindow ());
      }

    void                 setPSFocus (NeWStoken canvas)
      {
	ps_setcanvas (canvas);
      }

    void		 loadPSFile (char *ps_file)
      {
	char *transform_string = new char [30];
	sprintf (transform_string, "[1 0 0 -1 0 %d] concat ", rootWindowHeight);

	char *load_file_string = new char [strlen (ps_file) + 13];
	sprintf (load_file_string, "(%s) LoadFile ", ps_file);

	POSTSCRIPT_STRING ("gsave ");
	POSTSCRIPT_STRING ("32767 dict begin ");
        POSTSCRIPT_STRING (transform_string);
        POSTSCRIPT_STRING (load_file_string);
	POSTSCRIPT_STRING ("end "); 
	POSTSCRIPT_STRING ("grestore ");
        ps_flush_PostScript ();

	delete transform_string;
	delete load_file_string;
      }

    void		 clearPSWindow ()
      {
	POSTSCRIPT_STRING ("gsave ");
	POSTSCRIPT_STRING ("1 fillcanvas ");
	POSTSCRIPT_STRING ("grestore ");
        ps_flush_PostScript ();
      }

    void		 setPSGray (float gray)
      {
	char *temp_string = new char [128]; // Enough for setgray + a float

	sprintf (temp_string, " %f setgray ", gray);

        POSTSCRIPT_STRING (temp_string);
        ps_flush_PostScript ();

	delete temp_string;
      }

    void		 setPSColor (float red, float green, float blue)
     {
	char *temp_string = new char [128]; // Enough for setrgbcolor + 3 floats

	sprintf (temp_string, " %f %f %f setrgbcolor ", red, green, blue);

        POSTSCRIPT_STRING (temp_string);
        ps_flush_PostScript ();

	delete temp_string;
     }

    void		 setPSColor (Color color)
     {
	char *temp_string = new char [128]; // Enough for setrgbcolor + 3 floats

	sprintf (temp_string,
		 " %f %f %f setrgbcolor ",
		 ((float) color.red) / 255.0,
		 ((float) color.green) / 255.0,
		 ((float) color.blue) / 255.0);

        POSTSCRIPT_STRING (temp_string);
        ps_flush_PostScript ();

	delete temp_string;
     }

    void		 drawPSTransparentImagePlane (Image &image,
						      int x,
						      int y)
     {
	((UIObject *) &image)->createObject ();

	int         w = image.getWidth ();
	int         h = image.getHeight ();

	UICGeneric  xview_image = image.getXViewObject ();
	Display    *display = (Display *) xv_get (xview_image, XV_DISPLAY);
	XID         xid = (XID) xv_get (xview_image, XV_XID);

	XImage     *x_image = XGetImage (display, xid, 0, 0, w, h, 1, XYPixmap);

	char        temp_string [32];

	for (int j=0; j < h; j++)
	  for (int i=0; i < w; i++)
	    if (XGetPixel (x_image, i, j)) { 
	      sprintf (temp_string, "%d %d moveto ", x+i, y+j);
	      POSTSCRIPT_STRING (temp_string);
	      POSTSCRIPT_STRING ("0 0 rlineto "); 
	    }

	ps_stroke ();
	ps_flush_PostScript ();
     }

    void		 storePSDrawImageProc (char    *name,
					       Image   &image,
					       Boolean  transparent=FALSE,
					       Boolean  invert=FALSE);
    void		 storePSImageProc (char *, Image&);
    void		 storePSImageMaskProc (char    *name,
 					       Image   &image,
 					       Boolean  transparent=TRUE);
    void		 removePSProc (char *name);
#endif
#endif /* NO_XVPS */

    void                 addRepaintComponent (UIComponent &component)
			 { repaintComponents.addItem (&component); }

    void		 addComponentHotRegion (UIComponent &component,
						HotRegion   &hot_region,
						Boolean      flag = FALSE)
			 { ComponentHotRegion *chr = new ComponentHotRegion;
			   chr->component = &component;
			   chr->hotRegion = &hot_region;
			   chr->trapBackgroundEvents = flag;

			   componentHotRegions.addItem (chr); }

protected:
    Boolean              isPanel;
    Button              *defaultButton;
    UIComponent         *focusComponent;
    Boolean		 dynamic;
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
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
    NeWStoken		 canvasPSWindow;
#endif
#endif
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
			      repaintComponents [i])->repaintComponent (this);
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
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
    static  void repaintPSProc (PScanvas,
				NeWStoken,
				Display *,
				Window,
				Xv_xrectlist *);
#endif
#endif

    static  void resizeProc (Canvas, int, int);

    static  void         backgroundEventProc (Panel, Event *);
    static  void         eventProc (Xv_window, Event *);
    static  Notify_value panelEventProc (Xv_window,
				         Event *,
				         Notify_arg,
				         Notify_event_type);

private:
    void setComponentDisplayDefaults ();
};

#endif /* _COMPONENT_DISPLAY_H */
