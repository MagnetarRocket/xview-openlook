/*H+ ComponentGroup.h
*
*    SCCS ID: @(#)ComponentGroup.h 1.3 93/04/09 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the ComponentGroup class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ ComponentGroup
*
*    DESCRIPTION:
*
*    The ComponentGroup allows an arbitrary set of components to be grouped
*    together and to positioned, activated, shown and hidden as a group.
*
*    INHERITED CLASSES:
*
*    UIComponent - The ComponentGroup class can used like a UIComponent.
*                  A ComponentGroup object can be added to a ComponentDisplay.
*
*    ORDINARY DATA MEMBERS:
*
*    int                numRows - The number of rows to display.
*    int                numColumns - The number of columns to display.
*    Boolean		useNumRows - TRUE if the number of rows
*				     in "numRows" should be used as is and
*				     the number of columns should be calculated.
*    RowAlignment       rowAlignment - The type of row alignment.
*    ColumnAlignment    columnAlignment - The type of column alignment.
*    Layout             groupLayout - The type of group layout.
*    int                horizontalOffset - The horizontal offset in pixels.
*    int                verticalOffset - The vertcal offset in pixels.
*    int                horizontalSpacing - The horizontal spacing in pixels.
*    int                verticalSpacing - The vertcal spacing in pixels.
*    UIObject	       *anchorObject - The object used to position the group on
*                                      a ComponentDisplay.
*    CompassPoint       anchorPoint - The point on the anchor object used to
*                                     determine group positioning on a
*                                     ComponentDisplay.
*    CompassPoint       referencePoint - The point in the group that is anchored
*                                        on the group's anchor point.
*    GenericList        componentList - The list of components in the group.
*    Boolean            activeFlag - Specifies whether or not the components in
*                                    the group are active.
*    ComponentDisplay  *parentDisplay - The parent of this group if it was
*                                       added to a ComponentDisplay.
*    ComponentGroup    *parentGroup - The parent of this group if it was added
*				      to a ComponentGroup.
*    Boolean		newLayout - TRUE if the layout needs to be
*				    determined when createObject is
*				    called.
*    Boolean	        newPosition - TRUE if a new position needs to be
*				      calculated.
*    int		currentX - The current X location of the group.
*    int		currentY - The current Y location of the group.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setLayout (Layout) - Specifies the layout format (row, column, both
*                              or neither).
*                              Default: NO_LAYOUT
*    void setDisplayedRows (int) - Specifies the number ofrows to display.
*                                  Default: Not set.
*    void setDisplayedColumns (int) - Specifies the number of columns to
*                                     display.
*                                     Default: Not set.
*    void setRowAlignment (RowAlignment) - Specifies the vertical alignment of
*                                          the rows.
*                                          Default: TOP_EDGES.
*    void setColumnAlignment (ColumnAlignment) - Specifies the horizontal
*						 alignment of the columns.
*                                                Default: LEFT_EDGES.
*    void setHorizontalOffset (int) - Specifies the horizontal offset in
*                                     pixels.
*                                     Default: 10.
*    void setVerticalOffset (int) - Specifies the vertical offset in pixels.
*                                   Default: 10.
*    void setHorizontalSpacing (int) - Specifies the horizontal spacing in
*                                      pixels.
*                                      Default: 10.
*    void setVerticalSpacing (int) - Specifies the vertical spacing in pixels.
*                                    Default: 10.
*    void setAnchor (UIObject &, CompassPoint) - Specifies the object and
*                                                compass point on the
*                                                object to anchor the
*                                                group on.
*                                                Default: Not set.
*    void setReferencePoint (CompassPoint) - Specifies the compass point in
*                                            the group to anchor.
*                                            Default: NORTHWEST.
*    int addComponent (UIComponent &) - Adds a component to the group.  Returns
*                                       the index of the component in the
*                                       group.
*    void         removeComponent (UIComponent &) - Removes the component from
*					            the ComponentGroup.
*    UIComponent *replaceComponent (int index, UIComponent &component)
*		 - Replaces the component in the group at the specified index
*		   with a new component.  Returns the a pointer to the
*		   component that was replaced.
*    UIComponent *replaceComponent (UIComponent &oldComponent,
*				    UIComponent &component)
*		 - Replaces the old component in the group with a new component.
*		   Returns a pointer to the component that was replaced.
*    int getNumberOfComponents () - Returns the number of components in the
*                                   group.
*    UIComponent *getComponent (int) - Returns the component with the specified
*                                      index.
*    virtual void setLocation (int x, int y)
*		  - Specifies the X-Y location of the group.
*		    Default: x=0, y=0.
*    virtual void setX (int) - Specifies the X position of the group.
*			       Default: 0.
*    virtual void setY (int) - Specifies the Y position of the group.
*			       Default: 0.
*    virtual void setValueX (int) - Specifies the X coordinate of the absolute
*				    position of the group relative to the
*				    first group member's value X coordinate.
*				    Default: 0.
*    virtual void show (Boolean) - Specifies whether or not to show the
*                                  components in the group.
*                                  Default: TRUE.
*    virtual void setActive (Boolean) - Specifies whether or not to activate
*                                       the components in the group.
*                                       Default: TRUE.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Sets all of the appropriate
*    attributes of the components contained in this group.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setComponentGroupDefaults () - Initializes data members.
*
*    void determineGroupLayout () - Determines the physical layout of the
*                                   group.
*    void determineGroupAnchor () - Determines where the group is anchored if
*				    it has an ancchor.
*    void determineGroupPosition () - Determines the physical position of the
*                                     group.
*
*    void determineRowLayout () - If the layout type is ROW_LAYOUT, then
*				  this is used to determine the layout.
*    void determineColumnLayout () - If the layout type is COLUMN_LAYOUT, then
*				  this is used to determine the layout.
*    void determineRowAndColumnLayout () - If the layout type is
*					   ROW_AND_COLUMN_LAYOUT, then this
*					   is used to determine the layout.
*
*    int  calculateColumnValueX (int column) - Given a column number returns the
*					       maximum value X offset in the
*					       column relative to the
*					       beginning of the column.
*
*    void getCompassPointLocation (UIObject *, CompassPoint, int &x, int &y)
*	  - Given a pointer to a UIObject and a compass point, returns the
*	    X-Y location of the compass point.
*
*C-*/
#ifndef _COMPONENT_GROUP_H
#define _COMPONENT_GROUP_H

// Includes
#include "ComponentDisplay.h"
#include "UIComponent.h"

// Class Definition
class ComponentGroup : public UIComponent {
public:
         		 ComponentGroup ();
   		        ~ComponentGroup ();

	    void         setLayout (Layout);
	    void         setDisplayedRows (int);
	    void         setDisplayedColumns (int);
	    void         setRowAlignment (RowAlignment);
	    void         setColumnAlignment (ColumnAlignment);
	    void         setHorizontalOffset (int);
	    void         setVerticalOffset (int);
	    void         setHorizontalSpacing (int);
	    void         setVerticalSpacing (int);
	    void         setAnchor (UIObject &, CompassPoint);
	    void         setReferencePoint (CompassPoint);
	    int          addComponent (UIComponent &);
	    void         removeComponent (UIComponent &);
	    UIComponent *replaceComponent (int, UIComponent &);
	    UIComponent *replaceComponent (UIComponent &, UIComponent &);
            int          getNumberOfComponents ();
            UIComponent *getComponent (int);

    virtual void	 setLocation (int, int);
    virtual void	 setX (int);
    virtual void	 setY (int);
    virtual void	 setValueX (int);
    virtual void         show (Boolean);	    
    virtual void         setActive (Boolean);
	    
protected:
    Layout            groupLayout;
    int               numRows;
    int               numColumns;
    Boolean	      useNumRows;
    RowAlignment      rowAlignment;
    ColumnAlignment   columnAlignment;
    int               horizontalOffset;
    int               verticalOffset;
    int               horizontalSpacing;
    int               verticalSpacing;
    UIObject         *anchorObject;
    CompassPoint      anchorPoint;
    CompassPoint      referencePoint;
    GenericList       componentList;
    Boolean           activeFlag;
    ComponentDisplay *parentDisplay;
    ComponentGroup   *parentGroup;
    Boolean	      newLayout;
    Boolean	      newPosition;
    int		      currentX;
    int		      currentY;

    virtual void      createObject (UIObject *parent=NULL);

private:    
    void setComponentGroupDefaults ();

    void determineGroupLayout ();
    void determineGroupAnchor ();
    void determineGroupPosition ();

    void determineRowLayout ();
    void determineColumnLayout ();
    void determineRowAndColumnLayout ();

    int  calculateColumnValueX (int);

    void getCompassPointLocation (UIObject     *object,
				  CompassPoint  point,
				  int           &x,
				  int		&y)
         {
	   switch (point) {
	     case NORTHWEST:
	     case WEST:
	     case SOUTHWEST:
	       x = object->getX ();
	       break;

	     case NORTH:
	     case CENTER:
	     case SOUTH:
	       x = object->getX () + object->getWidth () / 2;
	       break;

	     case NORTHEAST:
	     case EAST:
	     case SOUTHEAST:
	       x = object->getX () + object->getWidth ();
	       break;

	     default:
               x = object->getX (); 
	       break;
	   }

           switch (point) { 
             case NORTHWEST:
             case NORTH:    
             case NORTHEAST:
               y = object->getY ();
               break;
 
             case WEST:
             case CENTER:
             case EAST:
               y = object->getY () + object->getHeight () / 2; 
               break;  
 
             case SOUTHWEST:
             case SOUTH:
             case SOUTHEAST:
               y = object->getY () + object->getHeight ();
               break;
 
             default: 
               y = object->getY (); 
               break;
	   }
	 }
};

#endif /* _COMPONENT_GROUP_H */
