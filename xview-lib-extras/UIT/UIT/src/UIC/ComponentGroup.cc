/*H+ ComponentGroup.cc
*
*    SCCS ID: @(#)ComponentGroup.cc 1.7 93/04/09 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the ComponentGroup class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)ComponentGroup.cc 1.7 93/04/09 SMI"; 
#endif

// Includes
#include "ComponentGroup.h"

/*F+ ComponentGroup::ComponentGroup ()
*
*    PURPOSE:
*
*    Constructor for the ComponentGroup class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ComponentGroup::ComponentGroup ()
{
  setComponentGroupDefaults ();
}

/*F+ void ComponentGroup::setComponentGroupDefaults ()
*
*    PURPOSE:
*
*    Constructor for the ComponentGroup class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setComponentGroupDefaults ()
{
  noDragDrop = TRUE;
  numRows = UIC_NOT_SET;
  numColumns = UIC_NOT_SET;
  useNumRows = FALSE;
  groupLayout = NO_LAYOUT;
  rowAlignment = TOP_EDGES;
  columnAlignment = LEFT_EDGES;
  horizontalOffset = 10;
  verticalOffset = 10;
  horizontalSpacing = 10;
  verticalSpacing = 10;
  anchorObject = (UIObject *) NULL;
  anchorPoint = NORTHWEST;
  referencePoint = NORTHWEST;
  showFlag = TRUE;
  activeFlag = TRUE;
  isComponentGroup = TRUE;
  parentDisplay = (ComponentDisplay *) NULL;
  parentGroup = (ComponentGroup *) NULL;
  newLayout = TRUE;
  newPosition = TRUE;

  xLoc = currentX = valueX = 0;
  yLoc = currentY = valueY = 0;

  groupList->addItem ((Generic *) this);
}

/*F+ ComponentGroup::~ComponentGroup ()
*
*    PURPOSE:
*
*    Destructor for the ComponentGroup class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ComponentGroup::~ComponentGroup ()
{
  if (!destroyChildren) {
    componentList.dropItems ();
  }
  else { 
    UIObject *child; 
 
    Boolean reset_flag = TRUE;
    reset_flag = TRUE;
    while (child = (UIObject *) componentList.traverse (reset_flag)) {
      reset_flag = FALSE;
      child->setDestroyChildren (TRUE);
    }
    componentList.clear (); 
  } 
}

/*F+ void ComponentGroup::setDisplayedRows (int num_rows)
*
*    PURPOSE:
*
*    Set the number of rows to display.
*
*    PARAMETERS:
*
*    int num_rows - The number of rows to display.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setDisplayedRows (int num_rows)
{
  numRows = num_rows;
  useNumRows = TRUE;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setDisplayedColumns (int num_columns)
*
*    PURPOSE:
*
*    Set the number of columns to display.
*
*    PARAMETERS:
*
*    int num_columns - The number of columns to display.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setDisplayedColumns (int num_columns)
{
  numColumns = num_columns;
  useNumRows = FALSE;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setLayout (Layout layout)
*
*    PURPOSE:
*
*    Specifies the group layout type.
*
*    PARAMETERS:
*
*    layput - The type of layout (ROW, COLUMN, ROW_AND_COLUMN, or NONE).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setLayout (Layout layout)
{
  groupLayout = layout;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setRowAlignment (RowAlignment alignment)
*
*    PURPOSE:
*
*    Specifies the group row alignment type.
*
*    PARAMETERS:
*
*    alignment - The type of row alignment.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setRowAlignment (RowAlignment alignment)
{
  rowAlignment = alignment;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setColumnAlignment (ColumnAlignment alignment)
*
*    PURPOSE:
*
*    Specifies the group column alignment type.
*
*    PARAMETERS:
*
*    alignment - The type of column alignment.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setColumnAlignment (ColumnAlignment alignment)
{
  columnAlignment = alignment;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setHorizontalOffset (int offset)
*
*    PURPOSE:
*
*    Specifies the horizontal offset of the group position.
*
*    PARAMETERS:
*
*    offset - The offset in pixels.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setHorizontalOffset (int offset)
{
  horizontalOffset = offset;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setVerticalOffset (int offset)
*
*    PURPOSE:
*
*    Specifies the vertical offset of the group position.
*
*    PARAMETERS:
*
*    offset - The offset in pixels.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setVerticalOffset (int offset)
{
  verticalOffset = offset;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setHorizontalSpacing (int spacing)
*
*    PURPOSE:
*
*    Specifies the horizontal spacing of the group position.
*
*    PARAMETERS:
*
*    spacing - The spacing in pixels.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setHorizontalSpacing (int spacing)
{
  horizontalSpacing = spacing;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setVerticalSpacing (int spacing)
*
*    PURPOSE:
*
*    Specifies the vertical spacing of the group position.
*
*    PARAMETERS:
*
*    spacing - The spacing in pixels.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setVerticalSpacing (int spacing)
{
  verticalSpacing = spacing;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setAnchor (UIObject     &object,
*                                    CompassPoint  point)
*
*    PURPOSE:
*
*    Specifies the object and compass point on the object to anchor
*    the group on.
*
*    PARAMETERS:
*
*    object - The object to anchor to.
*    point  - The compass point on the component to anchor to
*             (NORTHWEST, NORTH, NORTHEAST, SOUTHWEST, SOUTH, SOUTHEAST,
*              WEST, CENTER, or EAST).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setAnchor (UIObject &object, CompassPoint point)
{
  anchorObject = &object;
  anchorPoint = point;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ void ComponentGroup::setReferencePoint (CompassPoint point)
*
*    PURPOSE:
*
*    Specifies the compass point on the group to anchor.
*
*    PARAMETERS:
*
*    point - The compass point on the group (NORTHWEST, NORTH, NORTHEAST,
*            SOUTHWEST, SOUTH, SOUTHEAST, WEST, CENTER, or EAST).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setReferencePoint (CompassPoint point)
{
  referencePoint = point;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
}
 
/*F+ int ComponentGroup::addComponent (UIComponent &component)
*
*    PURPOSE:
*
*    Adds a component to the component group.
*
*    PARAMETERS:
*
*    component - The component to add to the group.
*
*    RETURN VALUES:
*
*    Returns the index of the component in the component group.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ComponentGroup::addComponent (UIComponent &component)
{
  componentList.addItem ((Generic *) &component);

  if (component.acceptDrops) acceptDrops = TRUE;

  if (parentDisplay)
    if (!component.getXViewObject ())
      parentDisplay->addComponent (component);

  if (component.isComponentGroup)
    ((ComponentGroup *) &component)->parentGroup = this;

  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }

  return (componentList.getSize ());
}

/*F+ void ComponentGroup::removeComponent (UIComponent &component)
*
*    PURPOSE:
*
*    Removes a component from the component group.
*
*    PARAMETERS:
*
*    component - The component to remove from the group.
*
*    RETURN VALUES:
*
*    No return values.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::removeComponent (UIComponent &component)
{
  int index = componentList.findItem ((Generic *) &component);

  if (index > -1) {
    (void) componentList.removeItem (index);

    if (parentDisplay)
      parentDisplay->removeComponent (component);

    if (xViewObjectsCreated) {
      newLayout = TRUE;
      createObject ();
    }
  }
}
 
/*F+ UIComponent *ComponentGroup::replaceComponent (int          index,
*						    UIComponent &component)
*
*    PURPOSE:
*
*    Replaces a component in the group with a new component.
*
*    PARAMETERS:
*
*    index     - The index of the component to replace in the group.
*    component - The new component to put in the group.
*
*    RETURN VALUES:
*
*    Returns a pointer to the component that was replaced.  Returns NULL
*    if the old component was not found.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
UIComponent *ComponentGroup::replaceComponent (int          index,
					       UIComponent &component)
{
  int num_components = componentList.getSize ();

  if ((index < 1) || (index > num_components))
    return ((UIComponent *) NULL);

  UIComponent *oldComponent = (UIComponent *) componentList [index-1];
  componentList [index-1] = (Generic *) &component;
 
  if (parentDisplay)
    if (!component.getXViewObject ())
      parentDisplay->addComponent (component);
 
  if (component.isComponentGroup)
    ((ComponentGroup *) &component)->parentGroup = this;
 
  if (xViewObjectsCreated) {
    newLayout = TRUE;
    createObject ();
  }
 
  return (oldComponent);
}

/*F+ UIComponent *ComponentGroup::replaceComponent (UIComponent &oldComponent,
*                                                   UIComponent &component)
* 
*    PURPOSE: 
* 
*    Replaces a component in the group with a new component.    
* 
*    PARAMETERS: 
* 
*    oldComponent - The component to replace in the group. 
*    component    - The new component to put in the group. 
* 
*    RETURN VALUES: 
* 
*    Returns a pointer to the component that was replaced.  Returns NULL
*    if the old component was not found.
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------
* 
*F-*/ 
UIComponent *ComponentGroup::replaceComponent (UIComponent &oldComponent,
                                               UIComponent &component)
{ 
  int num_components = componentList.getSize (); 
  int index = 0;

  for (int i=0; i < num_components; i++) {
    if (componentList [i] == (Generic *) &oldComponent) {
      index = i;
      break;
    }
  } 

  if (index == 0) 
    return ((UIComponent *) NULL);  
 
  componentList [index] = (Generic *) &component;
  
  if (parentDisplay)
    if (!component.getXViewObject ())
      parentDisplay->addComponent (component);
  
  if (component.isComponentGroup) 
    ((ComponentGroup *) &component)->parentGroup = this; 

  if (oldComponent.isComponentGroup) {
    ((ComponentGroup *)
     &oldComponent)->parentDisplay = (ComponentDisplay *) NULL; 
    ((ComponentGroup *) &oldComponent)->parentGroup = (ComponentGroup *) NULL; 
  }
  
  if (xViewObjectsCreated) { 
    newLayout = TRUE;
    createObject ();
  }
 
  return (&oldComponent);
}
 
/*F+ int ComponentGroup::getNumberOfComponents ()
*
*    PURPOSE:
*
*    Returns the number of components in the group.
*
*    RETURN VALUES:
*
*    Returns the number of components in the group.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ComponentGroup::getNumberOfComponents ()
{
  return (componentList.getSize ());
}
 
/*F+ UIComponent *ComponentGroup::getComponent (int index)
*
*    PURPOSE:
*
*    Gets the specified component from the group.
*
*    PARAMETERS:
*
*    index - The index of the component to get.
*
*    RETURN VALUES:
*
*    Returns a pointer to the specified component.  Returns
*    NULL if the index is not valid.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIComponent *ComponentGroup::getComponent (int index)
{
  if ((index < 1) || (index > componentList.getSize ()))
    return ((UIComponent *) NULL);

  return ((UIComponent *) componentList [index-1]);
}

/*F+ void ComponentGroup::setLocation (int x, int y)
*
*    PURPOSE:
*
*    Specifies the absolute position of the group.
*
*    PARAMETERS:
*
*    x - The X coordinate of the group position.
*    y - The Y coordinate of the group position.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setLocation (int x, int y)
{
  xLoc = x;
  yLoc = y;

  if (xViewObjectsCreated) {
    newPosition = TRUE;
    createObject ();
  }
}

/*F+ void ComponentGroup::setX (int x)
*
*    PURPOSE:
*
*    Specifies the X coordinate of the absolute position of the group.
*
*    PARAMETERS:
*
*    x - The X coordinate of the group position.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setX (int x)
{
  xLoc = x;
 
  if (xViewObjectsCreated) {
    newPosition = TRUE;
    createObject ();
  }
}

/*F+ void ComponentGroup::setY (int y)
* 
*    PURPOSE: 
* 
*    Specifies the Y coordinate of the absolute position of the group. 
* 
*    PARAMETERS: 
*
*    y - The Y coordinate of the group position. 
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void ComponentGroup::setY (int y)
{ 
  yLoc = y; 
  
  if (xViewObjectsCreated) { 
    newPosition = TRUE; 
    createObject (); 
  } 
} 

/*F+ void ComponentGroup::setValueX (int x)
*
*    PURPOSE:
*
*    Specifies the X coordinate of the absolute position of the group
*    relative to where the firtst group member's value is positioned.
*
*    PARAMETERS:
*
*    x - The X coordinate.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setValueX (int x)
{
  xLoc = currentX + x - valueX;
  valueX = x;

  if (xViewObjectsCreated) {
    newPosition = TRUE;
    createObject ();
  }
}

/*F+ void ComponentGroup::show (Boolean flag)
*
*    PURPOSE:
*
*    Specifies whether or not the group is to shown.
*
*    PARAMETERS:
*
*    flag - TRUE if the group is to be shown, FALSE if not.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::show (Boolean flag)
{
  showFlag = flag;

  if (xViewObjectsCreated) {
    int num_components = componentList.getSize ();

    for (int i=0; i < num_components; i++)
      ((UIComponent *) componentList [i])->show (flag);
  }
}

/*F+ void ComponentGroup::setActive (Boolean flag)
*
*    PURPOSE:
*
*    Specifies whether or not the group is active.
*
*    PARAMETERS:
*
*    flag - TRUE if the group is active, FALSE if not.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::setActive (Boolean flag)
{
  activeFlag = flag;
 
  if (xViewObjectsCreated) {
    int num_components = componentList.getSize ();
 
    for (int i=0; i < num_components; i++)
      ((UIComponent *) componentList [i])->setActive (flag);
  }
}

/*F+ void ComponentGroup::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Instantiates the XView graphics object.
*
*    PARAMETERS:
*
*    UIObject *parent - The parent of the ComponentGroup object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::createObject (UIObject *parent)
{
  /* If a parent was specified, then the group was added to
     a ComponentDisplay and is being created as a UIComponent */
  if (parent) {
    parentDisplay = (ComponentDisplay *) parent;

    int num_components = componentList.getSize ();
    for (int i=0; i < num_components; i++)
      if (!((UIComponent *) componentList [i])->getXViewObject ())
        parentDisplay->addComponent (*((UIComponent *) componentList [i]));
  }
  /* Else, the group is being created from the master group list in UIObject
     or a member function was called after the group was already created from
     the master group list... */
  else {
    // If a new layout is needed...
    if (newLayout) {
      determineGroupLayout ();
      newLayout = FALSE;
    }

    // If a new position calculation is needed...
    if (newPosition) {
      determineGroupPosition ();
      newPosition = FALSE;
    }
  }

  UIObject::setUIObjectAttributes (parent);
}

/*F+ void ComponentGroup::determineGroupLayout ()
*
*    PURPOSE:
*
*    Determines the physical layout of the group.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::determineGroupLayout ()
{
  int num_components = componentList.getSize ();
  int min_x = ((UIComponent *) componentList [0])->getX ();
  int min_y = ((UIComponent *) componentList [0])->getY ();

  if (num_components == 0)
    return;

  UIComponent *component;
  switch (groupLayout) {
    case NO_LAYOUT:
      int component_x;
      int component_y;

      int i;
      for (i=0; i < num_components; i++) {
	component = (UIComponent *) componentList [i];

        if (component->isComponentGroup)
	  ((ComponentGroup *) component)->determineGroupAnchor ();
	
        component_x = component->getX ();
        component_y = component->getY ();
	
        if (component_x < min_x) min_x = component_x;
        if (component_y < min_y) min_y = component_y;
      }

      currentX = min_x;
      currentY = min_y;

      break;

    case ROW_LAYOUT:
      determineRowLayout ();
      break;

    case COLUMN_LAYOUT:
      determineColumnLayout ();
      break;

    case ROW_AND_COLUMN_LAYOUT:
      determineRowAndColumnLayout ();
      break;

    default:
      break;
  }

  component = (UIComponent *) componentList [0];
  valueX = component->getValueX ();
  valueY = component->getValueY ();

  // If this group has a parent group...
  if (parentGroup)
    parentGroup->determineGroupLayout ();
  // Else, if this group is anchored...
  else if (anchorObject)
    determineGroupAnchor ();
}

/*F+ void ComponentGroup::determineGroupAnchor ()
*
*    PURPOSE:
*
*    Determines where the group is anchored if it has an anchor.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::determineGroupAnchor ()
{
  if (!anchorObject)
    return;

  int anchorX,    anchorY;
  int referenceX, referenceY;

  getCompassPointLocation (anchorObject, anchorPoint, anchorX, anchorY);
  getCompassPointLocation (this, referencePoint, referenceX, referenceY);

  xLoc = anchorX + (currentX - referenceX) + horizontalOffset; 
  yLoc = anchorY + (currentY - referenceY) + verticalOffset; 

  determineGroupPosition ();
}

/*F+ void ComponentGroup::determineGroupPosition ()
* 
*    PURPOSE: 
* 
*    Determines the physical position of the group.
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void ComponentGroup::determineGroupPosition ()
{
  int num_components = componentList.getSize ();

  if (num_components == 0)
    return;

  UIComponent *component;
  for (int i=0; i < num_components; i++) {
    component = (UIComponent *) componentList [i];

    component->setX (xLoc + component->getX () - currentX);
    component->setY (yLoc + component->getY () - currentY);

    if (!xViewObjectsCreated && component->isComponentGroup)
      ((ComponentGroup *) component)->determineGroupPosition ();
  }

  component = (UIComponent *) componentList [0];
  valueX = component->getValueX ();
  valueY = component->getValueY ();

  currentX = xLoc;
  currentY = yLoc;
}

/*F+ void ComponentGroup::determineRowLayout ()
*
*    PURPOSE:
*
*    Determines the layout of the group if the layout type is ROW_LAYOUT.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ComponentGroup::determineRowLayout ()
{
  int          base_y;
  UIComponent *base_component = (UIComponent *) componentList [0];
  
  switch (rowAlignment) {
    case TOP_EDGES:
      base_y = base_component->getY ();
      break;

    case HORIZONTAL_CENTERS:
      base_y = base_component->getY () + base_component->getHeight () / 2;
      break;

    case BOTTOM_EDGES:
      base_y = base_component->getY () + base_component->getHeight ();
      break;

    default:
      base_y = base_component->getY ();
      break;
  }

  int 	       max_height = base_component->getHeight ();
  int	       total_width = base_component->getWidth ();
  int	       component_y;
  int          component_height;
  int	       min_y = base_component->getY ();
  UIComponent *previous_component = base_component;
  UIComponent *current_component;

  int num_components = componentList.getSize ();
  for (int i=1; i < num_components; i++) {
    current_component = (UIComponent *) componentList [i];
    component_height = current_component->getHeight ();

    switch (rowAlignment) {
      case TOP_EDGES:
	component_y = base_y;
	break;

      case HORIZONTAL_CENTERS:
	component_y = base_y - component_height / 2;
	break;

      case BOTTOM_EDGES:
	component_y = base_y - component_height;
	break;

      default:
	component_y = base_y;
	break;
    }

    current_component->setX (previous_component->getX () +
			     previous_component->getWidth () +
			     horizontalSpacing);
    current_component->setY (component_y);

    if (component_y < min_y) min_y = component_y;

    if (component_height > max_height) max_height = component_height;    
    total_width += (current_component->getWidth () + horizontalSpacing);
 
    previous_component = current_component;
  }

  currentX = base_component->getX ();
  currentY = min_y;

  height = max_height;
  width = total_width;
}

/*F+ void ComponentGroup::determineColumnLayout () 
* 
*    PURPOSE: 
* 
*    Determines the layout of the group if the layout type is COLUMN_LAYOUT. 
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void ComponentGroup::determineColumnLayout () 
{ 
  int          base_x;
  UIComponent *base_component = (UIComponent *) componentList [0];
 
 
  switch (columnAlignment) {
    case LEFT_EDGES:
      base_x = base_component->getX ();
      break;
 
    case VERTICAL_CENTERS:
      base_x = base_component->getX () + base_component->getWidth () / 2;
      break;
 
    case RIGHT_EDGES:
      base_x = base_component->getX () + base_component->getWidth ();
      break;

    case LABELS:
      base_x = base_component->getValueX ();
      break;
 
    default:
      base_x = base_component->getX ();
      break;
  }
 
  int          max_width = base_component->getWidth ();
  int          total_height = base_component->getHeight ();
  int	       max_label_width = base_component->getValueX () -
				 base_component->getX ();
  int	       max_value_width = base_component->getWidth () - max_label_width;
  int          component_x;
  int          component_width;
  int          component_label_width;
  int          component_value_width;
  int	       min_x = base_component->getX ();
  UIComponent *previous_component = base_component;
  UIComponent *current_component;

  int num_components = componentList.getSize ();
  for (int i=1; i < num_components; i++) {
    current_component = (UIComponent *) componentList [i];
    component_width = current_component->getWidth ();

    switch (columnAlignment) { 
      case LEFT_EDGES:
        component_x = base_x; 
        break; 
 
      case VERTICAL_CENTERS:
        component_x = base_x - component_width / 2;
        break;
 
      case BOTTOM_EDGES: 
        component_x = base_x - component_width;
        break;
 
      case LABELS:
        component_x = base_x; 
        break; 
 
      default: 
        component_x = base_x;
        break;
    }   
 
    if (columnAlignment == LABELS)
      current_component->setValueX (component_x);
    else
      current_component->setX (component_x);

    current_component->setY (previous_component->getY () +
                             previous_component->getHeight () +
                             verticalSpacing);
 
    if (columnAlignment != LABELS)
      if (component_x < min_x)
        min_x = component_x;

    if (columnAlignment == LABELS) {
      component_label_width = current_component->getValueX () -
			      current_component->getX (); 
      component_value_width = current_component->getWidth () -
			      component_label_width;

      if (component_label_width > max_label_width)
        max_label_width = component_label_width;
      if (component_value_width > max_value_width)
        max_value_width = component_value_width;
    }
    else
      if (component_width > max_width) max_width = component_width;    

    total_height += (current_component->getHeight () + verticalSpacing);
 
    previous_component = current_component;
  }

  currentX = ((columnAlignment != LABELS) ? min_x :
				            component_x - max_label_width);
  currentY = base_component->getY ();
 
  height = total_height;

  if (columnAlignment == LABELS)
    width = max_label_width + max_value_width;
  else
    width = max_width;
} 
 
/*F+ void ComponentGroup::determineRowAndColumnLayout () 
* 
*    PURPOSE: 
* 
*    Determines the layout of the group if the layout type is
*    ROW_AND_COLUMN_LAYOUT. 
* 
*    REVISION HISTORY: 
* 
*    Who       When       Why 
*    --------- ---------- ------------------------ 
* 
*F-*/ 
void ComponentGroup::determineRowAndColumnLayout () 
{ 
  int num_components = componentList.getSize ();

  if (useNumRows) {
    if (numRows < 1) numRows = 1;
    if (numRows > num_components) numRows = num_components;

    numColumns = num_components / numRows;
    if (num_components % numRows) numColumns++;
  }
  else {
    if (numColumns < 1) numColumns = 1;
    if (numColumns > num_components) numColumns = num_components;

    numRows = num_components / numColumns;
    if (num_components % numColumns) numRows++;
  }

  int max_width = 0;
  int max_height = 0;

  int component_width;
  int component_height;

  UIComponent *component;

  if (columnAlignment == LABELS) {
    int label_width;
    int value_width;

    int max_label_width = 0;
    int max_value_width = 0;

    for (int i=0; i < num_components; i++) {
      component = (UIComponent *) componentList [i];

      label_width = component->getValueX () - component->getX ();
      value_width = component->getWidth () - label_width;

      if (label_width > max_label_width) max_label_width = label_width;
      if (value_width > max_value_width) max_value_width = value_width;

      if ((component_height = component->getHeight ()) > max_height)
        max_height = component_height;
    }

    max_width = max_label_width + max_value_width;
  }
  else {
    for (int i=0; i < num_components; i++) {
      component = (UIComponent *) componentList [i];

      if ((component_width = component->getWidth ()) > max_width)
        max_width = component_width;
      if ((component_height = component->getHeight ()) > max_height)
        max_height = component_height;
    }
  }

  int value_x = 0;

  if (columnAlignment == LABELS)
    value_x = calculateColumnValueX (0);

  int cell_x;
  int cell_y;

  int component_x;
  int component_y;

  int min_x = 0;
  int min_y = 0;

  int current_row = 0;
  int current_column = 0;

  for (int i=0; i < num_components; i++) {
    component = (UIComponent *) componentList [i];

    cell_x = currentX + current_column * (max_width + horizontalSpacing);
    cell_y = currentY + current_row * (max_height + verticalSpacing);

    switch (columnAlignment) {
      case LEFT_EDGES:
	component_x = cell_x;
	break;

      case VERTICAL_CENTERS:
	component_x = cell_x + max_width / 2 - component->getWidth () / 2;
	break;

      case RIGHT_EDGES:
	component_x = cell_x + max_width - component->getWidth ();
	break;

      case LABELS:
	component_x = cell_x +
		      component->getX () + value_x - component->getValueX ();
	break;

      default:
	component_x = cell_x;
	break;
    }

    switch (rowAlignment) {
      case TOP_EDGES:
	component_y = cell_y;
	break;

      case HORIZONTAL_CENTERS:
	component_y = cell_y + max_height / 2 - component->getHeight () / 2;
	break;

      case BOTTOM_EDGES:
	component_y = cell_y + max_height - component->getHeight ();
	break;

      default:
	component_y = cell_y;
	break;
    }

    component->setX (component_x);
    component->setY (component_y);

    if (component_x < min_x) min_x = component_x;
    if (component_y < min_y) min_y = component_y;

    if (useNumRows) {
      if (++current_column >= numColumns) {
	current_column = 0;
	current_row++;
      }
    }
    else {
      current_column++;
      current_row = 0;
    }

    if ((columnAlignment == LABELS) && (i+1 < num_components))
      value_x = calculateColumnValueX (current_column);
  }

  currentX = min_x;
  currentY = min_y;

  width  = numColumns * max_width + (numColumns - 1) * horizontalSpacing;
  height = numRows * max_height + (numRows - 1) * verticalSpacing;
} 

/*F+ int ComponentGroup::calculateColumnValueX (int column)
*
*    PURPOSE:
*
*    Determines the maximum value X offset relative to beginning of
*    the specified coulmn.
*
*    PARAMETERS:
*
*    column - The column (0 to numRows-1) to find the offset for.
*
*    RETURN VALUES:
*
*    Returns the value of the X value offset.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
int  ComponentGroup::calculateColumnValueX (int column)
{
  int start;
  int increment;
 
  if (useNumRows) {
    start = column;
    increment = numColumns;
  }
  else {
    start = column * numRows;
    increment = 1;
  }
 
  int num_components = componentList.getSize ();

  int value_x;
  int max_value_x = 0;
 
  UIComponent *component;
 
  for (int i=start, j=0;
       (i < num_components) && (j < numRows);
       i += increment, j++) {
    component = (UIComponent *) componentList [i];
 
    value_x = component->getValueX () - component->getX ();
    if (value_x > max_value_x) max_value_x = value_x;
  }
 
  return (max_value_x);
}
 
#ifdef TEST
#endif /* TEST */
