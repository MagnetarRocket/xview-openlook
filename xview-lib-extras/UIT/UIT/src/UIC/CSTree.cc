/*H+ CSTree.cc
*
*    SCCS ID: @(#)CSTree.cc 1.2 93/04/09 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSTree class.
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
static char sccsid [] = "@(#)CSTree.cc 1.2 93/04/09 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSTree.h"

/*F+ CSTree::CSTree ()
*
*    PURPOSE:
*
*    Constructor of the CSTree class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSTree::CSTree ()
{
  setCSTreeDefaults ();
}

/*F+ CSTree::setCSTreeDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSTree class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSTree::setCSTreeDefaults ()
{
  border = UIC_NOT_SET;
  layout = (Orientation) UIC_NOT_SET;
  parentDistance = UIC_NOT_SET;
}

/*F+ CSTree::~CSTree ()
*
*    PURPOSE:
*
*    Destructor of the CSTree class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSTree::~CSTree ()
{
}

/*F+ void CSTree::addLink (CSComponent &from_child,
*                          CSComponent &to_child)
*
*    PURPOSE:
*
*    Creates a link between the two specified objects.
*
*    PARAMETERS:
*
*    from_child - The object the link is from
*    to_child -  The object the link is to
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSTree::addLink (CSComponent &from_child,
                      CSComponent &to_child)
{
  if (&to_child == this) return;

  if (owObject) {
    if (&from_child != this)
      if (!from_child.getXViewObject ()) addChild (from_child);

    if (!to_child.getXViewObject ()) addChild (to_child);

    xv_set (owObject,
	    TREE_ADD_LINK, from_child.getXViewObject (),
	                   to_child.getXViewObject (),
	    NULL);
  }
  else {
    if (&from_child != this)
      if (children.findItem ((Generic *) &from_child) < 0)
        addChild (from_child);

    if (children.findItem ((Generic *) &to_child) < 0)
      addChild (to_child);

    Boolean   found = FALSE;
    TreeLink *link;
    int       num_links = links.getSize ();

    for (int i=0; i < num_links; i++) {
      TreeLink *link = (TreeLink *) links [i];
      
      if ((link->from == &from_child) && (link->to == &to_child)) {
	found = TRUE;
	break;
      }
    }
    
    if (!found) {
      link = new TreeLink;
      link->from = &from_child;
      link->to = &to_child;

      links.addItem ((Generic *) link);
    }
  }
}

/*F+ CSComponent *CSTree::getFromLink (CSComponent &to_child)
*
*    PURPOSE:
*
*    Get the node that forms the link to the specified child.
*
*    PARAMETERS:
*
*    to_child -  The object the link is to
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSComponent *CSTree::getFromLink (CSComponent &to_child)
{
  CSComponent *from_child = (CSComponent *) NULL;

  if (owObject) {
    UICGeneric object = xv_get (owObject,
				TREE_LINK_FROM,
				to_child.getXViewObject ());

    from_child = (CSComponent *) xv_get (object, XV_KEY_DATA, UIC_OBJECT_KEY);
  }
  else {
    Boolean   found = FALSE;
    TreeLink *link = (TreeLink *) NULL;
    int       num_links = links.getSize ();

    for (int i=0; i < num_links; i++) {
      link = (TreeLink *) links [i];
      
      if (link->to == &to_child) {
	found = TRUE;
	break;
      }
    }
    
    if (found)
      from_child = link->from;
  }

  return from_child;
}

/*F+ GenericList *CSTree::getToLinkList (CSComponent &from_child)
*
*    PURPOSE:
*
*    Get all of the nodes from the specified child.
*
*    PARAMETERS:
*
*    from_child -  The object the link is to
*
*    RETURN VALUES:
*
*    Returns a pointer to a GenericList of the nodes.  The GenericList
*    must be freed by the caller in the following way:
*
*         list->dropItems ();
*         delete list;
*
*    Returns NULL if there are no nodes.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
GenericList *CSTree::getToLinkList (CSComponent &from_child)
{
  GenericList *to_nodes = (GenericList *) NULL;

  if (owObject) {
    Rectobj_list *nodes = (Rectobj_list *)
			  xv_get (owObject,
				  TREE_LINK_TO_LIST,
				  from_child.getXViewObject ());

    if (nodes) {
      to_nodes = new GenericList;

      list_for (nodes) {
	Rectobj handle = RECTOBJ_LIST_HANDLE (nodes);
	to_nodes->addItem ((Generic *) xv_get (handle,
					       XV_KEY_DATA,
					       UIC_OBJECT_KEY));
      }
    }
  }
  else {
    int num_links = links.getSize ();
    for (int i=0; i < num_links; i++) {
      TreeLink *link = (TreeLink *) links [i];
      
      if (link->from == &from_child) {
	if (!to_nodes)
	  to_nodes = new GenericList;

	to_nodes->addItem ((Generic *) link->to);
      }
    }
  }

  return to_nodes;
}

/*F+ void CSTree::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSTree object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSTree::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), TREE, NULL);

  if (border != UIC_NOT_SET)
    xv_set (owObject, TREE_BORDER, border, NULL);

  if (layout == VERTICAL)
    xv_set (owObject,
	    TREE_LAYOUT, TREE_LAYOUT_VERTICAL,
	    NULL);
  else if (layout == HORIZONTAL)
    xv_set (owObject,
	    TREE_LAYOUT, TREE_LAYOUT_HORIZONTAL,
	    NULL);

  if (parentDistance != UIC_NOT_SET)
    xv_set (owObject, TREE_PARENT_DISTANCE, parentDistance, NULL);

  setCSComponentAttributes (parent);

  int num_links = links.getSize ();
  for (int i=0; i < num_links; i++) {
    TreeLink *link = (TreeLink *) links [i];

    if (link->from->getXViewObject () && link->to->getXViewObject ()) {
      xv_set (owObject,
	      TREE_ADD_LINK, link->from->getXViewObject (),
	                     link->to->getXViewObject (),
	      NULL);
    }
  }
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
