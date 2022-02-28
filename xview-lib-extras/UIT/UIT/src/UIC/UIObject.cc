/*H+ UIObject.cc
*
*    SCCS ID: @(#)UIObject.cc 1.14 93/06/08 SMI
*
*    Copyright (c) 1990-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the UIObject class.
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
static char sccsid [] = "@(#)UIObject.cc 1.14 93/06/08 SMI"; 
#endif

// Includes
#ifndef __GNUC__
#include <sysent.h>
#endif

#include <unistd.h>
#include "UIObject.h"
#include "ColorMap.h"
#include "CharacterFont.h"
#include "HotRegion.h"
#include "Image.h"

#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
#include <wire/wire.h>
#endif
#endif

// Static member initialization

Boolean	     UIObject::initUIDone = FALSE;
int	     UIObject::noInitUIArgs = FALSE;
#ifdef SVR4
char         UIObject::applicationHostName [MAXNAMELEN];
#else
char         UIObject::applicationHostName [MAXHOSTNAMELEN];
#endif
Display     *UIObject::rootDisplay = (Display *) NULL;
int          UIObject::rootWindowWidth = 0;
int          UIObject::rootWindowHeight = 0;
Boolean	     UIObject::monochrome = FALSE;
ColorMap    *UIObject::colorMap = (ColorMap *) NULL;
Boolean	     UIObject::colorMapInitialized = FALSE;
Boolean      UIObject::defaultColorMap = FALSE;
UIObject    *UIObject::baseWindow = (UIObject *) NULL;
UIObject    *UIObject::firstPopUp = (UIObject *) NULL;
GenericList *UIObject::groupList = (GenericList *) NULL;
GenericList *UIObject::dropSiteList = (GenericList *) NULL;
GenericList *UIObject::dropTargetList = (GenericList *) NULL;
int          UIObject::dragThreshold = 0;
Boolean      UIObject::xViewObjectsCreated = FALSE;
int          UIObject::numUIObjects = 0;
int          UIObject::totalUIObjectsCreated = 0;

/*F+ UIObject::UIObject ()
*
*    PURPOSE:
*
*    UIObject constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIObject::UIObject ()
{
  setUIObjectDefaults ();
}

/*F+ UIObject::UIObject (char *string_label)
*
*    PURPOSE:
*
*    UIObject constructor.
*
*    PARAMETERS:
*
*    string_label - Specifies a string for the label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIObject::UIObject (char *string_label)
{
  setUIObjectDefaults ();
  label = strcpy (new char [strlen (string_label) + 1], string_label);
}

/*F+ void UIObject::setUIObjectDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the UIObject class.  Also generates
*    the unique ID every UIC object has.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setUIObjectDefaults ()
{
  // Keep a count of the actual UIC objects currently existing
  numUIObjects++;

  // Generate unique ID
  totalUIObjectsCreated++;
  uiObjectID = totalUIObjectsCreated;

  objectCreated = FALSE;
  destroyChildren = FALSE;
  destroyXViewObject = TRUE;
  owObject = XV_NULL;
  parentObject = (UIObject *) NULL;
  serverParent = (UIObject *) NULL;
  resourceName = (char *) NULL;
  acceleratorChar = '\0';
  label = (char *) NULL;
  xLoc = UIC_NOT_SET;
  yLoc = UIC_NOT_SET;
  width = UIC_NOT_SET;
  height = UIC_NOT_SET;
  help = (char *) NULL;
  closed = FALSE;
  foregroundColor = UIC_NOT_SET;
  backgroundColor = UIC_NOT_SET;
  font = (CharacterFont *) NULL;
  fontGC = (GC) NULL;
  eventHandler = (UICHandlerFunction) NULL;
  eventResultSet = FALSE;
  objectData = new GenericHash (20);
  showFlag = TRUE;
  resetEventInformation ();
  noDragDrop = FALSE;
  draggable = FALSE;
  acceptDrops = FALSE;
  rect_construct (&dropRectangle, 0, 0, 0, 0);
  defaultDropSite = FALSE;
  newDragCursorImage = FALSE;
  dragCursorImage = (Image *) NULL;
  dragCursor = (Cursor) NULL;
  dragCursorX = UIC_NOT_SET;
  dragCursorY = UIC_NOT_SET;
  newAcceptDropCursorImage = FALSE;
  acceptDropCursorImage = (Image *) NULL;
  acceptDropCursor = (Cursor) NULL;
  acceptDropCursorX = UIC_NOT_SET;
  acceptDropCursorY = UIC_NOT_SET;
  dragFileName = (char *) NULL;
  dragMessage = (char *) NULL;
  dragObject = (Generic *) NULL;
  dragHostName = (char *) NULL;
  dropMessage = (char *) NULL;
  dropFileName = (char *) NULL;
  dropObject = (Generic *) NULL;
  dragResult = UIC_NOT_SET;
  dragStatus = DRAG_NO_DRAG;
  dragConvertDataSet = FALSE;
  dragConvertSelection = (Selection_owner) NULL;
  dragConvertType = (Atom) UIC_NOT_SET;
  dragConvertData = (UICGeneric) NULL;
  dragConvertLength = 0;
  dragConvertFormat = UIC_NOT_SET;
  sendDragDelete = FALSE;
  dropSite = (Xv_drop_site) NULL;
  dropSiteSelection = (Selection_requestor) NULL;
  dragSite = (Xv_drag_drop) NULL;
  dragFileNameSelection = (Xv_drag_drop) NULL;
  dragMessageSelection = (Xv_drag_drop) NULL;
  dragObjectSelection = (Xv_drag_drop) NULL;
  dragHandler = (UICHandlerFunction) NULL;
  dropHandler = (UICHandlerFunction) NULL;
  isComponentGroup = FALSE;
  isBaseWindow = FALSE;
  isHotRegion = FALSE;
  isDropTarget = FALSE;
  isScroller = FALSE;
  isUIComponent = FALSE;
  isUIMenu = FALSE;
  isDrawableOnCanvas = FALSE;
  useDefaultEventHandlerFlag = FALSE;
  useDefaultDragHandlerFlag = FALSE;
  useDefaultDropHandlerFlag = FALSE;

  if (numUIObjects == 1) {
    groupList = new GenericList;
    dropSiteList = new GenericList;
    dropTargetList = new GenericList;

    dragThreshold = defaults_get_integer("openwindows.dragthreshold",
					 "OpenWindows.DragThreshold",
					 5);
  }
}

/*F+ UIObject::~UIObject ()
*
*    PURPOSE:
*
*    UIObject destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIObject::~UIObject ()
{
  // This UIC object no longer exists
  numUIObjects--;

  if (help) delete help;
  if (label) delete label;
  if (newDragCursorImage) delete dragCursorImage;
  if (newAcceptDropCursorImage) delete acceptDropCursorImage;
  if (dragCursor && destroyXViewObject) xv_destroy_safe (dragCursor);
  if (acceptDropCursor && destroyXViewObject)
    xv_destroy_safe (acceptDropCursor);
  if (dragFileName) delete dragFileName;
  if (dragMessage) delete dragMessage;
  if (dragHostName) delete dragHostName;
  if (dropMessage) delete dropMessage;
  if (dragSite && destroyXViewObject) xv_destroy_safe (dragSite);
  if (dropSite && destroyXViewObject) xv_destroy_safe (dropSite);
  
  // If no UIC objects exists any more...
  if (!numUIObjects) {
    // Delete the default color map, if necessary
    if (defaultColorMap && colorMap)
      delete colorMap;

    // Drop the items from the group list and delete it
    groupList->dropItems ();
    delete groupList; 

    // Drop the items from the drop target list and delete it
    dropSiteList->dropItems ();
    delete dropSiteList; 

    // Drop the items from the drop target list and delete it
    dropTargetList->dropItems ();
    delete dropTargetList; 
  }
  
  /* Drop the items from the events list so that its destructor doesn't
     try to delete the integers */
  events.dropItems ();
  
  // Clear and delete the object data table
  objectData->dropItems ();
  delete objectData;

  if (owObject && destroyXViewObject)
    xv_destroy_safe (owObject);
}

/*F+ const char *UIObject::getVersion ()
*
*    PURPOSE:
*
*    Returns the compatibility version string of the UIC.  Currently
*    all versions of the UIC that return "UICV1" are compatible.
*
*    RETURN VALUES:
*
*    Returns a string constant containing the current UIC version.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
const char *UIObject::getVersion ()
{
  static const char *uic_version = UIC_VERSION_STRING;
  return (uic_version);
}

/*F+ void UIObject::initUI (int &argc, char **argv,
*			    UICAttribute attribute,
*			    ...)
*
*    PURPOSE:
*
*    In addition to initializing the graphics server, this function also is
*    used to specify attributes for the current session with the UIC. The
*    following attributes are defined: 
* 
*     Name                      Type of value   Purpose
*     ----                      -------------   -------
*     UIC_OPEN_PS_CONNECTION    Boolean         Specifies whether or not to
*                                               a connection to the NeWS 
*                                               wire service for PostScript
*                                               useage. 
* 
*     UIC_TEXT_DOMAIN           char *          Specifies a text domain to
*                                               to bind for int'lization
* 
*     UIC_LOCALE_DIR            char *          Specifies a int'lization
*                                               message directory. 
*
*    If no attributes are to be set, use one of the following:
*
*               initUI (argc, argv);
*               int no_init_ui_args=0; initUI (no_init_ui_args);
*
* NOTE: If this code is changed, then BaseWindow::BaseWindow (...) needs
*       to have the same change made.
*
*    PARAMETERS:
*
*    argc      - The argc from main.
*    argv      - The argv from main.
*    attribute - The type of the first attribute.
*    ...       - A list of UIC session attributes.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::initUI (int &argc, char **argv, UICAttribute attribute, ...)
{ 
  if (initUIDone) return;
  initUIDone = TRUE;

  if (!argc) {
    xv_init (NULL); 
  }
  else {
    Boolean   open_ps_connection = FALSE;
    char     *locale_dir = (char *) NULL;
    char     *text_domain = (char *) NULL;
    int     (*x_error_proc) () = NULL;
    
    // Get the UIC attribute values
    va_list ap;
    va_start (ap, argv);
    
    UICAttribute arg = attribute;
    while (arg) {
      switch (attribute) {
        case UIC_OPEN_PS_CONNECTION:
          open_ps_connection = va_arg (ap, Boolean);
	  break;
	
        case UIC_TEXT_DOMAIN:
	  text_domain = va_arg (ap, char*);
	  break;
	
        case UIC_LOCALE_DIR:
	  locale_dir = va_arg (ap, char*);
	  break;
	
        case UIC_X_ERROR_PROC:
	  x_error_proc = (int (*)()) va_arg (ap, void *);
	  break;
	
        case UIC_NO_ATTRIBUTE:
	default:
	  break;
      }

      // Get the next argument
      arg = va_arg (ap, UICAttribute);
    }

    va_end (ap);

    // Process UIC attributes

    // Bind the specified text domain
    if (text_domain)
      (void) bindtextdomain (text_domain, "."); 

    // If an X error proc was specified, then make sure it gets set
    Xv_attr xv_x_error_proc = (Xv_attr) NULL;
    if (x_error_proc) xv_x_error_proc = XV_X_ERROR_PROC;

    // Initialize the display server and XView
    if (locale_dir) 
      xv_init (XV_INIT_ARGC_PTR_ARGV, &argc, argv,
	       XV_USE_LOCALE,   TRUE,
	       XV_LOCALE_DIR,   locale_dir,
	       xv_x_error_proc, x_error_proc,
	       NULL);
    else if (text_domain)
      xv_init (XV_INIT_ARGC_PTR_ARGV, &argc, argv,
	       XV_USE_LOCALE,   TRUE,
	       xv_x_error_proc, x_error_proc,
	       NULL);
    else
      xv_init (XV_INIT_ARGC_PTR_ARGV, &argc, argv,
	       xv_x_error_proc, x_error_proc,
	       NULL);
    
#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
    // If the PostScript connection attribute is specified...
    if (open_ps_connection) {
      ps_open_PostScript ();

      wire_Wire ps_wire = wire_Open (NULL);
      wire_SetCurrent (ps_wire);

      char *news_package = " /NeWS 3 0 findpackage beginpackage ";
      pprintf (PostScript,
	       (unsigned char *) news_package,
	       strlen (news_package));
      ps_flush_PostScript ();
    }
#endif
#endif /* NO_XVPS */
  }

  // Get the values of miscellaneous UIC session properties

  // Save the application host name
#ifdef SVR4
  gethostname (applicationHostName, MAXNAMELEN);
#else
  gethostname (applicationHostName, MAXHOSTNAMELEN);
#endif
 
  // Create a temporary XView object for obtaining root window information
  Server_image temp_image = xv_create (NULL, SERVER_IMAGE, NULL);
 
  // Get the root window (frame buffer) display pointer, width and height
  rootDisplay = (Display *) xv_get (temp_image, XV_DISPLAY);
  rootWindowWidth = DisplayWidth (rootDisplay, 0);
  rootWindowHeight = DisplayHeight (rootDisplay, 0);
 
  xv_destroy_safe (temp_image);
}

/*F+ void UIObject::setEventInformation (Event *event)
*
*    PURPOSE:
*
*    Sets the internal UIC event information, including information
*    relating to what hot regions the event may have occurred in.
*
*    PARAMETERS:
*
*    event - XView event information structure.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setEventInformation (Event *event)
{
  if (!event) return;

  // Save the XView event information
  lastEvent.setXViewEvent (event);

  // Loop through the hot regions attached to this object...
  int i;
  for (i = hotRegions.getSize () - 1; i > -1; i--) {
    // If this event is in this hot region...
    if (((HotRegion *) hotRegions [i])->detectHotRegionEvent (this, event)) {
      // This is a hot region event, so make a note of it
      hotRegionEventFlag = TRUE;

      // Add this hot region into the list of "active" hot regions
      activeHotRegions.addItem (hotRegions [i]);

      // If this hot region is not transparent...
      if (!((HotRegion *) hotRegions [i])->isTransparent ())
	// Don't process any more hot regions
	break;
    }
  }

  // Drag and drop processing

  HotRegion *hot_region;
  Boolean    hot_region_drop = FALSE;
  int        num_active_hot_regions = activeHotRegions.getSize ();
  for (i=0; i < num_active_hot_regions; i++) {
    hot_region = (HotRegion *) activeHotRegions [i];

    hot_region->checkForDragEvent ();

    /* If this hot region accepts drops and the object the event occurred
       on is not a UIComponent (UIComponent drop events are handled 
       by events on their parent ComponentDisplay) */
    if (hot_region->acceptDrops && !isUIComponent) {
      hot_region->checkForDropEvent ();
      hot_region_drop = TRUE;
    }
  }

  // If draggable, check to see if this event qualifies
  checkForDragEvent ();

  /* If no active hot regions accept drops and this is not a UIComponent,
     then check for a drop event (UIComponent drop events are handled
     by events on their parent ComponentDisplay) */ 
  if (!hot_region_drop && !isUIComponent) checkForDropEvent ();

  // Event target processing

  int xview_event_action = event_action (event);
  int xview_event_id = event_id (event);

  // Loop through the list of event targets attached to this object
  for (i = eventTargets.getSize () - 1; i > -1; i--) {
    UITarget *target = (UITarget *) eventTargets [i];
	
    /* Call the user specified target function if the user specified
       target event occurred */
    if (target->targetFunction)
      if ((((target->eventAction == UIC_NOT_SET) ||
	    (target->eventAction == ACTION_NULL_EVENT)) &&
	   ((target->eventID == UIC_NOT_SET) ||
	    (target->eventID == xview_event_id))) ||
	  (target->eventAction == xview_event_action))
        (*target->targetFunction) (this, target->targetObject);
  }
}

/*F+ void UIObject::resetEventInformation ()
*
*    PURPOSE:
*
*    Clears the internal UIC event information.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::resetEventInformation ()
{
  // Clear the XView event structure pointer
  lastEvent.setXViewEvent ((Event *) NULL);

  // Clear the hot region information
  hotRegionEventFlag = FALSE;
  activeHotRegions.dropItems ();
}

/*F+ void UIObject::setLoation (int new_x, int new_y)
*
*    PURPOSE:
*
*    Sets the X-Y location of the object relative to its parent.
*
*    PARAMETERS:
*
*    new_x - The new X coordinate.
*    new_y - The new Y coordinate.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setLocation (int new_x, int new_y)
{ 
  setX (new_x);
  setY (new_y);
}

/*F+ void UIObject::setX (int new_x)
*
*    PURPOSE:
*
*    Sets the X location of the object relative to its parent.
*
*    PARAMETERS:
*
*    new_x - The new X coordinate.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setX (int new_x)
{
  xLoc = new_x;

  if (owObject) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_X, xLoc, NULL, NULL);
    else
      xv_set (owObject, XV_X, xLoc, NULL);
    if (dropSite) changeDropSiteSize ((Rect *) xv_get (owObject, XV_RECT));
  }
  else if (dropSite) {
    Rect rectangle;
    rect_construct (&rectangle, xLoc, yLoc, width, height);
    changeDropSiteSize (&rectangle);
  }
}

/*F+ void UIObject::setY (int new_y)
*
*    PURPOSE:
*
*    Sets the Y location of the object relative to its parent.
*
*    PARAMETERS:
*
*    new_y - The new Y coordinate.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setY (int new_y)
{ 
  yLoc = new_y;

  if (owObject) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_Y, yLoc, NULL, NULL);
    else
      xv_set (owObject, XV_Y, yLoc, NULL);
    if (dropSite) changeDropSiteSize ((Rect *) xv_get (owObject, XV_RECT));
  }
  else if (dropSite) {
    Rect rectangle;
    rect_construct (&rectangle, xLoc, yLoc, width, height);
    changeDropSiteSize (&rectangle);
  }
}

/*F+ void UIObject::setSize (int new_width, int new_height)
*
*    PURPOSE:
*
*    Sets the size of the object.
*
*    PARAMETERS:
*
*    new_width  - The new width of the object.
*    new_height - The new height of the object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setSize (int new_height, int new_width)
{ 
  setHeight (new_height);
  setWidth (new_width);
}

/*F+ void UIObject::setWidth (int new_width)
*
*    PURPOSE:
*
*    Sets the width of the object.
*
*    PARAMETERS:
*
*    new_width  - The new width of the object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setWidth (int new_width)
{ 
  width = new_width;

  if (owObject) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_WIDTH, width, NULL, NULL);
    else
      xv_set (owObject, XV_WIDTH, width, NULL);
    if (dropSite) changeDropSiteSize ((Rect *) xv_get (owObject, XV_RECT));
  }
  else if (dropSite) {
    Rect rectangle;
    rect_construct (&rectangle, xLoc, yLoc, width, height);
    changeDropSiteSize (&rectangle);
  }
}

/*F+ void UIObject::setHeight (int new_height)
*
*    PURPOSE:
*
*    Sets the height of the object.
*
*    PARAMETERS:
*
*    new_height - The new height of the object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setHeight (int new_height)
{
  height = new_height;

  if (owObject) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_HEIGHT, height, NULL, NULL);
    else
      xv_set (owObject, XV_HEIGHT, height, NULL);
    if (dropSite) changeDropSiteSize ((Rect *) xv_get (owObject, XV_RECT));
  }
  else if (dropSite) {
    Rect rectangle;
    rect_construct (&rectangle, xLoc, yLoc, width, height);
    changeDropSiteSize (&rectangle);
  }
}

/*F+ void UIObject::setForegroundColor (Color color)
*
*    PURPOSE:
*
*    Sets the foreground color of the object.
*
*    PARAMETERS:
*
*    color - A UIC color structure containing the RGB components of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setForegroundColor (Color color)
{
  // If no color map has been specified...
  if (!colorMap) {
    // Use the default UIC color map
    useDefaultColorMap ();

    // If the XView object already exists...
    if (owObject) initializeColorMap (serverParent);
  }
  
  // Find the foreground color index
  foregroundColor = colorMap->findColor (color);
  
  if (owObject)
    xv_set (owObject, WIN_FOREGROUND_COLOR, foregroundColor, NULL);
}

/*F+ void UIObject::setForegroundColor (char *color)
*
*    PURPOSE:
*
*    Sets the foreground color of the object.
*
*    PARAMETERS:
*
*    color - The name of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setForegroundColor (char *color)
{
  // If no color map has been specified...
  if (!colorMap) {
    // Use the default UIC color map
    useDefaultColorMap ();

    // If the XView object already exists...
    if (owObject) initializeColorMap (serverParent);
  }
  
  // Find the foreground color index
  foregroundColor = colorMap->findColor (color);
  
  if (owObject)
    xv_set (owObject, WIN_FOREGROUND_COLOR, foregroundColor, NULL);
}

/*F+ void UIObject::setBackgroundColor (Color color)
*
*    PURPOSE:
*
*    Sets the background color of the object.
*
*    PARAMETERS:
*
*    color - A UIC color structure containing the RGB components of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setBackgroundColor (Color color)
{
  // If no color map has been specified...
  if (!colorMap) {
    // Use the default UIC color map
    useDefaultColorMap ();

    // If the XView object already exists...
    if (owObject) initializeColorMap (serverParent);
  }
  
  // Find the background color index
  backgroundColor = colorMap->findColor (color);
  
  if (owObject)
    xv_set (owObject, WIN_BACKGROUND_COLOR, backgroundColor, NULL);
}

/*F+ void UIObject::setBackgroundColor (char *color)
*
*    PURPOSE:
*
*    Sets the background color of the object.
*
*    PARAMETERS:
*
*    color - The name of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setBackgroundColor (char *color)
{
  // If no color map has been specified...
  if (!colorMap) {
    // Use the default UIC color map
    useDefaultColorMap ();

    // If the XView object already exists...
    if (owObject) initializeColorMap (serverParent);
  }
  
  // Find the background color index
  backgroundColor = colorMap->findColor (color);
  
  if (owObject)
    xv_set (owObject, WIN_BACKGROUND_COLOR, backgroundColor, NULL);
}

/*F+ void UIObject::setFont (CharacterFont &char_font)
*
*    PURPOSE:
*
*    Specifies the font to be used with this object.
*
*    PARAMETERS:
*
*    char_font - The UIC font to use.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setFont (CharacterFont &char_font)
{ 
  // If a font was previously specified...
  if (font)
    // If a graphics context was already created...
    if (fontGC) {
      Display *display;

      // Free the graphics context
      display = (Display *) xv_get (baseWindow->getXViewObject (), XV_DISPLAY);
      XFreeGC (display, fontGC);
    }

  // Save the new font
  font = &char_font;
  fontGC = (GC) NULL;

  // If the XView object already exists...
  if (owObject) {
    // Create the XView font object and set it
    ((UIObject *) font)->createObject (this);
    xv_set (owObject, XV_FONT, font->getXViewObject (), NULL);
  }
}

/*F+ void UIObject::setEventHandler (UICHandlerFunction handler)
*
*    PURPOSE:
*
*    Specified the event handler for the object.
*
*    PARAMETERS:
*
*    handler - The handler funtion.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setEventHandler (UICHandlerFunction handler)
{
  eventHandler = handler;
}

/*F+ int UIObject::addHotRegion (HotRegion &hot_region)
*
*    PURPOSE:
*
*    Adds a hot region to the list of hot regions for this object.
*
*    PARAMETERS:
*
*    hot_region - The hot region to add.
*
*    RETURN VALUES:
*
*    Returns the position in the hot region list for this object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::addHotRegion (HotRegion &hot_region)
{
  hotRegions.addItem ((Generic *) &hot_region);
  hot_region.hotRegionObject = this;

  if (!noDragDrop && !hot_region.noDragDrop) {
    if (xViewObjectsCreated) {
      /* Set the hot region's server parent to be the same as the object
         it is added to */
      hot_region.serverParent = serverParent; 

      if (hot_region.draggable) createHotRegionDragSite (hot_region);
      if (hot_region.acceptDrops) createHotRegionDropSite (hot_region);
    }
  }

  return hotRegions.getSize ();
}

/*F+ HotRegion *UIObject::removeHotRegion (int index)
*
*    PURPOSE:
*
*    Removes the specified hot region to this object's list of hot regions.
*
*    PARAMETERS:
*
*    index - The index into the list of hot regions.
*
*    RETURN VALUES:
*
*    Returns a pointer to the hot region removed.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
HotRegion *UIObject::removeHotRegion (int index)
{
  return ((HotRegion *) hotRegions.removeItem (index-1));
}

/*F+ HotRegion *UIObject::removeHotRegion (HotRegion &hot_region)
*
*    PURPOSE:
*
*    Removes the specified hot region from this object's list of hot regions.
*
*    PARAMETERS:
*
*    hot_region - The hot region to remove.
*
*    RETURN VALUES:
*
*    Returns a pointer to the hot region removed.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
HotRegion *UIObject::removeHotRegion (HotRegion &hot_region)
{
  int index = hotRegions.findItem ((Generic *) &hot_region);
  return ((HotRegion *) hotRegions.removeItem (index));
}

/*F+ int UIObject::addEventTarget (UIObject          &object,
*				   UICTargetFunction  func,
*				   int		      xview_event_action,
*				   int		      xview_event_id)
*
*    PURPOSE:
*
*    Adds a target to the list of event targets for this object.
*
*    PARAMETERS:
*
*    object             - The target object.
*    func               - The target function.
*    xview_event_action - If specified, this is the XView event action that
*			  must occur for the target function to be called.
*    xview_event_id     - If specified, this is the XView event ID that
*			  must occur for the target function to be called.
*
*    RETURN VALUES:
*
*    Returns the position in the target list for this object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::addEventTarget (UIObject          &object,
			      UICTargetFunction  func,
			      int		 xview_event_action,
			      int		 xview_event_id)
{
  UITarget *target = new UITarget;

  target->targetObject = &object;
  target->targetFunction = func;
  target->eventAction = xview_event_action;
  target->eventID = xview_event_id;

  eventTargets.addItem (target);
  return eventTargets.getSize ();
}

/*F+ GC UIObject::getFontGC ()
*
*    PURPOSE:
*
*    Returns the X graphics context of this objects font.
*
*    RETURN VALUES:
*
*    Returns the X graphics context of this objects font.  If no
*    font is specified, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
GC UIObject::getFontGC ()
{
  // If the XView object exists and a font is specified...
  if (owObject && font) {
    // Create the XView font object, if necessary
    if (!font->getXViewObject ())
      ((UIObject *) font)->createObject (this);

    // If a graphics context has already been created...
    if (fontGC)
      return fontGC;
    // Else, create a graphics context...
    else {
      Display   *display;
      XGCValues  gc_values;
      
      display = (Display *) xv_get (baseWindow->getXViewObject (), XV_DISPLAY);
      
      // Set the default values of the graphics context
      gc_values.font = (Font) xv_get (font->getXViewObject (), XV_XID);
      gc_values.foreground = BlackPixel (display, DefaultScreen (display));
      gc_values.background = WhitePixel (display, DefaultScreen (display));
      gc_values.graphics_exposures = FALSE;
      
      // Create the graphics context
      fontGC = XCreateGC (display,
			  DefaultScreen (display),
			  GCForeground |
			  GCBackground |
			  GCFont |
			  GCGraphicsExposures,
			  &gc_values);
      
      return fontGC;
    }
  }

  return (GC) NULL;
}

/*F+ int UIObject::getX ()
*
*    PURPOSE:
*
*    Returns the X coordinate of this object.
*
*    RETURN VALUES:
*
*    The X coordinate is returned.  If the XView object does not exist
*    and no X coordinate has been set, UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::getX ()
{
  return ((owObject) ? (int) xv_get (owObject, XV_X) : xLoc);
}

/*F+ int UIObject::getY ()
*
*    PURPOSE:
*
*    Returns the Y coordinate of this object.
*
*    RETURN VALUES:
*
*    The Y coordinate is returned.  If the XView object does not exist
*    and no Y coordinate has been set, UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::getY ()
{
  return ((owObject) ? (int) xv_get (owObject, XV_Y) : yLoc);
}

/*F+ void UIObject::getLocation (int &x_loc, int &y_loc)
*
*    PURPOSE:
*
*    Returns the X-Y location of this object.
*
*    PARAMETERS:
*
*    x_loc - The X coordinate of the object.
*    y_loc - The Y coordinate of the object.
*
*    RETURN VALUES:
*
*    The X-Y location is is returned.  If the XView object does not exist
*    and no location has been set, UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::getLocation (int &x_loc, int &y_loc)
{
  x_loc = getX ();
  y_loc = getY ();
}

/*F+ int UIObject::getHeight ()
*
*    PURPOSE:
*
*    Returns the height of this object.
*
*    RETURN VALUES:
*
*    The height is returned.  If the XView object does not exist
*    and no height has been set, UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::getHeight ()
{ 
  return ((owObject) ? (int) xv_get (owObject, XV_HEIGHT) : height);
}

/*F+ int UIObject::getWidth ()
*
*    PURPOSE:
*
*    Returns the width of this object.
*
*    RETURN VALUES:
*
*    The width is returned.  If the XView object does not exist
*    and no width has been set, UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::getWidth ()
{
  return ((owObject) ? (int) xv_get (owObject, XV_WIDTH) : width);
}

/*F+ void UIObject::getSize (int &h, int &w)
*
*    PURPOSE:
*
*    Returns the size of this object.
*
*    PARAMETERS:
*
*    h - The height of the object.
*    w - The width of the object.
*
*    RETURN VALUES:
*
*    The height and width is returned.  If the XView object does not exist
*    and no size has been set, UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::getSize (int &h, int &w)
{
  h = getHeight ();
  w = getWidth ();
}

/*F+ void UIObject::show (Boolean show_flag)
*
*    PURPOSE:
*
*    Specifies whether or not to display this object.
*
*    PARAMETERS
*
*    show_flag - TRUE if the object is to be displayed.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::show (Boolean show_flag)
{ 
  showFlag = show_flag;

  if (owObject)
    xv_set (owObject, XV_SHOW, show_flag, NULL);
}

/*F+ Boolean UIObject::isClosed ()
*
*    PURPOSE:
*
*    Determines if this object's XView frame is iconized.
*
*    RETURN VALUES:
*
*    Returns TRUE if the frame is closed.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
Boolean UIObject::isClosed ()
{
  if (owObject) {
    Frame xview_frame = (Frame) xv_get (owObject, XV_KEY_DATA, UIC_FRAME_KEY);
    closed = (Boolean) xv_get (xview_frame, FRAME_CLOSED);
  }

  return closed; 
}

/*F+ void UIObject::consumeEvent (int event_type)
*
*    PURPOSE:
*
*    Specifies a type of event to be consumed.
*
*    PARAMETERS
*
*    event_type - The type of event to consume.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::consumeEvent (int event_type)
{
  events.addItem ((Generic *) event_type);
  if (owObject) xv_set (owObject, WIN_CONSUME_EVENT, event_type, NULL);
}

/*F+ void UIObject::useDefaultColorMap ()
*
*    PURPOSE:
*
*    Specifies the use of the default UIT color map.  This is not
*    normally called by a user, because setting a foreground or background
*    color will automatically use it.  However, if a user created their own
*    color map and then later needed to use the default one, this can
*    be called to switch color maps.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::useDefaultColorMap ()
{
  static const char *uic_colors [] = {
    "Aquamarine", "Blue", "Blue Violet", "Brown",
    "Cadet Blue", "Coral", "Cornflower Blue", "Cyan",
    "Dark Green", "Dark Olive Green", "Dark Orchid", "Dark Slate Blue",
    "Dark Slate Gray", "Dark Turquoise", "Dim Gray",
    "Firebrick", "Forest Green",
    "Gold", "Goldenrod", "Gray", "Green", "Green Yellow",
    "Indian Red", "Khaki",
    "Light Blue", "Light Gray", "Light Steel Blue", "Lime Green",
    "Magenta", "Maroon", "Medium Aquamarine", "Medium Blue",
    "Medium Orchid", "Medium Sea Green", "Medium Slate Blue",
    "Medium Spring Green", "Medium Turquoise", "Medium Violet Red",
    "Midnight Blue",
    "Navy", "Navy Blue",
    "Olive Drab", "Orange", "Orange Red", "Orchid",
    "Pale Green", "Pink", "Plum", "Red",
    "Salmon", "Sea Green", "Sienna", "Sky Blue",
    "Slate Blue", "Spring Green", "Steel Blue", "Tan",
    "Thistle", "Turquoise",
    "Violet", "Violet Red",
    "Wheat", "Yellow", "Yellow Green",
    "aquamarine", "blue", "blue violet", "brown",
    "cadet blue", "coral", "cornflower blue", "cyan",
    "dark green", "dark olive green", "dark orchid", "dark slate blue",
    "dark slate gray", "dark turquoise", "dim gray",
    "firebrick", "forest green",
    "gold", "goldenrod", "gray", "green", "green yellow",
    "indian red", "khaki",
    "light blue", "light gray", "light steel blue", "lime green",
    "magenta", "maroon", "medium aquamarine", "medium blue",
    "medium orchid", "medium sea green", "medium slate blue",
    "medium spring green", "medium turquoise", "medium violet red",
    "midnight blue",
    "navy", "navy blue",
    "olive drab", "orange", "orange red", "orchid",
    "pale green", "pink", "plum", "red",
    "salmon", "sea green", "sienna", "sky blue",
    "slate blue", "spring green", "steel blue", "tan",
    "thistle", "turquoise",
    "violet", "violet red",
    "wheat", "yellow", "yellow green" };
 
  // If the default color map is already being used...
  if (defaultColorMap) return;

  defaultColorMap = TRUE;
  colorMap = new ColorMap;

  // Add the default UIC colros to the new color map
  int num_colors = sizeof (uic_colors) / 4;
  for (int i = 0; i < num_colors ; i++) 
    colorMap->addColor (uic_colors [i]);

  // If the XView object already exists...
  if (owObject) initializeColorMap (serverParent);
}

/*F+ void UIObject::setColorMap (ColorMap &color_map)
*
*    PURPOSE:
*
*    Specifies the use of a user defined color map.
*
*    PARAMETERS
*
*    color_map - The user defined color map.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setColorMap (ColorMap &color_map)
{
  // If a default color map has already been created...
  if (defaultColorMap && colorMap)
    delete colorMap;

  defaultColorMap = FALSE;

  // Save the user specified color map
  colorMap = &color_map;

  // If the XView object already exists
  if (owObject) {
    colorMapInitialized = FALSE;
    initializeColorMap (serverParent);
  }
}

/*F+ void UIObject::setLabel (char *string_label)
*
*    PURPOSE:
*
*    Specifies the string to use for the object's label.
*
*    PARAMETERS
*
*    string_label - The string to use.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setLabel (char *string_label)
{
  if (label)
    delete label;

  if (string_label)
    label = strcpy (new char [strlen (string_label) + 1], string_label);
  else
    label = (char *) NULL;

  if (owObject)
    xv_set (owObject, XV_LABEL, label, NULL);
}

/*F+ char *UIObject::getLabel (char* &string_label)
*
*    PURPOSE:
*
*    Gets the value of the object's string label.
*
*    PARAMETERS
*
*    string_label - The string label.
*
*    RETURN VALUES:
*
*    Returns the pointer to a newly allocated string containing
*    the string label.  Returns NULL if not set.  The caller must
*    free up the string.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
char *UIObject::getLabel (char* &string_label)
{
  if (label)
    string_label = strcpy (new char [strlen (label) + 1], label);
  else
    string_label = (char *) NULL;

  return (string_label);
}

/*F+ char *UIObject::getValue (char* &return_value)
*
*    PURPOSE:
*
*    Gets the object's last known string value.
*
*    PARAMETERS
*
*    return_value - The string value.
*
*    RETURN VALUES:
*
*    Returns the string value.  If no string value exists, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *UIObject::getValue (char* &return_value)
{
  return (return_value = (char *) NULL);
}

/*F+ int UIObject::getValue (int &return_value)
*
*    PURPOSE:
*
*    Gets the object's last known integer value.
*
*    PARAMETERS
*
*    return_value - The integer value.
*
*    RETURN VALUES:
*
*    Returns the integer value.  If no integer value exists,
*    UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::getValue (int &return_value)
{
  return (return_value = UIC_NOT_SET);
}

/*F+ char *UIObject::getChoice (char* &return_value)
*
*    PURPOSE:
*
*    Gets the string value of the either the object's last selected choice
*    or the choice with the lowest index.
*
*    PARAMETERS
*
*    return_value - The string value.
*
*    RETURN VALUES:
*
*    Returns the string value.  If no string value exists, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *UIObject::getChoice (char *&return_value)
{
  return (return_value = (char *) NULL);
}

/*F+ int UIObject::getChoice (int &return_value)
*
*    PURPOSE:
*
*    Gets the integer value of the either the object's last selected choice
*    or the choice with the lowest index.
*
*    PARAMETERS
*
*    return_value - The integer value.
*
*    RETURN VALUES:
*
*    Returns the integer value.  If no integer value exists,
*    UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::getChoice (int &return_value)
{
  return (return_value = UIC_NOT_SET);
}

/*F+ int UIObject::getChoice (int which_one, int &return_value)
*
*    PURPOSE:
*
*    Gets the string value of the object's specified choice.
*
*    PARAMETERS
*
*    which_one    - Specifies the choice to get the value of.
*    return_value - The integer value.
*
*    RETURN VALUES:
*
*    Returns the integer value.  If no integer value exists,
*    UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::getChoice (int which_one, int &return_value)
{
  return (return_value = UIC_NOT_SET);
}

/*F+ char *UIObject::getChoice (int which_one, char* &return_value)
*
*    PURPOSE:
*
*    Gets the string value of the object's specified choice.
*
*    PARAMETERS
*
*    which_one    - Specifies the choice to get the value of.
*    return_value - The string value.
*
*    RETURN VALUES:
*
*    Returns the string value.  If no string value exists, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *UIObject::getChoice (int which_one, char *&return_value)
{
  return (return_value = (char *) NULL);
}

/*F+ int UIObject::getNumberSelected ()
*
*    PURPOSE:
*
*    Returns the number of selected choices.
*
*    RETURN VALUES:
*
*    Returns the number of selected choices.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::getNumberSelected ()
{
  return (0);
}

/*F+ void UIObject::initializeColorMap (UIObject *parent, Boolean force)
*
*    PURPOSE:
*
*    Creates the application color map and sets the default background
*    and foreground colors for this object.  Initialization is only done
*    if a cforeground or background color was specified for this object
*    or the initialization is forced.
*
*    PARAMETERS:
*
*    parent - If specified, the initialized color map is applked to the
*	      object pointed at by this.
*    force  - TRUE if the initialization is to be forced.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::initializeColorMap (UIObject *parent, Boolean force)
{
  UICGeneric  cms_parent = (parent ? parent->getXViewObject () : owObject);
  UIObject   *colormap_parent = (parent ? parent : serverParent);

  int depth = (unsigned int) xv_get (cms_parent, XV_DEPTH);
  int visual_class = (int) xv_get (cms_parent, XV_VISUAL_CLASS);
 
  /* Only initialize the color map if we have enough depth and are using
     a color visual. */
  if ((depth < 8) ||
      !((visual_class == StaticColor) ||
        (visual_class == PseudoColor) ||
	(visual_class == TrueColor) ||
        (visual_class == DirectColor))) {
    monochrome = TRUE;
    return;
  }
 
  if (((foregroundColor != UIC_NOT_SET) ||
       (backgroundColor != UIC_NOT_SET) || force) && colorMap) {
    if (!colorMapInitialized) {
      ((UIObject *) colorMap)->createObject (colormap_parent);
      colorMapInitialized = TRUE;
    }
 
    xv_set (cms_parent, WIN_CMS, colorMap->getXViewObject (), NULL);
  }  
}

/*F+ void UIObject::setDraggable (Boolean flag)
*
*    PURPOSE:
*
*    Specifies whether or not this object is draggable.
*
*    PARAMETERS:
*
*    flag - TRUE if the object is draggable.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setDraggable (Boolean flag)
{
  if (!noDragDrop) {
    draggable = flag;

    if (draggable) {
      if (owObject)
        createDragSite ();
      else if (isHotRegion) {
        if (((HotRegion *) this)->hotRegionObject && xViewObjectsCreated)
	  ((HotRegion *)
           this)->hotRegionObject->createHotRegionDragSite (*(HotRegion *)
							     this);
      }
    }
  }
}

/*F+ void UIObject::setAcceptDrops (Boolean flag)
* 
*    PURPOSE: 
*
*    Specifies whether or not this object will accept drops and whether
*    or not it is the drop site default.
*
*    PARAMETERS: 
* 
*    flag - TRUE if the object will accept drops.
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIObject::setAcceptDrops (Boolean flag)
{
  if (!noDragDrop) {
    acceptDrops = flag;

    if (acceptDrops) {
      if (owObject)
        createDropSite ();
      else if (isHotRegion) {
        if (((HotRegion *) this)->hotRegionObject && xViewObjectsCreated)
          ((HotRegion *)
           this)->hotRegionObject->createHotRegionDropSite (*(HotRegion *)
                                                             this);
      }
    } else if (dropSite) {
      xv_destroy_safe (dropSite);
      dropSite = (Drop_site_item) NULL;
    }
  }
}

/*F+ void UIObject::setDropRectangle (int x, int y, int width, int height)
*
*    PURPOSE:
*
*    Specifies the rectangular region on this object that is sensitive to drops.
* 
*    PARAMETERS: 
* 
*    x      - The X location of the rectangle.
*    y      - The Y location of the rectangle.
*    width  - The width of the rectangle.
*    height - The height of the rectangle.
*
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIObject::setDropRectangle (int x, int y, int width, int height)
{
  rect_construct (&dropRectangle, x, y, width, height);

  if (dropSite) {
    xv_set (dropSite, DROP_SITE_DELETE_REGION, NULL, NULL);
    xv_set (dropSite, DROP_SITE_REGION, &dropRectangle, NULL);
  }
}

/*F+ void UIObject::setDefaultDropSite (Boolean flag)
*
*    PURPOSE:
*
*    Specifies whether or not this object is the default drop site for its
*    window.
* 
*    PARAMETERS: 
* 
*    flag - TRUE if this is the default drop site.
*
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIObject::setDefaultDropSite (Boolean flag)
{
  defaultDropSite = flag;

  if (dropSite)
    xv_set (dropSite, DROP_SITE_DEFAULT, defaultDropSite, NULL);
}

/*F+ void UIObject::setDragCursor (Image &cursor_image, int x, int y)
*
*    PURPOSE:
*
*    Specifies the drag cursor for this object.
* 
*    PARAMETERS: 
* 
*    cursor_image - The image representing the cursor.
*    x            - The X location of the cursor hot spot.
*    y            - The Y location of the cursor hot spot.
*
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIObject::setDragCursor (Image &cursor_image, int x, int y)
{
  if (newDragCursorImage) delete dragCursorImage;

  newDragCursorImage = FALSE;
  dragCursorImage = &cursor_image;
  dragCursorX = x;
  dragCursorY = y;

  if (dragSite && dragCursorImage) {
    Cursor old_cursor = dragCursor;
    
    ((UIObject *) dragCursorImage)->createObject (serverParent);
    dragCursor = xv_create (serverParent->getXViewObject (), CURSOR,
			    CURSOR_IMAGE, dragCursorImage->getXViewObject (),
			    CURSOR_OP,    PIX_SRC^PIX_DST,
			    CURSOR_XHOT,  dragCursorX,
			    CURSOR_YHOT,  dragCursorY,
			    NULL);
		  
    xv_set (dragSite, DND_CURSOR, dragCursor, NULL);

    if (old_cursor) xv_destroy_safe (old_cursor);
  }
}

/*F+ void UIObject::setDragCursor (char *cursor_file, int x, int y)
*
*    PURPOSE:
*
*    Specifies the drag cursor for this object.
* 
*    PARAMETERS: 
* 
*    cursor_file - A file containing the image data representing the cursor.
*    x           - The X location of the cursor hot spot.
*    y           - The Y location of the cursor hot spot.
*
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIObject::setDragCursor (char *cursor_file, int x, int y)
{
  if (newDragCursorImage) delete dragCursorImage;

  newDragCursorImage = TRUE;
  dragCursorImage = new Image (cursor_file);
  dragCursorX = x;
  dragCursorY = y;

  if (dragSite && dragCursorImage) {
    Cursor old_cursor = dragCursor;

    ((UIObject *) dragCursorImage)->createObject (serverParent);
    dragCursor = xv_create (serverParent->getXViewObject (), CURSOR,
			    CURSOR_IMAGE, dragCursorImage->getXViewObject (),
			    CURSOR_OP,    PIX_SRC^PIX_DST,
			    CURSOR_XHOT,  dragCursorX,
			    CURSOR_YHOT,  dragCursorY,
			    NULL);
		  
    xv_set (dragSite, DND_CURSOR, dragCursor, NULL);
    
    if (old_cursor) xv_destroy_safe (old_cursor);
  }
}

/*F+ void UIObject::setDragCursorHotSpot (int x, int y)
*
*    PURPOSE:
*
*    Specifies the hot spot of this object's drag cursor.
* 
*    PARAMETERS: 
* 
*    x - The X location of the cursor hot spot.
*    y - The Y location of the cursor hot spot.
*
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIObject::setDragCursorHotSpot (int x, int y)
{
  dragCursorX = x;
  dragCursorY = y;

  if (dragSite && dragCursorImage) {
    Cursor old_cursor = dragCursor;
    
    ((UIObject *) dragCursorImage)->createObject (serverParent);
    dragCursor = xv_create (serverParent->getXViewObject (), CURSOR,
			    CURSOR_IMAGE, dragCursorImage->getXViewObject (),
			    CURSOR_OP,    PIX_SRC^PIX_DST,
			    CURSOR_XHOT,  dragCursorX,
			    CURSOR_YHOT,  dragCursorY,
			    NULL);
    
    xv_set (dragSite, DND_CURSOR, dragCursor, NULL);
    
    if (old_cursor) xv_destroy_safe (old_cursor);
  }
}

/*F+ void UIObject::setAcceptDropCursor (Image &cursor_image, int x, int y)
*
*    PURPOSE:
*
*    Specifies the accept drop cursor for drags originating from this object.
* 
*    PARAMETERS: 
* 
*    cursor_image - The image representing the cursor.
*    x            - The X location of the cursor hot spot.
*    y            - The Y location of the cursor hot spot.
*
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIObject::setAcceptDropCursor (Image &cursor_image, int x, int y)
{
  if (newAcceptDropCursorImage) delete acceptDropCursorImage;

  newAcceptDropCursorImage = FALSE;
  acceptDropCursorImage = &cursor_image;
  acceptDropCursorX = x;
  acceptDropCursorY = y;
  
  if (dragSite && acceptDropCursorImage) {
    Cursor old_cursor = acceptDropCursor;

    ((UIObject *) acceptDropCursorImage)->createObject (serverParent);
    acceptDropCursor = xv_create (serverParent->getXViewObject (), CURSOR,
				  CURSOR_IMAGE,
				    acceptDropCursorImage->getXViewObject (),
			          CURSOR_OP,   PIX_SRC^PIX_DST,
				  CURSOR_XHOT, acceptDropCursorX,
				  CURSOR_YHOT, acceptDropCursorY,
				  NULL);
		  
    xv_set (dragSite, DND_ACCEPT_CURSOR, acceptDropCursor, NULL);
    
    if (old_cursor) xv_destroy_safe (old_cursor);
  }
}

/*F+ void UIObject::setAcceptDropCursor (char *cursor_file, int x, int y)
*
*    PURPOSE:
*
*    Specifies the accept drop cursor for drags originating from this object.
* 
*    PARAMETERS: 
* 
*    cursor_file - A file containing the image data representing the cursor.
*    x           - The X location of the cursor hot spot.
*    y           - The Y location of the cursor hot spot.
*
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIObject::setAcceptDropCursor (char *cursor_file, int x, int y)
{
  if (newAcceptDropCursorImage) delete acceptDropCursorImage;
  
  newAcceptDropCursorImage = TRUE;
  acceptDropCursorImage = new Image (cursor_file);
  acceptDropCursorX = x;
  acceptDropCursorY = y;
  
  if (dragSite && acceptDropCursorImage) {
    Cursor old_cursor = acceptDropCursor;
    
    ((UIObject *) acceptDropCursorImage)->createObject (serverParent);
    acceptDropCursor = xv_create (serverParent->getXViewObject (), CURSOR,
				  CURSOR_IMAGE,
				    acceptDropCursorImage->getXViewObject (),
			          CURSOR_OP,   PIX_SRC^PIX_DST,
				  CURSOR_XHOT, acceptDropCursorX,
				  CURSOR_YHOT, acceptDropCursorY,
				  NULL);
    
    xv_set (dragSite, DND_ACCEPT_CURSOR, acceptDropCursor, NULL);
    
    if (old_cursor) xv_destroy_safe (old_cursor);
  }
}

/*F+ void UIObject::setAcceptDropCursorHotSpot (int x, int y)
*
*    PURPOSE:
*
*    Specifies the hot spot of this object's accept drop cursor.
* 
*    PARAMETERS: 
* 
*    x - The X location of the cursor hot spot.
*    y - The Y location of the cursor hot spot.
*
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void UIObject::setAcceptDropCursorHotSpot (int x, int y)
{
  acceptDropCursorX = x;
  acceptDropCursorY = y;

  if (dragSite && acceptDropCursorImage) {
    Cursor old_cursor = acceptDropCursor;
    
    ((UIObject *) acceptDropCursorImage)->createObject (serverParent);
    acceptDropCursor = xv_create (serverParent->getXViewObject (), CURSOR,
				  CURSOR_IMAGE,
				    acceptDropCursorImage->getXViewObject (),
			          CURSOR_OP,   PIX_SRC^PIX_DST,
				  CURSOR_XHOT, acceptDropCursorX,
				  CURSOR_YHOT, acceptDropCursorY,
				  NULL);
    
    xv_set (dragSite, DND_ACCEPT_CURSOR, acceptDropCursor, NULL);
    
    if (old_cursor) xv_destroy_safe (old_cursor);
  }
}

/*F+ void UIObject::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of this object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::createObject (UIObject *parent)
{
}

/*F+ void UIObject::setUIObjectAttributes (UIObject *parent)
*
*    PURPOSE:
*
*    Sets attroibutes that are common to most UIObjects.
*
*    PARAMETERS:
*
*    parent - The parent of this UIObject.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::setUIObjectAttributes (UIObject *parent)
{
  // Set the object created flag
  objectCreated = TRUE;

  // The UIC parent
  parentObject = parent;

  // Object to use for setting the color map for this object
  if (!serverParent)
    serverParent = ((isUIComponent || isUIMenu) ? parent : this);

  // The object to use for the parent of NoticePrompt objects for this object
  if (owObject) {
    UIObject *notice_parent =
	      (isUIComponent ? parent : (isUIMenu ? (UIObject *) NULL : this));
    xv_set (owObject, XV_KEY_DATA, UIC_NOTICE_KEY, notice_parent, NULL);
  }

  if (!noDragDrop) {
    if (draggable) createDragSite ();
    if (acceptDrops) createDropSite ();
  }

  if (owObject) {
    UIInput *input_entry;
    int      num_input_functions = inputFunctions.getSize ();
    for (int i=0; i < num_input_functions; i++) {
      input_entry = (UIInput *) inputFunctions [i];
      (void) notify_set_input_func (owObject,
				    (Notify_func) &UIObject::inputFunc,
				    input_entry->fileDescriptor);
    }
  }

  HotRegion *hot_region;
  int        num_hot_regions = hotRegions.getSize ();
  for (int i=0; i < num_hot_regions; i++) {
    hot_region = (HotRegion *) hotRegions [i];

    /* Set the hot region's server parent to be the same as the object
       it belongs to */
    hot_region->serverParent = serverParent; 

    if (!noDragDrop && !hot_region->noDragDrop) {
      if (hot_region->draggable) createHotRegionDragSite (*hot_region);
      if (hot_region->acceptDrops) createHotRegionDropSite (*hot_region);
    }
  }

  if (owObject) {
    UIAccelerator *accelerator;
    int            num_accelerators = acceleratorList.getSize ();
    for (int i=0; i < num_accelerators; i++) {
      accelerator = (UIAccelerator *) acceleratorList [i];

      xv_set (owObject,
	      FRAME_ACCELERATOR, accelerator->acceleratorChar,
				 &UIObject::acceleratorProc,
				 accelerator,
	      NULL);
    }
  }

  if (owObject && showFlag)
    xv_set (owObject, XV_SHOW, showFlag, NULL);
}

/*F+ void UIObject::createDragSite ()
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
void UIObject::createDragSite ()
{
  if (!dragSite) createXViewDragSite (owObject);
}

/*F+ void UIObject::createDropSite ()
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
void UIObject::createDropSite ()
{
  Rect *rectangle;

  if (!dropSite) {
    if ((dropRectangle.r_width > 0) && (dropRectangle.r_height > 0))
      rectangle = &dropRectangle;
    else
      rectangle = (Rect *) xv_get (owObject, XV_RECT);

    createXViewDropSite (owObject, rectangle);
  }
}

/*F+ void UIObject::createHotRegionDragSite (HotRegion &hot_region)
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
void UIObject::createHotRegionDragSite (HotRegion &hot_region)
{
  if (!hot_region.dragSite) hot_region.createXViewDragSite (owObject);
}
 
/*F+ void UIObject::createHotRegionDropSite (HotRegion &hot_region)
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
void UIObject::createHotRegionDropSite (HotRegion &hot_region)
{
  Rect rectangle;

  if (!hot_region.dropSite) {
    if ((hot_region.dropRectangle.r_width > 0) &&
	(hot_region.dropRectangle.r_height > 0)) {
      rectangle = hot_region.dropRectangle;
    }
    else {
      int rect_x, rect_y, rect_w, rect_h;

      hot_region.getDefaultRectangle (rect_x, rect_y, rect_w, rect_h);
      rect_construct (&rectangle, rect_x, rect_y, rect_w, rect_h);
    }

    hot_region.createXViewDropSite (owObject, &rectangle);
  }
}

/*F+ void UIObject::createXViewDragSite (UICGeneric xview_object)
*
*    PURPOSE:
*
*    Creates the XView drag site for the specified XView object.
*
*    PARAMETERS:
*
*    xview_object - The XView object to create the drag site for.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::createXViewDragSite (UICGeneric xview_object)
{
  Xv_server server = XV_SERVER_FROM_WINDOW (xview_object);

  dragSite = xv_create (xview_object, DRAGDROP,
			DND_TYPE,	  DND_COPY,
			SEL_CONVERT_PROC, &UIObject::dragSelectionConvertProc,
			SEL_DONE_PROC,    &UIObject::dragSelectionDoneProc,
			NULL);

  xv_set (dragSite, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  xv_set (xview_object,
	  WIN_CONSUME_EVENTS,
	    LOC_DRAG,
	    WIN_MOUSE_BUTTONS,
	    NULL,
	  NULL);

  if (dragCursorImage) {
    ((UIObject *) dragCursorImage)->createObject (serverParent);
    dragCursor = xv_create (serverParent->getXViewObject (), CURSOR,
			    CURSOR_IMAGE, dragCursorImage->getXViewObject (),
			    CURSOR_OP,    PIX_SRC^PIX_DST,
			    CURSOR_XHOT,  dragCursorX,
			    CURSOR_YHOT,  dragCursorY,
			    NULL);

    xv_set (dragSite, DND_CURSOR, dragCursor, NULL);
  }

  if (acceptDropCursorImage) {
    ((UIObject *) acceptDropCursorImage)->createObject (serverParent);
    acceptDropCursor = xv_create (serverParent->getXViewObject (), CURSOR,
				  CURSOR_IMAGE,
				    acceptDropCursorImage->getXViewObject (),
				  CURSOR_OP,   PIX_SRC^PIX_DST,
				  CURSOR_XHOT, acceptDropCursorX,
				  CURSOR_YHOT, acceptDropCursorY,
				  NULL);

    xv_set (dragSite, DND_ACCEPT_CURSOR, acceptDropCursor, NULL);
  }

  dragMessageSelection = xv_create (dragSite, SELECTION_ITEM,
		                    SEL_DATA, dragMessage,
		                    NULL);

  dragFileNameSelection = xv_create (dragSite, SELECTION_ITEM,
                    		     SEL_TYPE, xv_get (server,
					               SERVER_ATOM,
						       "FILE_NAME"),
		                     SEL_DATA, dragFileName,
		                     NULL);

  (void) xv_create (dragSite, SELECTION_ITEM,
		    SEL_TYPE, xv_get (server, SERVER_ATOM, "HOST_NAME"),
		    SEL_DATA, applicationHostName,
		    NULL);

  (void) xv_create (dragSite, SELECTION_ITEM,
		    SEL_TYPE, xv_get (server,
				      SERVER_ATOM,
				      "_SUN_FILE_HOST_NAME"),
		    SEL_FORMAT, 8,
		    SEL_LENGTH, strlen (applicationHostName),
		    SEL_DATA,   applicationHostName,
		    NULL);

  dragObjectSelection =
    xv_create (dragSite, SELECTION_ITEM,
	       SEL_TYPE,   (Atom) xv_get (server, SERVER_ATOM, "OBJECT"),
	       SEL_FORMAT, 32,
	       SEL_LENGTH, 1,
	       SEL_DATA,   &dragObject,
	       NULL);
}

/*F+ void UIObject::createXViewDropSite (UICGeneric xview_object, Rect *region)
*
*    PURPOSE:
*
*    Creates the XView drop site for the specified XView object and region.
*
*    PARAMETERS:
*
*    xview_object - The XView object to create the drop site for.
*    region       - The rectangular region.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::createXViewDropSite (UICGeneric xview_object, Rect *region)
{
  dropSite = xv_create (xview_object, DROP_SITE_ITEM,
			DROP_SITE_ID,         uiObjectID,
			DROP_SITE_REGION,     region,
			DROP_SITE_EVENT_MASK, DND_ENTERLEAVE | DND_MOTION,
			DROP_SITE_DEFAULT,    defaultDropSite,
			NULL);

  xv_set (dropSite, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);

  dropSiteList->addItem ((Generic *) dropSite); 
}

/*F+ void UIObject::checkForDragEvent ()
*
*    PURPOSE:
*
*    Checks to see if the last event on this object was a drag event and
*    initiates the drag.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::checkForDragEvent ()
{
  if (draggable && !noDragDrop && !isDropTarget) {
    InputEvent *event = getEvent ();
    int         action;

    if (!isEventOnADropTarget (event)) {
      switch (action = event->getAction ()) {
      case LOC_DRAG:
      case PANEL_EVENT_CANCEL:
        if (event->isSelectButtonDown ()) {
	  /*
	   * If the drag threshold has been reached or the drag
	   * site is being exited (drag could have started close
	   * to the edge and the drag threshold will never be
	   * reached).
	   */
          if ((locDragCount++ == dragThreshold) ||
	      (action == PANEL_EVENT_CANCEL)) {
	    UIObject *object = (UIObject *) xv_get (dragSite,
						    XV_KEY_DATA,
						    UIC_OBJECT_KEY);
	
	    object->abortDragFlag = FALSE;
            object->dragStatus = DRAG_START;
            if (object->useDefaultDragHandlerFlag)
	      object->defaultDragHandler ();
            else if (object->dragHandler)
	      (*object->dragHandler) (object);

	    if (!abortDragFlag) {
              object->dragResult = dnd_send_drop (dragSite);

              object->dragStatus = DRAG_END;
              if (object->useDefaultDragHandlerFlag)
	        object->defaultDragHandler ();
              else if (object->dragHandler)
	        (*object->dragHandler) (object);
	    }

            object->dragStatus = DRAG_NO_DRAG;
            locDragCount = 0;
          }
        }

        break;

      default:
        locDragCount = 0;
        break;
      }
    }
  }
}

/*F+ void UIObject::checkForDropEvent ()
*
*    PURPOSE:
*
*    Checks to see if the last event on this object was a drop event and
*    handles the drop.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::checkForDropEvent ()
{
  if (!noDragDrop) {
    InputEvent *event = getEvent ();
    int	        action = event->getAction ();

    switch (action) {
    case ACTION_DRAG_COPY:
    case ACTION_DRAG_MOVE:
      if (isEventOnADropSite (event)) {
        Drop_site_item      drop_site;
	Selection_requestor selection =
	  xv_create (serverParent->getXViewObject (),
		     SELECTION_REQUESTOR,
		     NULL);
	dropSiteSelection = selection;

        if ((drop_site = dnd_decode_drop (selection, event->xViewEvent))
            != XV_ERROR) {
	  UIObject *object = (UIObject *) xv_get (drop_site,
						  XV_KEY_DATA,
						  UIC_OBJECT_KEY);

	  if (object) {
	    object->dropSiteSelection = selection;
            object->getInternalDropData (selection);

            if (object->useDefaultDropHandlerFlag)
              object->defaultDropHandler ();
            else if (object->dropHandler)
              (*object->dropHandler) (object);

	    if (sendDragDelete && (action = ACTION_DRAG_MOVE)) {
	      long length;
	      int  format;

	      xv_set (selection, SEL_TYPE_NAME, "DELETE", NULL);
	      (void) xv_get (selection, SEL_DATA, &length, &format);
	    }

	    object->dropSiteSelection = (Selection_requestor) NULL;
	  }

          dnd_done (selection);

	  dropSiteSelection = (Selection_requestor) NULL;

          /* Activate all of the drop targets */
          int num_drop_targets = dropTargetList->getSize ();
          for (int i=0; i < num_drop_targets; i++) 
            xv_set (((UIObject *) (*dropTargetList) [i])->getXViewObject (),
                    PANEL_BUSY, FALSE, 
                    NULL); 
        }

	xv_destroy_safe (selection);
      }
      break;

    default:
      break;
    }
  }
}

/*F+ Boolean UIObject::isEventOnADropSite (InputEvent *event)
*
*    PURPOSE:
*
*    Determines if the specified UIC event was on a drop site.
*
*    PARAMETERS:
*
*    event - The UIC event.
*
*    RETURN VALUES:
*
*    Returns TRUE if the event was on a drop site.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
Boolean UIObject::isEventOnADropSite (InputEvent *event)
{
  Boolean result = FALSE;
 
  int num_drop_sites = dropSiteList->getSize ();
  for (int i=0; i < num_drop_sites; i++) {
    if (xv_get ((Xv_opaque) (*dropSiteList) [i], DROP_SITE_ID) ==
	dnd_site_id (event->getXViewEvent ())) {
      result = TRUE;
      break;
    }
  }
 
  return result;
}
 
/*F+ Boolean UIObject::isEventOnADropTarget (InputEvent *event)
*
*    PURPOSE:
*
*    Determines if the specified UIC event was on a DropTarget.
*
*    PARAMETERS:
*
*    event - The UIC event.
*
*    RETURN VALUES:
*
*    Returns TRUE if the event was on a DropTarget.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
Boolean UIObject::isEventOnADropTarget (InputEvent *event)
{
  Boolean result = FALSE;

  int x = event->getX ();
  int y = event->getY ();

  int num_drop_targets = dropTargetList->getSize ();
  for (int i=0; i < num_drop_targets; i++) {
    if (((UIObject *) (*dropTargetList) [i])->parentObject == this)
      if (result =
	    rect_includespoint
              ((Rect *) xv_get (((UIObject *)
                                 (*dropTargetList) [i])->getXViewObject (),
                                XV_RECT),
               x, y))
        break;
  }

  return result;
}

/*F+ void UIObject::getInternalDropData (Selection_item selection)
*
*    PURPOSE:
*
*    Gets the data from the last drop and stores it internally.
*
*    PARAMETERS:
*
*    selection - The XView selection item to get the data from.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::getInternalDropData (Selection_item selection)
{
  long length;
  int  format;

  if (dropMessage) {
    delete dropMessage;
    dropMessage = (char *) NULL;
  }

  if (dropFileName) {
    delete dropFileName;
    dropFileName = (char *) NULL;
  }

  if (dragHostName) {
    delete dragHostName;
    dragHostName = (char *) NULL;
  }

  dropObject = (Generic *) NULL;

  xv_set (selection, SEL_TYPE, XA_STRING, NULL);
  char *string = (char *) xv_get (selection, SEL_DATA, &length, &format);

  if (string && length && (length != SEL_ERROR)) {
    dropMessage = strcpy (new char [length + 1], string);
    dropMessage [length] = '\0';
    free (string);
  }

  xv_set (selection, SEL_TYPE_NAME, "FILE_NAME", NULL);
  string = (char *) xv_get (selection, SEL_DATA, &length, &format);

  if (string && length && (length != SEL_ERROR)) {
    dropFileName = strcpy (new char [length + 1], string);
    dropFileName [length] = '\0';
    free (string);
  }

  xv_set (selection, SEL_TYPE_NAME, "HOST_NAME", NULL);
  string = (char *) xv_get (selection, SEL_DATA, &length, &format);
  
  if (!string || !length || (length == SEL_ERROR)) {
    xv_set (selection, SEL_TYPE_NAME, "_SUN_FILE_HOST_NAME", NULL);
    string = (char *) xv_get (selection, SEL_DATA, &length, &format);
  }

  if (string && length && (length != SEL_ERROR)) {
    dragHostName = strcpy (new char [length + 1], string);
    dragHostName [length] = '\0';
    free (string);
  }

  xv_set (selection, SEL_TYPE_NAME, "OBJECT", NULL);
  long *object = (long *)
		 xv_get (selection, SEL_DATA, &length, &format);
  if (object && length && (length != SEL_ERROR)) {
    dropObject = (Generic *) *object;
    free ((char *) object);
  }
}

/*F+ void UIObject::acceleratorProc (Xv_opaque data, Event *event)
*
*    PURPOSE:
*
*    XView accelerator notify proc.
*
*    PARAMETERS:
*
*    data  - The data specified in the accelerator set.
*    event - The event that triggered the accelerator.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::acceleratorProc (Xv_opaque data, Event *event)
{
  UIAccelerator *accelerator = (UIAccelerator *) data;
  UIObject      *object = accelerator->acceleratorObject;

  object->acceleratorChar = accelerator->acceleratorChar;
  object->setEventInformation (event);
 
  if (accelerator->acceleratorHandler)
    (*accelerator->acceleratorHandler) (object);
 
  object->resetEventInformation ();
}

/*F+ Notify_value UIObject::inputFunc (Notify_client client, int fd) 
*
*    PURPOSE:
*
*    XView notifier input function.
*
*    PARAMETERS:
*
*    client - The client the function was set on.
*    fd     - The active file descriptor.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
Notify_value UIObject::inputFunc (Notify_client client, int fd)
{
  UIObject  *object = (UIObject *)
                      xv_get (client, XV_KEY_DATA, UIC_OBJECT_KEY);

  UIInput *input_entry;
  int      num_input_functions = object->inputFunctions.getSize ();
  for (int i=0; i < num_input_functions; i++) {
    input_entry = (UIInput *) object->inputFunctions [i];
    if (input_entry->fileDescriptor == fd) {
      if (input_entry->inputFunction)
	(*input_entry->inputFunction) (object, fd);
    }
  }

  return NOTIFY_DONE;
}

/*F+ void UIObject::dragSelectionConvertProc (Selection_owner  selection,
*					      Atom	      *type,
*					      Xv_opaque	      *data,
*					      unsigned long   *length,
*					      int 	      *format)
*
*    PURPOSE:
*
*    XView selection convert proc for drag selections.
*
*    PARAMETERS:
*
*    selection - The selection owner.
*    type      - The type of the selection.
*    data      - The address that contains the selection data.
*    length    - The number of elements in the reply buffer.
*    format    - The format of the reply buffer data.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
int UIObject::dragSelectionConvertProc (Selection_owner  selection,
                                        Atom            *type,
                                        Xv_opaque       *data,
                                        unsigned long   *length,
                                        int             *format)
{
  UIObject  *object = (UIObject *)
                      xv_get (selection, XV_KEY_DATA, UIC_OBJECT_KEY);
  Xv_server  server = XV_SERVER_FROM_WINDOW (xv_get (selection, XV_OWNER));
  Boolean    use_default_proc = TRUE;

  object->dragConvertDataSet = FALSE;
  object->dragConvertSelection = selection;
  object->dragConvertType = *type;
  object->dragConvertData = *data;
  object->dragConvertLength = *length;
  object->dragConvertFormat = *format;
 
  if (*type == (Atom) xv_get (server, SERVER_ATOM, "_SUN_SELECTION_END")) {
    xv_set (object->dragSite, SEL_OWN, FALSE, NULL);

    *type   = (Atom) xv_get (server, SERVER_ATOM, "NULL");
    *data   = NULL;
    *length = 0;
    *format = 32;
 
    use_default_proc = FALSE;
  }
  else if (*type == (Atom) xv_get (server, SERVER_ATOM, "DELETE")) {
    object->dragStatus = DRAG_DELETE;
 
    if (object->useDefaultDragHandlerFlag)
      object->defaultDragHandler ();
    else if (object->dragHandler)
      (*object->dragHandler) (object);
 
    if (object->dragConvertDataSet) {
      *type   = object->dragConvertType; 
      *data   = object->dragConvertData;
      *length = object->dragConvertLength; 
      *format = object->dragConvertFormat;
    }
    else {
      *type   = (Atom) xv_get (server, SERVER_ATOM, "NULL");
      *data   = NULL;
      *length = 0;
      *format = 32;
    }

    use_default_proc = FALSE;
  } 
  else { 
    object->dragStatus = DRAG_CONVERT;
 
    if (object->useDefaultDragHandlerFlag)
      object->defaultDragHandler ();
    else if (object->dragHandler)
      (*object->dragHandler) (object);
 
    if (object->dragConvertDataSet) {
      *type   = object->dragConvertType; 
      *data   = object->dragConvertData;
      *length = object->dragConvertLength; 
      *format = object->dragConvertFormat;

      use_default_proc = FALSE;
    }
  } 
    
  object->dragConvertDataSet = FALSE;
  object->dragConvertSelection = (Selection_owner) NULL;
  object->dragConvertType = (Atom) UIC_NOT_SET;
  object->dragConvertData = (UICGeneric) NULL;
  object->dragConvertLength = 0;
  object->dragConvertFormat = UIC_NOT_SET;

  return (use_default_proc ?
	  sel_convert_proc (selection, type, data, length, format) : TRUE);
}

/*F+ void UIObject::dragSelectionDoneProc (Selection_owner selection,
*                                          Xv_opaque       data,
*					   Atom		   type)
*
*    PURPOSE:
*
*    XView selection done proc for drag selections.
*
*    PARAMETERS:
*
*    selection - The selection owner.
*    data      - Address containing selection data.
*    type      - The type of data.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIObject::dragSelectionDoneProc (Selection_owner selection,
                                      Xv_opaque       data,
				      Atom	      type)
{
  UIObject  *object = (UIObject *)
                      xv_get (selection, XV_KEY_DATA, UIC_OBJECT_KEY);

  /* Activate all of the drop targets */
  int num_drop_targets = object->dropTargetList->getSize ();
  for (int i=0; i < num_drop_targets; i++)
    xv_set (((UIObject *) (*object->dropTargetList) [i])->getXViewObject (),
            PANEL_BUSY, FALSE,
            NULL);

}
 
#ifdef TEST
#endif /* TEST */
