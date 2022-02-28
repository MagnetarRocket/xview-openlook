/*H+ UIObject.h
*
*    SCCS ID: @(#)UIObject.h 1.10 93/06/01 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE: 
*
*    Defines the UIObject class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       04/26/91   Preparation for code review
*
*H-*/

/*C+ UIObject
*
*    DESCRIPTION:
*  
*    The nature of this object is to define the generic characteristics 
*    of all UIC objects.  It includes such things as setting the properties 
*    of an object, such as its height, width, color and the function that 
*    will execute when an event happens to this object, such as a user 
*    clicking a mouse on it.  It also carries virtual functions which the 
*    Derived Class implements. 
*
*    INHERITED CLASSES:
*
*    Generic - Inheriting the Generic class will allow the UIObject class
*    to be treated as a Generic object. Therefore, a GenericHash or 
*    GenericList class can contain a UIObject.
*
*    LOCAL CLASSES:
*
*    InputEvent - contains all of the information that pertains to a user
*    interface event, such as a user clicking the mouse pointer on a button.
*    Every UIC class has the ability to obtain the last event that happened
*    to an object, while in a handler, by using the getEvent member function.  
*    When your application calls the getEvent function, that function returns 
*    a pointer to an InputEvent object. You NEVER create an instance of this 
*    class; this object is always returned by the getEvent.  Observe, that 
*    this class has no corresponding .h file.
*
*             ORDINARY DATA MEMBERS:
*
*	      Event      *xViewEvent  -  pointer to the XView
*	      Event structure.
*
*             timeval     currentSelectTime - The time of the current select
*             event.  Used for detecting double clicks.
*
*             timeval     lastSelectTime - The time of the last select event.
*             Used for detecting double clicks.
*    
*             Rect        lastRegion - The region the last event took place in.
*
*             STATIC DATA MEMBERS:
*
*             short thresholdsInitialized - Flag for making sure the thresholds
*             are only initialized once.
*
*             int   timeThreshold - The maximum amount of microseconds between
*             clicks to be a double click.
*
*             int   dragThreshold - The pixel size of the region that events
*             must take place in to be part of a double click.
*
*             PUBLIC MEMBER FUNCTIONS: 
*
*             int     getID()          - Returns the XView event ID.
*
*	      int     getAction()      - Returns the XView action.
*
*	      Boolean isButtonUp()     - Returns TRUE if the mouse button 
*	      is released (not pressed down) when the event occurs.
*
*	      Boolean isButtonDown()   - Returns TRUE if the mouse button
*	      is pressed when the event occurs.
*
*	      Boolean isButtonDown()   - Returns TRUE if the mouse button
*	      is pressed when the event occurs.
*
*	      Boolean isSelectButtonDown () - Returns TRUE if the select button
*             is down.
*
*	      Boolean isAdjustButtonDown () - Returns TRUE if the select button
*             is down.
*
*	      Boolean isMenuButtonDown () - Returns TRUE if the select button is
*             down.
*
*             Boolean isAscii () - Returns TRUE if the event was an ASCII
*	      event.
*
*	      Boolean isDoubleClick () - Returns TRUE if the event was
*	      a double click.
*
*	      Event   *getXViewEvent() - Returns the pointer to the XView
*	      Event structure.
*
*             int      getX()    - Returns the X coordinate of the position 
*             of the locator (mouse) relative to the window in which the event 
*             occurred.
*
*             int      getY()    - Returns the Y coordinate of the position 
*             of the locator (mouse) relative to the window in which the event 
*             occurred.
*
*	      timeval *getTime() - Returns the absolute time of the event.
*
*	      PRIVATE MEMBER FUNCTIONS:
*
*	      void setXViewEvent (Event *) - Sets the internal XView
*	      event structure pointer.
*
*	      Boolean detectDoubleClick (Event *) - Returns TRUE if a double
*	      click event has been detected.
*
*             FRIEND CLASSES:
*
*             UIObject - UIObject is a friend of InputEvent so that it
*             can set the XView event information InputEvent uses.
*
*	      HotRegion - HotRegion is a basic event handling class and
*	      therefore needs to be able to set XView event information
*	      in an InputEvent object.
*
*             NoticePrompt - The NoticePrompt needs to be able to set
*	      the XViewEvent information when a NoticePrompt trigger
*             event occurs.
*
*   UITarget - Contains information used to assocate/bind objects. This 
*   binding occurs between the object in which the event is noted and an
*   object that requires a "secondary" notification.  This notification,
*   in turn, may trigger some functions to be executed upon the object.
*   This class is used internally by the UIObject only.  You NEVER 
*   create an instance of this class and there is no corresponding .h file.
*
*             ORDINARY DATA MEMBERS:
*
*             UIObject          *targetObject   - Pointer to the
*             associated/bound object.
*
*             UITargetFunction   targetFunction - Function that is to be
*             executed upon the above object.
*
*	      int		 eventAction	- If specified, the target
*	      function is only executed when this event action occurs.
*
*	      int		 eventID	- If specified, the target
*	      function is only executed when this event ID occurs.
*
*   UIInput - Contains an input function and a file descriptor for
*   use by notify_set_input_func.
*
*             ORDINARY DATA MEMBERS:
*
*	      UICInputFunction  *inputFunction	- Function to be called
*	      when the file descriptor is active.
*
*	      int		 fileDescriptor - The file descriptor.
*
*   UIAccelerator - Contains an object to be notified and a character
*   that specifies a frame accelerator.
*
*             ORDINARY DATA MEMBERS:
*
*	      UIObject *acceleratorObject - The object to be notified.
*
*	      char      acceleratorChar - The character that triggers
*	      the accerator.
*             
*   STATIC DATA MEMBERS:
*
*   Boolean       initUIDone - TRUE if initUI has been called.
*   int           noInitUIArgs - Dummy argument for initUI calls without
*                 argc and argv.
*
* #ifdef SVR4
*    char         applicationHostName [MAXNAMELEN]
* #else
*    char         applicationHostName [MAHOSTXNAMELEN]
* #endif
*                 - The name of the host the application is running on.
*
*    Display     *rootDisplay - The X Display pointer of the root window.
*    int	  rootWindowWidth - The width of the frame buffer.
*    int	  rootWindowHeight - The Height of the frame buffer.
*
*    Boolean      monochrome - TRUE if the display is monochrome or can't
*    support the UIC default color map.
*
*    ColorMap    *colorMap - The main color map used by all of the UIC 
*    objects.
*
*    Boolean      defaultColorMap - TRUE if the default color map is being used.
*
*    UIObject    *baseWindow - Pointer to the main (first) Base Window.
*
*    UIObject    *firstPopUp - Pointer to the first Pop Up Window.
*
*    GenericList *groupList - List of all ComponentGroups.
*
*    GenericList *dropSiteList - List of drop sites in this application.
*
*    GenericList *dropTargetList - List of DropTargets in this application.
*
*    int          dragThreshold - The number of times a LOC_DRAG needs to
*    occur in order for a drag to be sourced.
*
*    Boolean      xViewObjectsCreated - TRUE if the XView objects have
*    been created.
*
*    int          numUIObjects - The number of objects (derived from 
*    UIObject) instantiated.
*
*    int          totalUIObjectsCreated	- Used for generating a unique ID for
*    every object (derived from UIObject) instantiated.
*
*    ORDINARY DATA MEMBERS:
*
*    Boolean		 objectCreated	    - Set to TRUE when
*    setUIOBjectAttributes is called (from inside of a createObject
*    call).
*
*    Boolean		 destroyChildren    - If set, then all of the
*    UIC objects that are children of this object will be destroyed.
*
*    Boolean		 destroyXViewObject - If set, then when this
*    object is destructed, the XView object associated with it is destroyed.
*
*    char		*resourceName       - If set, contains the XView
*			 instance name to be used to lookup X resource DB
*			 values for this object's attributes.
*
*    GenericList	 acceleratorList    - List of accelerators
*    specified for this object.
*
*    char		 acceleratorChar    - The character of the
*    last accelerator triggered on this object.
*
*    int                 xLoc               - X coordinate of this object.
*
*    int                 yLoc               - Y coordinate of this object.
*
*    int                 width              - Width of this object. 
*
*    int                 height             - Height of this object.
*
*    char               *label              - Pointer to the label that is to 
*    appear on the object. 
*
*    char               *help               - Specifies the help string used 
*    by the help package to display on-line help. Default: No help available.
*
*    UICGeneric          owObject           - XView object pointer.
*
*    UIObject		*parentObject	    - If set, it is the UIC parent
*    of this object.
*
*    UIObject		*serverParent	    - If set, this is the object that
*    should used as the parent object for initializing color maps and creating
*    images for this object.
*
*    Boolean		 closed		    - Indicates whether or not
*    this object's XView frame is iconized.
*
*    int		 foregroundColor    - Numerical value of the Foreground
*    Color derived from the Color Map. 
*
*    int		 backgroundColor    - Numerical value of the Background
*    Color derived from the Color Map.
*
*    UICHandlerFunction	 eventHandler       - Function that is to be executed 
*    when an event is noted in this object. 
*
*    GenericList	 events             - List of events to consume.  Used
*    for specifing that the window will accept an event of the type specified.
*
*    InputEvent		 lastEvent          - The last event that occurred on 
*    this object. 
*
*    Boolean		 eventResultSet     - TRUE if the user had specified an 
*    event result, while in the handler.
*
*    EventResult	 eventResult        - Contains the event result the 
*    user specified.
*
*    GenericList	 inputFunctions     - List of input functions to set
*    for this object.
*
*    GenericList	 hotRegions         - Lists all hot regions (active or 
*    not) associated with this object.
*
*    GenericList	 activeHotRegions   - Lists all active hot regions 
*    associated with this object.
*
*    Boolean		 hotRegionEventFlag - TRUE if the last event took 
*    place in a hot region.
*
*    GenericList	 eventTargets       - List of objects to be notified 
*    when an event occurs in this object.
*
*    CharacterFont      *font               - The font to be used with this 
*    object.
*
*    GC                  fontGC             - A holder for the X font graphics 
*    context for this object.
*
*    GenericHash        *objectData         - A table containing object data
*    associated with this object.
*
*    Boolean		 showFlag           - Specifies whether or not this 
*    object is to be shown (XV_SHOW).
*
*    Boolean		 noDragDrop	    - TRUE if this object will not
*    aloow dragging and dropping even when setDraggable or setAcceptDrops
*    are set to TRUE.
*
*    Boolean		 draggable	    - TRUE if this object is draggable.
*
*    Boolean		 acceptDrops	    - TRUE if this object will accept
*    drops on it.
*
*    Boolean             defaultDropSite - TRUE if this object is the default
*    drop site for drops forwarded from the window manager.
*
*    Image              *dragCursor         - The object's drag cursor.
*    int                 dragCursorX        - The X location of the drag cursor's
*    hot spot.
*    int                 dragCursorY        - The Y location of the drag cursor's
*    hot spot.
*
*    Image              *acceptDropCursor   - The object's accept drop cursor.
*    int                 acceptDropCursorX  - The X location of the accept drop
*    cursor's hot spot.
*    int                 acceptDropCursorY  - The Y location of the accept drop
*    cursor's hot spot.
*
*    char               *dragFileName	    - The file name that will be sent
*    by drag operations from this object.
*
*    char               *dragMessage        - The message that will be sent
*    by drag operations from this object.
*
*    Generic            *dragObject         - A pointer to the object that will
*    be sent by drag operations from this object.
*
*    char               *dragHostName       - The name of the origination host
*    of the last drag and drop operation dropped on this object.
*
*    char               *dropMessage        - The message received from
*    the last drop onto this object.
*
*    char               *dropFileName	    - The file name received from
*    the last drop onto this object.
*
*    Generic            *dropObject         - A pointer to an object received
*    from the last drop onto this object.
*
*    int                 dragResult         - Contains the result of the last
*    drag and drop operation involving this object.
*
*    DragStatus          dragStatus         - Contains the status of the current
*    drag operation from this object.
*
*    Boolean		 abortDragFlag      - If set to TRUE when the dragStatus
*                                             is DRAG_START, the drag operation
*                                             is aborted.
*
*    Boolean		 dragConvertDataSet - TRUE if drag convert data was set.
*
*    Selection_owner     dragConvertSelection - The selection owner of the
*    convert data.
*    Atom		 dragConvertType    - The type of the convert data.
*    UICGeneric		 dragConvertData    - The convert data buffer.
*    unsigned long	 dragConvertLength  - The length of the convert data.
*    int		 dragConvertFormat  - The format of the convert data.
*
*    Boolean		 sendDragDelete     - TRUE if a "DELETE" selection
*    should be sent after handling a drop and the event action is
*    ACTION_MOVE_COPY.
*    Default: FALSE.
*
*    Xv_drop_site        dropSite           - The XView drop site.
*    Selection_requestor dropSiteSelection  - The drop site selection requestor.
*
*    Xv_drag_drop        dragSite           - The XView drag site.
*    Selection_item      dragFileNameSelection - An XView selection item
*    used to hold the drag file name.
*    Selection_item      dragMessageSelection - An XView selection item
*    used to hold the drag message.
*    Selection_item      dragObjectSelection - An XView selection item
*    used to hold the drag object.
*
*    UICHandlerFunction  dragHandler       - The user specified drag handler.
*    UICHandlerFunction  dropHandler       - The user specified drop handler.
*
*    int                 locDragCount - Use for detecting when to source a drag.   
*
*    Boolean             isComponentGroup - TRUE if this object is a
*    ComponentGroup.
*    Boolean             isDropTarget - TRUE if this object is a DropTarget.
*    Boolean             isBaseWindow - TRUE if this object is a BaseWindow.
*    Boolean             isHotRegion - TRUE if this object is a HotRegion.
*    Boolean             isScroller - TRUE if this object is a Scroller.
*    Boolean             isUIComponent - TRUE if this object is a UIComponent.
*    Boolean             isUIMenu - TRUE if this object is a UIMenu.
*
*    Boolean             isDrawableOnCanvas - TRUE if this object
*    is drawable on an XView canvas.
*
*    Boolean		 useDefaultEventHandlerFlag - TRUE if the default
*    event handler should be used.
*
*    Boolean		 useDefaultDragHandlerFlag - TRUE if the default
*    drag handler should be used.
*
*    Boolean		 useDefaultDropHandlerFlag - TRUE if the default
*    drop handler should be used.
*
*    int		 uiObjectID         - The object's unique ID.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*            void initUI (int &argc, char **argv = (char **) NULL,
*			  UICAttribute attribute = UIC_NO_ATTRIBUTE,
*                         ...) -
*    In addition to initializing the graphics server, this function also is
*    used to specify attributes for the current session with the UIC. The
*    following attributes are defined:
*
*     Name			Type of value	Purpose
*     ----			-------------	-------
*     UIC_OPEN_PS_CONNECTION	Boolean		Specifies whether or not to
*						a connection to the NeWS
*						wire service for PostScript
*						useage.
*
*     UIC_TEXT_DOMAIN		char *		Specifies a text domain to
*						to bind for int'lization
*
*     UIC_LOCALE_DIR		char *		Specifies a int'lization
*						message directory.
*
*    If no attributes are to be set, use one of the following:
*
*       	initUI (argc, argv);
*       	int no_init_ui_args=0; initUI (no_init_ui_args);
*
*    const char *getVersion () - Returns the release compatibility
*				 version of the UIC.  Currently, all versions
*				 of the UIC that return "UICV1" are compatible.
*
*    void useResourceDB (char *) - Specifies the XView instance name to use
*				   to look up X reousrce DB values to for
*				   this object's attributes.
*
*    void setAccelerator (char, UICHandlerFunction) - Sets the specified
*    character as an accelerator.  Calls the specified handler when
*    triggered.
*
*    char getAcceleratorChar () - Gets the last accelerator character that
*    was triggered on this object.
*
*    int getRootWindowWidth () - Returns the width of the root window
*				 (frame buffer).
*
*    int getRootWindowHeight () - Returns the height of the root window
*				  (frame buffer).
*
*    int getObjectID () - Returns the unique ID of this object.
*
*    void setDestroyChildren (Boolean) - Specifies whether or not to
*    destroy the UIC objects that are children of this object when
*    this object is destroyed.  Default: FALSE.
*
*    void setDestroyXViewObject (Boolean) - Specifies whther or not to
*    destroy the XView object managed by this object when this object
*    destroyed.  Default: TRUE.
*
*    UIObject *getParent () - Returns the parent object of this object.
*
*    virtual void  	 setLocation (int, int)       - Sets the location 
*    (X and Y coordinates) of this object.
*
*    virtual void  	 setSize (int, int)           - Sets the size
*    (width and height) of this object.
*
*    virtual void        setX (int)                   - Sets the X coordinate
*    of this object.  Default: Not set.
*
*    virtual void        setY (int)                   - Sets the Y coordinate
*    of this object.  Default: Not set.
*
*    virtual void        setWidth (int)               - Sets the width of this 
*    object.  Default: Not set.
*
*    virtual void 	 setHeight (int)              - Sets the height of this
*    object.  Default: Not set.
*
*    virtual void	 setForegroundColor (Color)   - Finds the associated 
*    index of the Color and sets the foregroundColor to this value. 
*    Default: Not set.
*
*    Color is a UIC color data type. It has the following data members:
*    int red - Contains the red color component,
*    int green - Contains the green color component,
*    int blue - Contains the blue color component.
*
*    virtual void	 setForegroundColor (char *)  - Finds the associated 
*    index of ther named color and sets the foregroundColor to this value. 
*    Default: Not set.
*
*    virtual void	 setBackgroundColor (Color)   -  Finds the associated 
*    index of the Color and sets the backgroundColor to this value. 
*    Default: Not set.
*
*    virtual void	 setBackgroundColor (char *)  - Finds the associated 
*    index of ther named color and sets the backgroundColor to this value. 
*    Default: Not set.
*
*    virtual void	 setEventHandler (UICHandlerFunction) - Sets the 
*    function that is to execute when an event is noted in this subject.  
*    Default: Not set.
*
*    void                addInputFunction (UICInputFunction input_function,
*					   int              file_descriptor)
*    - Specifies a function to call when the specified file descriptor is
*    active.
*
*    void                removeInputFunction (UICInputFunction input_function,
*					      int              file_descriptor)
*    - Removes the specified function from the list of functions to call when
*    the specified file descriptor is active.
*
*    int                 addHotRegion (HotRegion &hot_region)   - Adds a hot 
*    region to the list of hot regions for this object.  Returns the index 
*    of this hot region.
*
*    HotRegion           *removeHotRegion (int index)          -  Removes 
*    the hot region specified by index from the list of hot regions for 
*    this object. 
*
*    HotRegion           *removeHotRegion (HotRegion &hot_region) - Removes
*    the hot region specified by pointer from the list of hot regions for this 
*    object.  
*
*    HotRegion		 *getActiveHotRegion (int index)       - Returns a 
*    pointer to a hot region where the last event took place.  The "index" 
*    is an index with this list.
*
*    Boolean		  isHotRegionEvent ()                  - TRUE if the 
*    last event occurred in a hot region.
*
*    int		  addEventTarget (UIObject&,
*					  UICTargetFunction,
*					  int xview_event_action = UIC_NOT_SET,
*					  int xview_event_id = UIC_NOT_SET) -
*    Adds an event notification target to the list of targets.  If an event
*    action or ID is specified, the target is notified only if the specified
*    event action or ID occurs.
*
*    UICGeneric 	  getXViewObject ()          - Returns the XView 
*    pointer of the UIObject.
*
*    void		  setXViewObject (UICGeneric) - Sets the XView 
*    pointer of the UIObject.  This is meant for situations where
*    the programmer wants to create his own XView object but use
*    UIObject member functions to set or get attributes.  It must
*    be used with extreme caution.
*
*    XID		  getXID ()                  - Returns the XID of
*    the UIObject.
*
*    void 		  setHelp (char *helpentry)  - Sets the name by which 
*    Help text may be found in the Help file.  Default: Not set.
*
*    virtual int 	  getX ()                    - Returns the current X 
*    coordinate of the object.
*
*    virtual int 	  getY ()                    - Returns the current Y
*    coordinate of the object.
*
*    virtual void	  getLocation (int&, int&)   - Returns the location 
*    (X and Y coordinates) of this object.
*
*    virtual int 	  getHeight ()               - Returns the current 
*    height of the object.
*
*    virtual int 	  getWidth ()                - Returns the current 
*    width of the object.
*
*    virtual void	  getSize (int&, int&)       - Returns the size 
*    (height and width) of this object.
*
*    InputEvent		 *getEvent ()                - Returns the last 
*    event that occurred on this object.
*
*    void                 setEventResult (EventResult result)        -
*    Sets the value that is to be returned to the XView notifier after the 
*    event handler function is executed.  At this time it is only
*    used for events on XView panels, icons and scrollbars. Default: Not set.
*
*    void                 setObjectData (char *name, Generic &data)  -
*    Associates an object with the current one.  Arguments are the name by
*    which the object may be retrieved, and the object itself.
*
*    void                 setObjectData (char *name, Generic *data)  -
*    Associates an object with the current one.  Arguments are the name by
*    which the object may be retrieved, and a pointer to the object.
*
*    Generic*            &getObjectData (char *name)                 - Returns
*    an object that had been previously associated with this one.  The argument
*    is the name by which this object is known.
*
*    Generic             *findObjectData (const char *name)	     - Returns
*    a pointer to an object that had been previously associated with this one.
*    The argument is the name by which this object is known.  If the name
*    does not correspond to an existing object, NULL is returned.
*
*    Generic             *deleteObjectData (char *name)              - Deletes 
*    the object data corresponding to the specified name.
*
*    Generic             *traverseObjectData (Boolean reset_flag,
*                                             char* &name) -
*    Traverses the object data that you associated with this object.  The
*    member function returns the object data pointer by its return value,
*    and returns the object data name through the char* & argument.  If the
*    Boolean is TRUE, traversal starts over from the beginning of the object
*    chain; otherwise, traversal continues.  When there are no more associated
*    objects, the member function returns NULL.  This function allows you to
*    find out about object data without having to know an object's name.
*
*    virtual void         setFont (CharacterFont &)   - Sets the font to be
*    used with this object.
*
*    GC                   getFontGC ()                - Gets the X font
*    graphics context of this object.
*
*    void		  consumeEvent (int)          - Event that this object 
*    is interested in knowing about.  The argument is the event ID of the 
*    event.
*
*    void                 useDefaultColorMap ()       - Specifies that the 
*    default color map should be created and used.  If a background or 
*    foreground color is specified and no explicit color map has been set 
*    the default is used automatically.  This results in this member function 
*    not usually being used.
*
*    void                 setColorMap (ColorMap&)     - Sets the ColorMap for
*    this object. Default: Set to default ColorMap.
*
*    virtual void 	  setLabel (char *)           - Sets the label of this 
*    object. Default: Not set.
*
*    virtual char        *getLabel (char *&)          - Gets the label of this 
*    object. Default: Not set.
*
*    virtual char	 *getValue (char *&)          - Virtual function which
*    is implemented in derived classes.
*
*    virtual int	  getValue (int &)            - Virtual function which
*    is implemented in derived classes.
*
*    virtual char	 *getChoice (char *&)         - Virtual function which
*    is implemented in derived classes.
*
*    virtual int 	  getChoice (int &)           - Virtual function which
*    is implemented in derived classes.
*
*    virtual int 	  getChoice (int, int &)      - Virtual function which
*    is implemented in derived classes.
*
*    virtual char	 *getChoice (int, char *&)    - Virtual function which
*    is implemented in derived classes.
*
*    virtual int 	  getNumberSelected ()        - Virtual function which
*    is implemented in derived classes.
*
*    virtual void         show (Boolean)              - Specifies whether or
*    not this object should be displayed.
*
*    virtual Boolean      isClosed () - Retruns TRUE if this object's XView
*    frame is iconized.
*
*    	     virtual void setDropRectangle (int x, int y, int width, int height)
*			  - Specifies the rectangular region that can
*			    be dropped on relative to this objects owner
*			    if it is a UIComponent derived object, or in the
*			    objects's own coordinate system if the object
*			    is a UIWindow or UIDisplay derived object.
*
*    	     virtual void setDefaultDropSite (Boolean)
*			  - Specifies whether or not the object is the
*			    default drop site for the window it is in.
*			    Only one default drop site should be specified
*			    per window.
*
*            virtual void setDraggable (Boolean)
*			  - Specifies whether or not this object is draggable.
*
*            virtual void setAcceptDrops (Boolean flag)
*			  - Specifies whether or not this object accepts
*			    drops.
*
*    	     void	  setDragCursor (Image &cursor_image, int x=0, int y=0)
*    	     void	  setDragCursor (char *cursor_file, int x=0, int y=0)
*			  - Specifies the drag cursor and the hot spot
*			    on the cursor.
*
*            void	  setDragCursorHotSpot (int, int)
*			  - Specifies the hot spot on the drag cursor,
*			    
*    	     void	  setAcceptDropCursor (Image &cursor_image,
*					       int x=0, int y=0)
*    	     void	  setAcceptDropCursor (char *cursor_file,
*					       int x=0, int y=0)
*			  - Specifies the accept drop cursor and the hot spot
*			    on the cursor for drags originating from this
*			    object.
*			    
*            void	  setAcceptDropCursorHotSpot (int, int)
*			  - Specifies the hot spot on the accept drop cursor.
*
*            void         setDragFileName (char *)
*                         - Specifies a file name to be passed to the
*                           drop destination object.
*
*	     void	  setDragMessage (char *)
*			  - Specifies a text message to be passed to the
*			    drop destination object.
*
*	     void	  setDragObject (Generic &)
*	     void	  setDragObject (Generic *)
*			  - Specifies an object to be passed to the drop
*			    destinatin object.  This can only be used for
*			    application local drag and drop.
*
*	     char	 *getDragFileName ()
*			  - Gets the file name that the will be sent if a drag
*                           operation is initiated.  The caller must free up
*                           the returned string.
*
*	     char	 *getDragMessage ()
*			  - Gets the message that the will be sent if a drag
*                           operation is initiated.  The caller must free up
*                           the returned string.
*
*	     Generic	 *getDropObject ()
*			  - Gets a pointer to the object that the will be sent
*                           if a drag operation is initiated.
*
*	     char	 *getDragHostName ()
*			  - Gets the host name of the origin machine of
*			    the last drag and drop operation that dropped
*			    onto this object.  The caller must free up
*                           the returned string.
*
*	     char	 *getDropMessage ()
*			  - Gets the message that the last drag and drop
*			    operation dropped onto this object.  The caller must
*                           free up the returned string.
*
*	     char	 *getDropFileName ()
*			  - Gets the file name that the last drag and drop
*			    operation dropped onto this object.  The caller must
*                           free up the returned string.
*
*	     Generic	 *getDropObject ()
*			  - Gets a pointer to the object that the last drag and
*			    drop operation dropped onto this object.
*
*            int          getDragResult ()
*                         - Returns the result of the last drag from this
*			    object.
*
*	     DragStatus   getDragStatus ()
*			  - Returns the status of the current drag
*			    operation from this object.  The return
*			    values will be DRAG_START or DRAG_END indicating
*			    the beginning or ending of a drag operation.
*			    DRAG_CONVERT is returned if drag selection
*			    convert data is expected.  DRAG_DELETE is
*			    returned if a delete signal was sent by the
*			    drop receiver.  DRAG_NO_DRAG is returned if
*			    no drag is in operation. This should be used
*			    from inside a drag handler.
*
*            void         abortDrag ()
*			  - If called while the dragStatus is DRAG_START,
*			    the current drag operation is aborted.
*
*	     Selection_owner getDragConvertSelection ()
*			  - Returns the XView SELECTION_OWNER object
*			    associated with the current convert.
*
*            void	  getDragConvertData (Atom          &type,
*					      UICGeneric    &data,
*					      unsigned long &length,
*					      int	    &format)
*			  - Get the data from the current DRAG_CONVERT.
*
*            void	  setDragConvertData (Atom          type,
*					      UICGeneric    data,
*					      unsigned long length,
*					      int	    format)
*			  - Set the data for the current DRAG_CONVERT.
*
*            Boolean	  setSendDragDelete (Boolean)
*			  - Specifies if a "DELETE" selection should
*			    be sent after handling a drop and the event
*			    action is ACTION_MOVE_COPY.
*
*            Selection_requestor getDropSelection ()
*                         - Returns the XView selection requestor
*                           associated with drops that occur on this
*                           object.
*
*            Xv_drag_drop getDragSite ()
*                         - Returns the XView drag and drop object
*                           associated with this object.
*
*            int          getDropHandler (UICHandlerFunction)
*			  - Specify the handler to be called when something
*			    is dropped onto this object.
*
*	     void	  setDragHandler (UICHandlerFunction)
*			  - Specify the handler to be called when something
*			    is being dragged from this object.
*
*            void         postXViewEvent (Notify_event, Notify_event_type)
*                         - Post the specified event for this object.
*
*    virtual void	  createObject (UIObject *parent=NULL) - Virtual 
*    function which is implemented in derived classes.  createObject is always
*    used to create the XView counterpart to the UIC object.  The parent
*    is the UIC object parent and is used to obtain the XView parent of
*    the XView object.
*
*    void createXViewObjects () - Creates the XView objects corresponding
*    to the main BaseWindow of the application and all of its sibling's XView
*    objects, and any child window XView objects.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*            void        setEventInformation(Event *event) - Sets the object's
*    internal name information structure.
*
*    	     void        resetEventInformation() - Erases or resets the 
*    internal event information.
*
*            void        setHelpData (UICGeneric xview_object = NULL) - Sets
*    the XView help data string.  If no xview_object is specified, owObject
*    is used.
*
*    virtual void        createDragSite () - Creates a drag site for this
*    object.
*
*    virtual void        createDropSite () - Creates a drop site for this
*    object.
*
*    virtual void        createHotRegionDragSite (HotRegion&) - Creates a
*    drag site for this object using the specified hot region.
*
*    virtual void        createHotRegionDropSite (HotRegion&) - Creates a
*    drop site for this object using the specified hot region.
*
*            void        createXViewDragSite (UICGeneric) - Creates that actual
*    XView drag site for the specified XView object.
*
*            void        createXViewDropSite (UICGeneric, Rect *) - Creates that
*    actual XView drop site for the specified XView object and the specified
*    rectangle.
*
*            void	 changeDropSiteSize (Rect *) - Changes the size of
*    of the drop site to the specified rectangle unless there is a
*    drop rectangle already specified.
*
*            void        checkForDragEvent () - Checks to see if the last event
*    is a drag event and initiates the drag.
*
*            void        checkForDropEvent () - Checks to see if the last event
*    is a drop event and handles the drop.
*
*            Boolean     isEventOnADropSite (InputEvent *) - Returns TRUE
*    if the specified event is on a drop site.
*
*            Boolean     isEventOnADropTarget (InputEvent *) - Returns TRUE
*    if the specified event is on a drop target.
*
*            void	 getInternalDropData (Selection_requestor=NULL) - Get
*    the data from the current drop and store it internally using the specified
*    selection requestor.
*
*            void	 initializeColorMap (UIObject *parent=NULL,
*					     Boolean   force=FALSE) - Creates
*    the application color map and sets the default background and foreground
*    colors for this object.  The color map will only be initialized if
*    a foreground or background color was specified or if it is forced.
*
*            void	 setUIObjectAttributes (UIObject *) - Sets attributes
*    that are common to most UIObject objects.
*
*    void useDefaultEventHandler (Boolean) - Specifies if the default event
*    handler should be used.
*
*    void useDefaultDragHandler (Boolean) - Specifies if the default drag
*    handler should be used.
*
*    void useDefaultDropHandler (Boolean) - Specifies if the default drop
*    handler should be used.
*
*    virtual void defaultEventHandler () - The default event handler.
*
*    virtual void defaultDragHandler () - The default drag handler.
*
*    virtual void defaultDropHandler () - The default drop handler.
*
*    static  void	  acceleratorProc (Xv_opaque, Event *) -
*			  - XView wrapper for an accelerator notify proc.
*
*    static  Notify_value inputFunc (Notify_client client, int fd)
*                         - XView notifier input function.
*
*    static  int dragSelectionConvertProc (Selection_owner,
*                                          Atom *,
*                                          Xv_opaque *,
*                                          unsigned long *,
*                                          int *) - XView selection convert proc
*    for this object's drag selection.
*
*    static  void dragSelectionDoneProc (Selection_owner, Xv_opaque, Atom)
*    	          - XView selection done proc for this object's drag selection.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*            void	 setUIObjectDefaults () - Sets the object's data
*    member's default values.
*
*C-*/
#ifndef _UIOBJECT_H
#define _UIOBJECT_H

// Includes

// UIC Includes
#include "UICTypes.h"
#include "private/UICPrivateTypes.h"

// Generic Includes
#include "Generic.h"
#include "GenericHash.h"
#include "GenericList.h"

#ifndef NO_XVPS
#if !XV_VERSION_NUMBER || (XV_VERSION_NUMBER < 3200)
#include "private/postscript_canvas.h"
#endif
#endif

// Class Declarations
class ColorMap;
class CharacterFont;
class HotRegion;
class Image;

// Local Class Definitions
class InputEvent : public Generic {
public:
  InputEvent ()
    {
      if (!thresholdsInitialized) {
        timeThreshold = defaults_get_integer ("openwindows.multiclicktimeout",
					      "OpenWindows.MultiClickTimeout",
					      4);
 
	// Convert for timeval.tv_usec (tenths of seconds to microseconds)
	timeThreshold *= 100000;
        
	dragThreshold = defaults_get_integer ("openwindows.dragthreshold",
					      "OpenWindows.DragThreshold",
					      5);
 
	thresholdsInitialized = TRUE;
      }
    }

  ~InputEvent () {}

  int getID ()
    { return (xViewEvent ? event_id (xViewEvent) :
	      UIC_NOT_SET); }
  int getAction ()
    { return (xViewEvent ? event_action (xViewEvent) :
	      UIC_NOT_SET); }
  int getX ()
    { return (xViewEvent ? event_x (xViewEvent) :
	      UIC_NOT_SET); }
  int getY () { return (xViewEvent ? event_y (xViewEvent) :
			UIC_NOT_SET); }
  timeval *getTime () { return (xViewEvent ?
				&event_time (xViewEvent) :
				(timeval *) NULL); }
  Boolean isButtonUp () { return (xViewEvent ?
				  event_is_up (xViewEvent) :
				  FALSE); }
  Boolean isButtonDown () { return (xViewEvent ?
				    event_is_down (xViewEvent) :
				    FALSE); }
  Boolean isSelectButtonDown () { return (xViewEvent ?
				          action_select_is_down (xViewEvent) :
				          FALSE); }
  Boolean isAdjustButtonDown () { return (xViewEvent ?
				          action_adjust_is_down (xViewEvent) :
				          FALSE); }
  Boolean isMenuButtonDown () { return (xViewEvent ?
				        action_menu_is_down (xViewEvent) :
				        FALSE); }
  Boolean isAscii () { return (xViewEvent ?
			       event_is_ascii (xViewEvent) :
			       FALSE); }
  Boolean isDoubleClick () { return (xViewEvent ?
				     detectDoubleClick (xViewEvent) :
				     FALSE); }

  Event *getXViewEvent () { return xViewEvent; }

protected:
  Event        *xViewEvent;
  timeval       currentSelectTime;
  timeval       lastSelectTime;
  Rect          lastRegion;
 
  static short  thresholdsInitialized;
  static int    timeThreshold;
  static int    dragThreshold;
  
private:
  void    setXViewEvent (Event *event) { xViewEvent = event; }
  Boolean detectDoubleClick (Event *xview_event);

friend class HotRegion;
friend class NoticePrompt;
friend class UIObject;
};

class UITarget : public Generic {
public:
  UITarget () { targetObject = (UIObject *) NULL;
		targetFunction = (UICTargetFunction) NULL;
		eventAction = UIC_NOT_SET;
		eventID = UIC_NOT_SET; }
  
  UIObject          *targetObject;
  UICTargetFunction  targetFunction;
  int		     eventAction;
  int		     eventID;
};

class UIInput : public Generic {
public:
  UIInput () { inputFunction = (UICInputFunction) NULL;
	       fileDescriptor = -1; }
  
  UICInputFunction inputFunction;
  int		   fileDescriptor;
};

class UIAccelerator : public Generic {
public:
  UIAccelerator () { acceleratorObject = (UIObject *) NULL;
		     acceleratorChar = '\0';
		     acceleratorHandler = (UICHandlerFunction) NULL; }

  UIObject           *acceleratorObject;
  char                acceleratorChar;
  UICHandlerFunction  acceleratorHandler;
};

// Class Definition
class UIObject : public Generic {
public:

    			 UIObject ();
			 UIObject (char *);
			~UIObject ();

    void	         initUI (int &argc, char **argv = (char **) NULL,
				 UICAttribute = UIC_NO_ATTRIBUTE,
				 ...);

    const char          *getVersion ();

    void		 useResourceDB (char *resource_name)
			 {
			   if (resourceName) delete resourceName;

			   if (resource_name && strlen (resource_name)) 
			     resourceName =
			       strcpy (new char [strlen (resource_name) + 1],
				       resource_name);
			   else 
			     resourceName = (char *) NULL;

			   if (owObject)
			     xv_set (owObject,
				     XV_INSTANCE_NAME, resourceName,
				     NULL);
			 }

    void		 setAccelerator (char               accelerator_char,
					 UICHandlerFunction handler)
			 {
			   UIAccelerator *accelerator = new UIAccelerator;
			   accelerator->acceleratorObject = this;
			   accelerator->acceleratorChar = accelerator_char;
			   accelerator->acceleratorHandler = handler;

			   acceleratorList.addItem ((Generic *) accelerator);

			   if (owObject) {
			     xv_set (owObject,
				     FRAME_ACCELERATOR,
				       accelerator_char,
				       &UIObject::acceleratorProc,
				       accelerator,
				     NULL);
			   }
			 }

    char		 getAcceleratorChar ()
			 {
			   return acceleratorChar;
			 }

    int                  getRootWindowWidth ()
			 { initUI (noInitUIArgs);
			   return (rootWindowWidth); }

    int                  getRootWindowHeight ()
			 { initUI (noInitUIArgs);
			   return (rootWindowHeight); }

    int                  getObjectID () { return uiObjectID; }

    void		 setDestroyChildren (Boolean flag)
			 { destroyChildren = flag; }

    void                 setDestroyXViewObject (Boolean flag)
                         { destroyXViewObject = flag; }

    UIObject		*getParent () { return parentObject; }

    virtual void  	 setLocation (int, int);
    virtual void  	 setSize (int, int);
    virtual void         setX (int);
    virtual void         setY (int);
    virtual void         setWidth (int);
    virtual void 	 setHeight (int);

    virtual void	 setForegroundColor (Color);
    virtual void	 setForegroundColor (char *);

    virtual void	 setBackgroundColor (Color);
    virtual void	 setBackgroundColor (char *);

    virtual void	 setEventHandler (UICHandlerFunction);

    void		 addInputFunction (UICInputFunction input_function,
					   int		    file_descriptor)
      {
        if (owObject)
          (void) notify_set_input_func (owObject,
				        (Notify_func) &UIObject::inputFunc,
				        file_descriptor);

        UIInput *input = new UIInput;
        input->inputFunction = input_function;
        input->fileDescriptor = file_descriptor;
        inputFunctions.addItem (input);
      }

    void		 removeInputFunction (UICInputFunction input_function,
                                              int              file_descriptor)
      {
	UIInput *input_entry;
	int      num_input_functions = inputFunctions.getSize ();
	for (int i=0; i < num_input_functions; i++) {
	  input_entry = (UIInput *) inputFunctions [i];
	  if ((input_entry->inputFunction == input_function) &&
	      (input_entry->fileDescriptor == file_descriptor)) {
	    (void) inputFunctions.removeItem (i);
	    break;
	  }
	}
      }
			 
    int			 addHotRegion (HotRegion &hot_region);
    HotRegion           *removeHotRegion (int index);
    HotRegion           *removeHotRegion (HotRegion &hot_region);

    HotRegion		*getActiveHotRegion (int index) 
			 { HotRegion *hot_region = (HotRegion *) NULL;

			   if ((index > 0) &&
			       (index <= activeHotRegions.getSize ()))
			     hot_region = (HotRegion *)
					activeHotRegions [index-1];
			     
			   return hot_region;  }

    Boolean		 isHotRegionEvent ()
			 { return hotRegionEventFlag; }

    int			 addEventTarget (UIObject&,
					 UICTargetFunction,
					 int xview_event_action = UIC_NOT_SET,
					 int xview_event_id = UIC_NOT_SET);

    UICGeneric 		 getXViewObject ()
			 { return (owObject); }

    void		 setXViewObject (UICGeneric xview_object)
			 { owObject = xview_object; }

    XID			 getXID ()
		         { XID xid = (XID) UIC_NOT_SET;
			   if (owObject)
			     xid = (XID) xv_get (owObject, XV_XID);
			   return xid;
			 }

    void 		 setHelp (char *helpentry)
			 { help = new char [strlen (helpentry) + 1];
    			   strcpy (help, helpentry); }

    virtual int 	 getX ();
    virtual int 	 getY ();
    virtual void	 getLocation (int&, int&);
    virtual int 	 getHeight ();
    virtual int 	 getWidth ();
    virtual void	 getSize (int&, int&);

    InputEvent		*getEvent ()
			 { return (&lastEvent) ; }

    void                 setEventResult (EventResult result)
                         { eventResultSet = TRUE; 
			   eventResult = result; }

    void                 setObjectData (char *name, Generic &data)
                         { objectData->addItem (name, &data); }

    void                 setObjectData (char *name, Generic *data)
                         { objectData->addItem (name, data); }

    Generic*            &getObjectData (char *name)
                         { return (*objectData) [name]; }

    Generic             *findObjectData (const char *name)
			 { return objectData->getItem (name); }

    Generic             *deleteObjectData (char *name)
			 { return objectData->removeItem (name); }

    Generic             *traverseObjectData (Boolean reset_flag, char* &name)
                         { return objectData->traverse (reset_flag, name); }

    virtual void         setFont (CharacterFont &);
    GC                   getFontGC ();
    void		 consumeEvent (int);
    void                 useDefaultColorMap ();
    void                 setColorMap (ColorMap&);

    virtual void 	 setLabel (char *);
    virtual char	*getLabel (char *&);
    virtual char	*getValue (char *&);
    virtual int		 getValue (int &);
    virtual char	*getChoice (char *&);
    virtual int 	 getChoice (int &);
    virtual int 	 getChoice (int, int &);
    virtual char	*getChoice (int, char *&);
    virtual int 	 getNumberSelected ();

    virtual void         show (Boolean);
    virtual Boolean	 isClosed ();

    virtual void         setDropRectangle (int x, int y, int width, int height);

    virtual void         setDefaultDropSite (Boolean);

    virtual void         setDraggable (Boolean);
    virtual void         setAcceptDrops (Boolean);

            void         setDragCursor (Image &cursor_image, int x=0, int y=0);
            void         setDragCursor (char *cursor_file, int x=0, int y=0);
            void         setDragCursorHotSpot (int x, int y);

            void         setAcceptDropCursor (Image &cursor_image,
					      int x=0, int y=0);
            void         setAcceptDropCursor (char *cursor_file,
					      int x=0, int y=0);
            void         setAcceptDropCursorHotSpot (int x, int y);

            void         setDragFileName (char *file_name)
              { if (dragFileName) delete dragFileName;

                if (file_name)
                  dragFileName = strcpy (new char [strlen (file_name)+1],
					 file_name);
                else
                  dragFileName = strcpy (new char [1], "");

                if (dragFileNameSelection)
                  xv_set (dragFileNameSelection, SEL_DATA, dragFileName, NULL);
              }  

            void         setDragMessage (char *message)
	      { if (dragMessage) delete dragMessage;

		if (message)
		  dragMessage = strcpy (new char [strlen (message)+1], message);
		else
		  dragMessage = strcpy (new char [1], "");

		if (dragMessageSelection)
                  xv_set (dragMessageSelection, SEL_DATA, dragMessage, NULL);
	      }

            void         setDragObject (Generic &object)
	      { dragObject = &object;

                if (dragObjectSelection)
		  xv_set (dragObjectSelection, SEL_DATA, &dragObject, NULL);
	      }

            void         setDragObject (Generic *object)
	      { dragObject = object;

                if (dragObjectSelection)
		  xv_set (dragObjectSelection, SEL_DATA, &dragObject, NULL);
	      }

	    void	 setDragHandler (UICHandlerFunction handler)
	      { dragHandler = handler; }
	    void	 setDropHandler (UICHandlerFunction handler)
	      { dropHandler = handler; }

            char        *getDragFileName ()
	      { char *file_name;

		if (dragFileName)
		  file_name = strcpy (new char [strlen (dragFileName)+1],
				      dragFileName);
		else
		  file_name = (char *) NULL;

		return file_name;
	      }

            char        *getDragMessage ()
	      { char *message;

		if (dragMessage)
		  message = strcpy (new char [strlen (dragMessage)+1],
				    dragMessage);
		else
		  message = (char *) NULL;

		return message;
	      }

            Generic     *getDragObject () { return dragObject; }

            char        *getDragHostName ()
	      { char *host_name;

		if (dragHostName)
		  host_name = strcpy (new char [strlen (dragHostName)+1],
				      dragHostName);
		else
		  host_name = (char *) NULL;

		return host_name;
	      }

	    char        *getDropMessage ()
	      { char *message;

		if (dropMessage)
		  message = strcpy (new char [strlen (dropMessage)+1],
				    dropMessage);
		else
		  message = (char *) NULL;

		return message;
	      }

	    char        *getDropFileName ()
	      { char *file_name;

		if (dropFileName)
		  file_name = strcpy (new char [strlen (dropFileName)+1],
				      dropFileName);
		else
		  file_name = (char *) NULL;

		return file_name;
	      }

	    Generic     *getDropObject () { return dropObject; }

	    int          getDragResult () { return dragResult; }
	    DragStatus	 getDragStatus () { return dragStatus; }

	    void	 abortDrag () { abortDragFlag = TRUE; }

	    Selection_owner getDragConvertSelection ()
			    { return dragConvertSelection; }

            void         getDragConvertData (Atom          &type,
                                             UICGeneric    &data,
                                             unsigned long &length,
                                             int           &format)
			 { type   = dragConvertType;
			   data   = dragConvertData;
			   length = dragConvertLength;
			   format = dragConvertFormat; }

            void         setDragConvertData (Atom          type, 
                                             UICGeneric    data, 
                                             unsigned long length, 
                                             int           format) 
			 { dragConvertDataSet = TRUE;
	
			   dragConvertType   = type;
			   dragConvertData   = data;
			   dragConvertLength = length;
			   dragConvertFormat = format; }

	    void	 setSendDragDelete (Boolean flag)
			 { sendDragDelete = flag; }

            Selection_requestor getDropSelection ()
                                { return dropSiteSelection; }

            Xv_drag_drop getDragSite () { return dragSite; }

            Notify_error postXViewEvent (Notify_event      xview_event,
				         Notify_event_type when_hint)
                         { if (owObject)
			     return (notify_post_event (owObject,
							xview_event,
							when_hint));
			   else
			     return NOTIFY_UNKNOWN_CLIENT; }

    virtual void	 createObject (UIObject *parent=NULL);

            void         createXViewObjects ()
			 { if (!baseWindow) baseWindow = firstPopUp;

			   if (baseWindow) { 
			     if (!xViewObjectsCreated) {
			       initUI (noInitUIArgs);
			       ((UIObject *) baseWindow)->createObject ();

			       int num_groups = groupList->getSize ();
			       for (int i = 0; i < num_groups; i++)
				 ((UIObject *)
				  (*groupList) [i])->createObject ();
			     }

			     xViewObjectsCreated = TRUE;
			   }
			 }

protected:
    Boolean		 objectCreated;
    Boolean		 destroyChildren;
    Boolean		 destroyXViewObject;
    char		*resourceName;
    GenericList          acceleratorList;
    char		 acceleratorChar;
    int 	 	 xLoc;
    int		 	 yLoc;
    int	 	 	 height;
    int 	 	 width;
    char		*label;
    char 		*help;
    UICGeneric	 	 owObject;
    UIObject		*parentObject;
    UIObject		*serverParent;
    Boolean		 closed;
    int			 foregroundColor;
    int			 backgroundColor;
    UICHandlerFunction	 eventHandler;
    GenericList		 events;
    InputEvent		 lastEvent;
    Boolean		 eventResultSet;
    EventResult		 eventResult;
    GenericList		 inputFunctions;
    GenericList		 hotRegions;
    GenericList		 activeHotRegions;
    Boolean		 hotRegionEventFlag;
    GenericList		 eventTargets;
    CharacterFont       *font;
    GC                   fontGC;
    GenericHash         *objectData; 
    Boolean		 showFlag;
    Boolean		 noDragDrop;
    Boolean		 draggable;
    Boolean		 acceptDrops;
    Rect                 dropRectangle;
    Boolean              defaultDropSite;
    Boolean              newDragCursorImage;
    Image               *dragCursorImage;
    Cursor               dragCursor;
    int                  dragCursorX;
    int                  dragCursorY;
    Boolean              newAcceptDropCursorImage;
    Image               *acceptDropCursorImage;
    Cursor               acceptDropCursor;
    int                  acceptDropCursorX;
    int                  acceptDropCursorY;
    char                *dragMessage;
    Generic             *dragObject;
    char                *dragHostName;
    char                *dragFileName;
    char                *dropMessage;
    char                *dropFileName;
    Generic             *dropObject;
    int                  dragResult;
    DragStatus           dragStatus;
    Boolean		 abortDragFlag;
    Boolean		 dragConvertDataSet;
    Selection_owner      dragConvertSelection;
    Atom		 dragConvertType;
    UICGeneric		 dragConvertData;
    unsigned long	 dragConvertLength;
    int		 	 dragConvertFormat;
    Boolean	         sendDragDelete;
    Xv_drop_site         dropSite;
    Selection_requestor  dropSiteSelection;
    Xv_drag_drop         dragSite;
    Selection_item	 dragFileNameSelection;
    Selection_item	 dragMessageSelection;
    Selection_item	 dragObjectSelection;
    UICHandlerFunction   dragHandler;
    UICHandlerFunction   dropHandler;
    int                  locDragCount;
    Boolean              isComponentGroup;
    Boolean              isDropTarget;
    Boolean              isBaseWindow;
    Boolean              isHotRegion;
    Boolean              isScroller;
    Boolean              isUIComponent;
    Boolean              isUIMenu;
    Boolean              isDrawableOnCanvas;
    int			 uiObjectID;
    Boolean		 useDefaultEventHandlerFlag;
    Boolean		 useDefaultDragHandlerFlag;
    Boolean		 useDefaultDropHandlerFlag;

    static  Boolean	 initUIDone;
    static  int		 noInitUIArgs;
#ifdef SVR4
    static  char         applicationHostName [MAXNAMELEN];
#else
    static  char         applicationHostName [MAXHOSTNAMELEN];
#endif
    static  Display     *rootDisplay;
    static  int          rootWindowWidth;
    static  int          rootWindowHeight;
    static  Boolean	 monochrome;
    static  ColorMap    *colorMap;
    static  Boolean	 colorMapInitialized;
    static  Boolean      defaultColorMap;
    static  UIObject    *baseWindow;
    static  UIObject    *firstPopUp;
    static  GenericList *groupList;
    static  GenericList *dropSiteList;
    static  GenericList *dropTargetList;
    static  int          dragThreshold;
    static  Boolean      xViewObjectsCreated;
    static  int          numUIObjects;
    static  int          totalUIObjectsCreated;

            void         setEventInformation (Event *event);
            void         resetEventInformation ();

            void         setHelpData (UICGeneric xview_object = NULL)
              { if (help) {
		  if (xview_object)
                    xv_set (xview_object, XV_HELP_DATA, help, NULL);
		  else
                    xv_set (owObject, XV_HELP_DATA, help, NULL);
	        }
              }

    virtual void	 createDragSite ();
    virtual void	 createDropSite ();

    virtual void	 createHotRegionDragSite (HotRegion&);
    virtual void	 createHotRegionDropSite (HotRegion&);

	    void         createXViewDragSite (UICGeneric);
	    void         createXViewDropSite (UICGeneric, Rect *);

	    void	 changeDropSiteSize (Rect *rectangle)
	    {
	      if (dropSite &&
		  (dropRectangle.r_width == 0) &&
		  (dropRectangle.r_height == 0)) {
		xv_set (dropSite, DROP_SITE_DELETE_REGION, NULL, NULL);
    	        xv_set (dropSite, DROP_SITE_REGION, rectangle, NULL);
	      }
	    }

            void         checkForDragEvent ();
            void         checkForDropEvent ();

	    Boolean	 isEventOnADropSite (InputEvent *event);
	    Boolean	 isEventOnADropTarget (InputEvent *event);

	    void	 getInternalDropData (Selection_requestor=NULL);

	    void	 initializeColorMap (UIObject *parent=NULL,
					     Boolean   force=FALSE);
	    void	 setUIObjectAttributes (UIObject *);

	    void	 useDefaultEventHandler (Boolean flag)
			 { useDefaultEventHandlerFlag = flag; }

	    void	 useDefaultDragHandler (Boolean flag)
			 { useDefaultDragHandlerFlag = flag; }

	    void	 useDefaultDropHandler (Boolean flag)
			 { useDefaultDropHandlerFlag = flag; }

    virtual void         defaultEventHandler () {}
    virtual void         defaultDragHandler () {}
    virtual void         defaultDropHandler () {}

#ifdef __GNUC__
public:
#endif
    static  void	 acceleratorProc (Xv_opaque, Event *);

    static  Notify_value inputFunc (Notify_client client, int fd);

    static  int	         dragSelectionConvertProc (Selection_owner,
						   Atom *,
						   Xv_opaque *,
						   unsigned long *,
						   int *);

    static  void         dragSelectionDoneProc (Selection_owner,
						Xv_opaque,
						Atom);

private:
            void	 setUIObjectDefaults ();

friend class CanvasShellDisplay;
};

#endif /* _UIOBJECT_H */
