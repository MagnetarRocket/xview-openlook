/*H+ DropTarget.cc
*
*    SCCS ID: @(#)DropTarget.cc 1.5 92/05/23 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the DropTarget class.
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
static char sccsid [] = "@(#)DropTarget.cc 1.5 92/05/23 SMI"; 
#endif

// Includes
#include "DropTarget.h"

/*F+ DropTarget::DropTarget ()
*
*    PURPOSE:
*
*    Constructor for the DropTarget class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
DropTarget::DropTarget ()
{
  setDropTargetDefaults ();
}

/*F+ DropTarget::DropTarget (char *string_label)
*
*    PURPOSE:
*
*    Constructor for the DropTarget class.
*
*    PARAMETERS:
*
*    char *string_label - String label for the input field.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
DropTarget::DropTarget (char *string_label) : UIComponent (string_label)
{
  setDropTargetDefaults ();
}

/*F+ DropTarget::DropTarget (Image &image_label)
*
*    PURPOSE:
*
*    Constructor for the DropTarget class.
*
*    PARAMETERS:
*
*    Image &image_label - Image label for the input field.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
DropTarget::DropTarget (Image &image_label) : UIComponent (image_label)
{
  setDropTargetDefaults ();
}

/*F+ void DropTarget::setDropTargetDefaults ()
*
*    PURPOSE:
*
*    Constructor for the DropTarget class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void DropTarget::setDropTargetDefaults ()
{
  icon = (Image *) NULL;
  newIcon = FALSE;
  busyIcon = (Image *) NULL;
  newBusyIcon = FALSE;
  acceptDrops = TRUE;
  isDropTarget = TRUE;
}

/*F+ DropTarget::~DropTarget ()
*
*    PURPOSE:
*
*    Destructor for the DropTarget class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
DropTarget::~DropTarget ()
{
  if (newIcon) delete icon;
  if (newBusyIcon) delete busyIcon;
}

/*F+ void DropTarget::setDraggable (Boolean flag)
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
void DropTarget::setDraggable (Boolean flag)
{
  draggable = flag;

  if (owObject && draggable)
    createDragSite ();

  if (owObject)
    xv_set (owObject, PANEL_DROP_FULL, draggable, NULL);
}

/*F+ void DropTarget::setAcceptDrops (Boolean flag)
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
void DropTarget::setAcceptDrops (Boolean flag)
{
  acceptDrops = flag;
  setActive (acceptDrops);
}

/*F+ int DropTarget::notifyProc (Panel_item item, int value, Event *event)
*
*    PURPOSE:
*
*    XView drop target notify proc.  This member function is protected.
*
*    PARAMETERS:
*
*    item  - The XView panel item that the notify occurred on.
*    value - The result of the drop.
*    event - The XView event info structure.
*
*    RETURNS VALUES:
*
*    The user specified notify result is returned.  XV_OK is returned by
*    default.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int DropTarget::notifyProc (Panel_item item, int value, Event *event)
{
  DropTarget *object = (DropTarget *)
                       xv_get (item, XV_KEY_DATA, UIC_OBJECT_KEY);
  
  /* Set the internal UIT event information */
  object->setEventInformation (event);

  switch (event_action (event)) {
  case ACTION_SELECT:
  case LOC_DRAG:
    object->dragResult = value;
    object->dragStatus = DRAG_END;

    /* Call the user specified drag handler */
    if (object->useDefaultDragHandlerFlag)
      object->defaultDragHandler ();
    else if (object->dragHandler)
      (*object->dragHandler) (object);

    object->dragStatus = DRAG_NO_DRAG;
    break;

  case ACTION_DRAG_COPY:
  case ACTION_DRAG_MOVE:
    object->dropSiteSelection = xv_get (object->owObject,
					PANEL_DROP_SEL_REQ,
					NULL);
    object->getInternalDropData (object->dropSiteSelection);

    /* Call the user specified drop handler */
    if (object->useDefaultDropHandlerFlag)
      object->defaultDropHandler ();
    else if (object->dropHandler)
      (*object->dropHandler) (object);

    /* Activate all of the drop targets */
    int num_drop_targets;
    num_drop_targets = object->dropTargetList->getSize ();

    int i;
    for (i=0; i < num_drop_targets; i++)
      xv_set (((UIObject *) (*object->dropTargetList) [i])->getXViewObject (),
	      PANEL_BUSY, FALSE,
	      NULL);

    break;
  
  default:
    break;
  }

  /* Notify any notify targets */
  object->notifyNotifyTargets ();

  /* Call the user specified notify handler */
  if (object->useDefaultNotifyHandlerFlag)
    object->defaultNotifyHandler ();
  else if (object->notifyHandler)
    (*object->notifyHandler) (object);

  /* Erase internal UIT event information */
  object->resetEventInformation ();
  object->dropSiteSelection = (Selection_requestor) NULL;

  // If the user specified a notify status
  if (object->notifyStatus != UIC_NOT_SET) {
    // Set the notify status
    xv_set (item, PANEL_NOTIFY_STATUS, object->notifyStatus, NULL);

    // Reset the internal notify status
    object->notifyStatus = UIC_NOT_SET;
  }

  // If the user specified a notify result...
  if (object->notifyResult != UIC_NOT_SET) {
    int result = object->notifyResult;
    object->notifyResult = UIC_NOT_SET;

    // Return the user specified result
    return (result);
  }

  return (XV_OK);
}

/*F+ void DropTarget::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Instantiates the XView graphics object.
*
*    PARAMETERS:
*
*    UIObject *parent - The parent of the DropTarget object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void DropTarget::createObject (UIObject *parent)
{
  // Create the XView object, setting the fonts during the create
  createWithFonts (parent, PANEL_DROP_TARGET, NULL);

  xv_set (owObject, PANEL_NOTIFY_PROC, &DropTarget::notifyProc, NULL);

  if (icon) {
    ((UIObject *) icon)->createObject (parent);
    xv_set (owObject, PANEL_DROP_GLYPH, icon->getXViewObject (), NULL);
  }
 
  if (busyIcon) {
    ((UIObject *) busyIcon)->createObject (parent);
    xv_set (owObject, PANEL_DROP_BUSY_GLYPH, busyIcon->getXViewObject (), NULL);
  }

  if (defaultDropSite)
    xv_set (owObject, PANEL_DROP_SITE_DEFAULT, TRUE, NULL);

  active = acceptDrops;

  dropTargetList->addItem ((Generic *) this);

  // Set common XView attributes
  setUIComponentAttributes (parent);
}

/*F+ void DropTarget::createDragSite ()
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
void DropTarget::createDragSite ()
{
  if (!dragSite) {
    createXViewDragSite (parentObject->getXViewObject ());
    xv_set (owObject,
	    PANEL_DROP_DND,  dragSite,
	    PANEL_DROP_FULL, TRUE,
	    NULL);
  }
}

/*F+ void DropTarget::createDropSite ()
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
void DropTarget::createDropSite ()
{
}

#ifdef TEST
#endif /* TEST */
