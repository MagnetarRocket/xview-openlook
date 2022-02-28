/*H+ CSTree.h
*
*    SCCS ID: @(#)CSTree.h 1.1 93/02/08 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSTree class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSTree
*
*    DESCRIPTION:
*
*    The CSTree component is a collection object that positions its
*    children like tiles in an array of tiles.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSTree object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setBorder (int new_border) - Sepcifies the space in pixels
*    that is reserved around each node.
*
*    void setLayout (Orientation new_layout) - specifies the layout direction
*    of the tree:
*
*         VERTICAL
*         HORIZONTAL
*
*    void setParentDistance (int distance) - Specifies the distance
*    between nodes and their leaves.
*
*    void          addLink (CSComponent &, CSComponent &) - Creates a
*    link between the two objects.
*
*    void          deleteLink (CSComponent &) - Removes the specified
*    object from the tree.  Can only be called after XView objects have
*    been created.
*
*    CSComponent *getFromLink (CSComponent &) - Returns the node that
*    points to the specified object.
*
*    GenericList *getToLinkList (CSComponent &) - Returns a GenericList
*    of the nodes the specified object points to.  The caller must
*    free the array after dropping the items in the list:
*
*         list->dropItems ();
*         delete list;
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    ARRAY_TILE encapsulated by this object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSTreeDefaults () - Initializes data members.
*
*/

#ifndef _CS_TREE_H
#define _CS_TREE_H

// Includes
#include "CSComponent.h"

#include <sspkg/tree.h>

class TreeLink : public Generic {
public:
  TreeLink () { from = (CSComponent *) NULL;
	        to = (CSComponent *) NULL; }
  ~TreeLink () {}

  CSComponent *from;
  CSComponent *to;
};

// Class Definition
class CSTree : public CSComponent {
public:
  CSTree ();
  ~CSTree ();

  void setBorder (int new_border)
    {
      border = new_border;

      if (owObject)
	xv_set (owObject, TREE_BORDER, border, NULL);
    }

  void setLayout (Orientation new_layout)
    {
      layout = new_layout;

      if (owObject) {
	if (layout == VERTICAL)
	  xv_set (owObject,
		  TREE_LAYOUT, TREE_LAYOUT_VERTICAL,
		  NULL);
	else if (layout == HORIZONTAL)
	  xv_set (owObject,
		  TREE_LAYOUT, TREE_LAYOUT_HORIZONTAL,
		  NULL);
      }
    }

  void setParentDistance (int distance)
    {
      parentDistance = distance;

      if (owObject)
	xv_set (owObject, TREE_PARENT_DISTANCE, parentDistance, NULL);
    }

  void          addLink (CSComponent &, CSComponent &);

  void          deleteLink (CSComponent &child)
    {
      if (owObject)
	xv_set (owObject, TREE_UNLINK, child.getXViewObject (), NULL);
    }

  CSComponent *getFromLink (CSComponent &);
  GenericList *getToLinkList (CSComponent &);

protected:
  int         border;
  Orientation layout;
  int         parentDistance;
  GenericList links;

  virtual void createObject (UIObject *parent);

private:    
  void setCSTreeDefaults ();
};

#endif /* _CS_TREE_H */
