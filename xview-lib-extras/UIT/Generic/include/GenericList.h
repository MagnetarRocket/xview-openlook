/*H+ GenericList.h
*
*    SCCS ID: @(#)GenericList.h 1.3 92/06/05 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the GenericList class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       09/10/90   Original creation.
*    MGS       01/27/92   Make SVR4 compatible.
*
*H-*/

/*C+ GenericList
*
*    DESCRIPTION:
*
*    The GenericList class provides a way to create polymorphic lists
*    of any object sub-classed off of the Generic object class.
*    The list can have objects added to it, deleted from it, accessed
*    randomly, traversed in order, and have other lists appended to it.
*    When the list is deleted via the delete operator, delete is called
*    on each of the items in the list.
*
*    INHERITED CLASSES:
*
*    Generic - Inheriting the Generic class will allow the GenericList class
*              to be treated as a Generic object.  Therefore, a GenericList
*              can contain a GenericList.
*
*    ORDINARY DATA MEMBERS:
*
*    int       growSize     - The amount to grow the physical size
*                             when it is exceeded.
*    Generic **list         - The actual list of objects.
*    int       numItems     - The number of items in the list.
*    int       physicalSize - The current size of allocated memory holding
*			      the list.
*    int       position     - The current traversal position.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setGrowSize (int)      - Sets the grow size of the physical memory
*                                  of the list if the number of items
*				   exceeds the current physical size of the
*				   list.
*    void addItem (Generic *)    - Adds the specified item to the end of
*                                  the list.
*    Generic *removeItem (int)   - Removes and returns the specified item from
*                                  the list.
*    void removeItem (const Generic *)
*                                - Removes the item from the list that matches
*                                  the specified pointer.
*    Generic *getItem (int) const
*                                - Gets the specified item from the list.
*    int findItem (const Generic *) const
*                                - Finds the item that matches the specified
*                                  pointer and returns the position number.
*    Generic *traverse (Boolean reset_flag)
*                                - Traverses the list.  If the reset flag
*                                  is set, traversal starts at the beginning
*                                  of the list.
*    void clear ()               - Clears the list and destroys each item
*                                  in the list.
*    void dropItems ()           - Drops the items from the list without
*                                  destroying each item.
*    int getSize () const        - Returns the number of items in the list.
*    GenericList &operator= (const GenericList&)
*                                - Allows assignment of one list to another.
*    Generic* &operator[] (int) const
*                                - Allows access to the specified item in
*                                  the list.  The list entry can be read and
*                                  modified.
*    GenericList operator+ (const GenericList&)
*                                - Concatenates two lists.
*
*C-*/
#ifndef _GENERICLIST_H
#define _GENERICLIST_H

// Includes
#include <stddef.h>

#include "Boolean.h"
#include "Generic.h"

// Class Definition
class GenericList : public Generic {
 public:
  GenericList () { growSize = 10;
		   list = (Generic **) NULL;
		   numItems = 0;
		   physicalSize = 0;
		   position = 0; }
  GenericList (const GenericList& l);
  ~GenericList () { if (numItems) {
//                      for (int i=0; i < numItems; i++)
//			delete list [i];
		      delete list;
		    }
		  }
  void	       setGrowSize (int grow_size) { if (grow_size > 0)
					       growSize = grow_size;
					   }
  void         addItem (Generic *);
  Generic     *removeItem (int);
  void	       removeItem (const Generic *);
  Generic     *getItem (int) const;
  int          findItem (const Generic *) const;
  Generic     *traverse (Boolean);
  void         clear () { for (int i=0; i < numItems; i++)
			    delete list [i];
                          numItems = 0;
			  physicalSize = 0;
			  position = 0;
			  delete list;
			  list = (Generic **) NULL;
			}
  void         dropItems () { numItems = 0;
			      physicalSize = 0;
			      position = 0;
			      delete list;
			      list = (Generic **) NULL;
			    }
  int          getSize () const { return numItems; }
  GenericList &operator= (const GenericList&);
  Generic*    &operator[] (int) const;
  GenericList  operator+ (const GenericList&);
 protected:
  int       growSize;
  Generic **list;
  int       numItems;
  int       physicalSize;
  int       position;
};

#endif /* _GENERICLIST_H */
