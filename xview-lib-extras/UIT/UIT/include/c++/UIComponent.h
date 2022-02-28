/*H+ UIComponent.h 
*
*    SCCS ID: @(#)UIComponent.h 1.4 93/02/08 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE: 
*
*    Defines the UIComponent class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/06/91   Preparation for code review
*
*H-*/

/*C+ UIComponent
*
*    DESCRIPTION:
*
*    Defines the generic characteristics of UIComponents (Button, Slider,
*    ToggleChoice, etc...) in a GUI application. These include such things
*    as the layout orientation (horizontal or vertical) ways of object label
*    represenation (image or string).
*
*    INHERITED CLASSES:
*
*    UIObject - The base class of all UI classes in the UIC.
*
*    ORDINARY DATA MEMBERS:
*
*    Image	 	 *imageLabel     - Pointer to the label that
*    represents object. Could be either an image or a character.
*
*    int		  valueX         - The X coordinate of the location of 
*    the value associated with this UIComponent within the ComponentDisplay.
*
*    int		  valueY         - The Y coordinate of the location of
*    the value associated with this UIComponent within the ComponentDisplay.
*
*    Boolean              active         - TRUE if this component button
*    is active.
*
*    Position		  labelPosition  - Holds the location of the label 
*    associated with this UIComponent.  Position is a data type that has two 
*    values: LEFT and ABOVE.
*
*    int                  notifyResult   - User specified result of the last
*    notify event on this object.
*
*    int                  notifyStatus   - User specified status of the last
*    notify event on this object.
*
*    CharacterFont       *labelFont      - The font to be used to render the
*    component label.
*
*    CharacterFont       *valueFont      - The font to be used to render the
*    component value.
*
*    GenericList	  notifyTargets  - The list of targets to be notified
*    when a notify event occurs on this object.
*
*    UICHandlerFunction   notifyHandler  - Internal wrapper around an XView
*    notify proc for objects of classes derived from UIComponent.
*
*    Boolean		  useDefaultNotifyHandlerFlag - TRUE if default
*    notify handler should be used.
*
*    HotRegion		 *dragSiteHotRegion - This region specifies the
*    hot region used for detecting drags on a user defined component.  The
*    developer of the component should specify the hot region this points to.
*
*    HotRegion		 *dropSiteHotRegion - This region specifies the
*    hot region used for detecting drops on a user defined component.  The
*    developer of the component should specify the hot region this points to.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    virtual void    setLabel (char *) - Sets the text label of this
*            object.  Default: Not set.
*
*    virtual void    setLabel (Image &) - Sets the image label of this
*            object.  Default: Not set.
*
*    virtual void    setLabelPosition (Position) - Sets the location for
*            the label associated with this object.  Default: Not set.
*
*    virtual char   *getLabel (Image* &) - Gets the string label of this
*            object.
*
*    virtual Image  *getLabel (Image* &) - Gets the image label of this
*            object.
*
*    virtual void    setActive (Boolean) - Designates this component as
*            active.  Default: Active.
*
*            void    setNotifyHandler (UICHandlerFunction) - Sets the function
* 	     that is executed whenever an event occurred in this component.
*            Default: Not set.
*
*            void    setNotifyResult (int result) - Allows the user to specify
*            the result of a notify event.  Default: XV_OK.
*
*            void    setNotifyResult (DisplaySetting result) - Allows the user
*            to specify the result of a notify event.  Default: The return
*            value of the XView "panel_text_notify" function.
*
*            void    setNotifyStatus (int status) - Allows the user to specify
*            a panel notify status.  See the XView PANEL_NOTIFY_STATUS
*            attribute in the XView Reference Manual.
*
*    virtual void setLabelFont (CharacterFont &) - Sets the
*            character font for this UIComponent's label.  Default: Not set.
*
*    virtual void    setValueFont (CharacterFont &) - Sets the character
*	     font for this UIComponent's value.  Default: Not set.
*
*            int     addNotifyTarget (UIObject &,
*                         	      UICTargetFunction,
*                         	      int xview_event_action = UIC_NOT_SET,
*                         	      int xview_event_id = UIC_NOT_SET) - Adds
*	     the specified object as a target to be notified when the
*            UIComponent is notified.  The target function is called with
*            the pointer to the UIComponent and the target object.  If an
*            event action or ID is specified, then the target is notified
*            only when an event with that action or ID occurs.
*
*    virtual void    setValueX (int x) - Sets the X coordinate of the
*            location of the value associated with this UIComponent within the
*	     ComponentDisplay.  Default: Not set.
*
*    virtual void    setValueY (int y) - Sets the Y coordinate of the
*            location of the value associated with this UIComponent within the
* 	     ComponentDisplay.  Default: Not set.
*
*    virtual int     getValueX () - Gets the X coordinate of the
*            location of the value associated with this UIComponent within the
* 	     ComponentDisplay.  Default: Not set.
*
*    virtual int     getValueY () - Gets the Y coordinate of the
*            location of the value associated with this UIComponent within the
*	     ComponentDisplay.  Default: Not set.
*
*    virtual void    setForegroundColor (Color)
*    virtual void    setForegroundColor (char *) -
*	     Sets the foreground color of the component.  The color is
*	     specified by either name or a UIC color structure containing
*	     the RGB components of the color. 
* 
*    virtual void    repaintComponent (ComponentDisplay *) - Virtual function 
*    which is implemented in a user-defined component.  This function is
*    called by ComponentDisplay whenever the component needs to be repainted.
*
*    virtual void    componentHotRegionEventProc (ComponentDisplay *) - Virtual
*    function which is implemented in a user-defined component.  This function
*    is called by ComponentDisplay whenever an event occurs inside of a user-
*    defined component's hot region.
*
*    virtual void    componentBackgroundEventProc (ComponentDisplay *) - Virtual
*    function which is implemented in a user-defined component.  This function
*    is called by ComponentDisplay whenever an event occurs inside the
*    ComponentDisplay, but outside of the component's hot region.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*            void   notifyNotifyTargets () - Notifies all of the targets
*            associated with this component.
*
*    virtual void   createWithFonts (UIObject *,
*				     Xv_pkg *,
*				     void * = NULL, void * = NULL,
*				     void * = NULL, void * = NULL,
*				     void * = NULL, void * = NULL,
*				     void * = NULL, void * = NULL,
*                                    void * = NULL) - Creates the XView object
*    for this object.  Takes into account user specified fonts in the
*    xv_create call.  This version of createWithFonts expects the specification
*    of two XView attributes.
*
*            void   setUIComponentAttributes (UIObject *) - Sets XView
*    attributes that are common to the classes derived from UIComponent.
*
*            void   useDefaultNotifyHandler (Boolean) - Specifies if the
*    the default notify handler should be used.
*
*    virtual void   defaultNotifyHandler () - The default notify handler.
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
*            void createUserComponentDragSite (UICGeneric, HotRegion&) - Creates
*	     an XView drag site using the specified XView object and
*            the specified hot region for drag event detection.
*
*            void createUserComponentDropSite (UICGeneric, HotRegion&) - Creates
*	     an XView drop site using the specified XView object and
*            the size of the specified hot region.
*
*    STATIC MEMBER FUNCTIONS:
*
*            void eventProc (Panel_item, Event *) - Internal wrapper
*            around an XView event proc.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*            void setUIComponentDefaults () - Initializes data members.
*
*    FRIEND CLASSES:
*
*    CanvasShellDisplay - CanvasShellDisplay is a friend of UIComponent because
*    user defined components may be displayed on a ComponentDisplay and
*    internal UIComponent information is needed to determine when this is the
*    case.
*
*    ComponentDisplay - ComponentDisplay is a friend of UIComponent because
*    different types of components may be displayed differently on a
*    ComponentDisplay and internal UIComponent information is needed to
*    determine when this is the case.
*
*    ComponentGroup - ComponentGroup objects contain and manipulate UIComponents
*    and therefore need to be friends of UIComponents.
*
*C-*/
#ifndef _UICOMPONENT_H
#define _UICOMPONENT_H

// Includes
#include "UIObject.h"
#include "CharacterFont.h"
#include "Image.h"

#include <xview/rect.h>
#include <xview/win_input.h>
#include <pixrect/pixrect.h>
#include <pixrect/pixfont.h>
#include <xview/panel.h>
#include <xview/scrollbar.h>

// Class Declarations
class ComponentDisplay;

// Class Definition
class UIComponent : public UIObject {
public:
    		  	 UIComponent ();
    		  	 UIComponent (char *);
		  	 UIComponent (Image &);
			~UIComponent ();

    virtual void	 setLabel (char *);
    virtual void	 setLabel (Image &);
    virtual void         setLabelPosition (Position);

    virtual char	*getLabel (char* &label)
			 { return UIObject::getLabel (label); }
    virtual Image	*getLabel (Image* &);

    virtual void	 setActive (Boolean);

    void                 setNotifyHandler (UICHandlerFunction);

    void                 setNotifyResult (int result)
			 { notifyResult = result; }

    void                 setNotifyResult (DisplaySetting result)
			 { notifyResult = (int) result; }

    void                 setNotifyStatus (int status)
			 { notifyStatus = status; }

    virtual void         setLabelFont (CharacterFont &);
    virtual void         setValueFont (CharacterFont &);

    int		 	 addNotifyTarget (UIObject &,
					  UICTargetFunction,
					  int xview_event_action = UIC_NOT_SET,
					  int xview_event_id = UIC_NOT_SET);

    virtual void	 setValueX (int);
    virtual void	 setValueY (int);

    virtual int		 getValueX ();
    virtual int		 getValueY ();

    virtual void         setForegroundColor (Color);
    virtual void         setForegroundColor (char *);
	 
    virtual void	 repaintComponent (ComponentDisplay *);
    virtual void	 componentHotRegionEventProc (ComponentDisplay *);
    virtual void         componentBackgroundEventProc (ComponentDisplay *);

protected:
    Image	 	*imageLabel;
    int			 valueX;
    int			 valueY;
    Boolean		 active;
    Position		 labelPosition;
    int                  notifyResult;
    int                  notifyStatus;
    CharacterFont       *labelFont;
    CharacterFont       *valueFont;
    HotRegion		*dragSiteHotRegion;
    HotRegion		*dropSiteHotRegion;
    GenericList		 notifyTargets;
    UICHandlerFunction   notifyHandler;
    Boolean		 useDefaultNotifyHandlerFlag;

    void		 notifyNotifyTargets ();

    virtual void	 createWithFonts (UIObject *,
					  Xv_pkg *,
					  void * = NULL, void * = NULL,
					  void * = NULL, void * = NULL,
					  void * = NULL, void * = NULL,
					  void * = NULL, void * = NULL,
					  void * = NULL);

    void		 setUIComponentAttributes (UIObject *);

    void		 useDefaultNotifyHandler (Boolean flag)
			 { useDefaultNotifyHandlerFlag = flag; }

    virtual void	 defaultNotifyHandler () {}

    virtual void createDragSite ();
    virtual void createDropSite ();

    virtual void createHotRegionDragSite (HotRegion&);
    virtual void createHotRegionDropSite (HotRegion&);

    void	 createUserComponentDragSite (UICGeneric, HotRegion&);
    void	 createUserComponentDropSite (UICGeneric, HotRegion&);

#ifdef __GNUC__
public:
#endif
    static void	 eventProc (Panel_item, Event *);

private:
    void setUIComponentDefaults ();

friend class CanvasShellDisplay;
friend class ComponentDisplay;
friend class ComponentGroup;
};

#endif /* _UICOMPONENT_H */
