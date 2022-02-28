/*H+ GenericList.cc
*
*    SCCS ID: @(#)GenericList.cc 1.5 93/02/08 SMI 
*
*    Copyright (c) 1990-1992 by Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the GenericList class.
*
*    GLOBAL ROUTINES:
*
*    GenericList::GenericList (const GenericList&)
*					      - Copy constructor.  Allows
*						initialization from another
*						another GenericList object.
*    void GenericList::addItem (Generic *)
*                                             - Adds the specified item to the
*                                               end of the list.
*    Generic *GenericList::removeItem (int)   - Removes and returns the
*                                               specified item from the list.
*    void GenericList::removeItem (const Generic *)
*                                             - Removes the specified item
*                                               from the list.
*    int GenericList::findItem (const Generic *) const
*                                             - Returns the position of the
*                                               specified item in the list.
*    Generic *GenericList::traverse (Boolean)
*                                             - Traverses the list.  If the
*                                               reset flag is set, traversal
*                                               starts at the beginning of
*                                               the list.
*    GenericList &GenericList::operator= (const GenericList&)
*                                             - Allows assignment of one list
*                                               to another.
*    Generic* &GenericList::operator[] (int) const
*                                             - Allows access to the specified
*                                               item in list.  The list entry
*                                               can be read and modified.
*    GenericList GenericList::operator+ (const GenericList&)
*                                             - Concatenates two lists.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       09/10/90   Original creation.
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)GenericList.cc 1.5 93/02/08 SMI"; 
#endif

// Includes
#include <stdio.h>
#include <stdlib.h>

#include "GenericList.h"

/*F+ GenericList::GenericList (const GenericList &l)
*
*    PURPOSE:
*
*    Allow creation of a new GenericList from an existing GenericList
*    object.
*
*    PARAMETERS:
*
*    l - The existing GenericList.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*    MGS       01/27/92   Make SVR4 compatible.
*
*F-*/
GenericList::GenericList (const GenericList& l)
{
  growSize = l.growSize;
  numItems = l.numItems;
  physicalSize = l.physicalSize;
  position = 0;
  list = new Generic* [physicalSize];
  for (int i = 0; i < numItems; i++)
    list [i] = l.list [i];
}

/*F+ void GenericList::addItem (Generic *item)
*
*    PURPOSE:
*
*    Add a pointer to an object to the list of objects.
*
*    PARAMETERS:
*
*    item - The pointer to the object to add to the list.
*
*    PSEUDO CODE:
*
*    Increment the item counter
*    If the number of items > the physical size of memory
*      Allocate enough memory to hold the list plus the grow size
*      Copy the list into the new memory
*      Delete the old list
*      Set the list pointer to the new memory
*    Put the new item at the end of the new memory
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
void GenericList::addItem (Generic *item)
{
  Generic **new_list = list;

  numItems++;
  if (numItems > physicalSize) {
    physicalSize += growSize;
    new_list = new Generic* [physicalSize];

    for (int i=0; i < numItems-1; i++)
      new_list [i] = list [i];

    if (list) delete list;
    list = new_list;
  }

  list [numItems-1] = item;
}

/*F+ Generic *GenericList::removeItem (int index)
*
*    PURPOSE:
*
*    Removes and returns the specified item from the list.
*
*    PARAMETERS:
*
*    index - The index of the item to remove.  The first item in the
*            list has an index of 0.
*
*    RETURN VALUES:
*
*    Returns a pointer to the item removed from the list.  Returns NULL
*    if the index is illegal.
*
*    PSEUDO CODE:
*
*    Initialize the item pointer to NULL
*    If the index is legal
*      Save the item corresponding to the index in the item pointer
*      Decrease the item counter
*      Move the items below the index up one position in the list
*    Return the item pointer
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
Generic *GenericList::removeItem (int index)
{
  Generic *item = (Generic *) NULL;

  if ((index > -1) && (index < numItems)) {
    item = list [index];

    numItems--;

    for (int i=index; i < numItems; i++)
      list [i] = list [i+1];
  }

  return item;
}

/*F+ void GenericList::removeItem (const Generic *item)
*
*    PURPOSE:
*
*    Removes the specified item from the list.
*
*    PARAMETERS:
*
*    item - The pointer to the item to remove.
*
*    PSEUDO CODE:
*
*    Initialize the index to an illegal value
*    Loop through the list
*      If an item in the list matches the specified item
*        Set the index value to the position of the item
*    If the index is legal
*      Decrease the item counter
*      Move the items below the index up one position in the list
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
void GenericList::removeItem (const Generic *item)
{
  int index = -1;

  for (int i=0; i < numItems; i++)
    if (item == list [i])
      index = i;

  if (index != -1) {
    numItems--;

    for (i=index; i < numItems; i++)
      list [i] = list [i+1];
  }
}

/*F+ int GenericList::findItem (const Generic *item) const
*
*    PURPOSE:
*
*    Find the specified item pointer in the list and return the position.
*
*    PARAMETERS:
*
*    item - The pointer to the item to find.
*
*    RETURN VALUES:
*
*    Returns the position of the item in the list.  If the item is not
*    found, -1 is returned.
*
*    PSEUDO CODE:
*
*    Initialize the index to -1
*    Loop through the list
*      If an item in the list matches the specified item
*        Set the index value to the position of the item
*    Return the index
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
int GenericList::findItem (const Generic *item) const
{
  int index = -1;

  for (int i=0; i < numItems; i++)
    if (item == list [i])
      index = i;

  return index;
}

/*F+ Generic *GenericList::getItem (int index) const
*
*    PURPOSE:
*
*    Returns the specified item from the list.
*
*    PARAMETERS:
*
*    index - The index of the item to get.  The first item in the
*            list has an index of 0.
*
*    RETURN VALUES:
*
*    Returns a pointer to the item in the list.  Returns NULL if the index
*    is illegal.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
Generic *GenericList::getItem (int index) const
{
  if ((index > -1) && (index < numItems))
    return list [index];

  return (Generic *) NULL;
}

/*F+ Generic *GenericList::traverse (Boolean reset_flag)
*
*    PURPOSE:
*
*    Traverses the list in order.  If the reset flag is set, then
*    traversal is started from the beginning of the list.
*
*    PARAMETERS:
*
*    reset_flag - If set, traversal starts from the beginning of the list.
*
*    RETURN VALUES:
*
*    Returns a pointer to an item in the list.  Returns NULL if there
*    are no more items in the list.
*
*    PSEUDO CODE:
*
*    If the reset flag is set, set the position to the beginning of the list
*    If the position is legal, return the item and increment the position
*    If the position is not legal, return NULL
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
Generic *GenericList::traverse (Boolean reset_flag)
{
  if (reset_flag) position = 0;

  if (position < numItems)
    return list [position++];

  return (Generic *) NULL;
}

/*F+ GenericList &GenericList::operator= (const GenericList& l)
*
*    PURPOSE:
*
*    Allows assignment of one list to another list.  The object pointers
*    in the list are copied, not the actual objects.  To clear a list
*    that is a copy of another list, use the dropItems member function,
*    instead of the clear function.
*
*    PARAMETERS:
*
*    l - The list to be assigned to (LHS of assignment).
*
*    RETURN VALUES:
*
*    Returns a list containing the same items as the input list.
*
*    PSEUDO CODE:
*
*    If the input list is not the same as the target list
*      Initialize the position counter
*      Set the physical size and grow size information
*      Set the items counter to the number of items in the input list
*      Delete the old target list
*      Allocate the physical size in memory
*      Copy the input list into the allocated memory
*    Return the target list
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
GenericList &GenericList::operator= (const GenericList &l)
{
  if (&l != this) {
    growSize = l.growSize;
    numItems = l.numItems;
    physicalSize = l.physicalSize;
    position = 0;      
    delete list;

    list = new Generic* [physicalSize];
    for (int i = 0; i < numItems; i++)
      list [i] = l.list [i];
  }

  return *this;
}

/*F+ Generic* &GenericList::operator[] (int index) const
*
*    PURPOSE:
*
*    Returns the specified item by reference, thereby allowing modification
*    of the list entry.
*
*    PARAMETERS:
*
*    index - The index of the item to return.  The first item in the list
*            has an index of 0.
*
*    RETURN VALUES:
*
*    Returns the item in the list by reference.  If the specified index
*    is illegal, then a message is output and a core dump is generated.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
Generic* &GenericList::operator[] (int index) const
{
  if ((index > -1) && (index < numItems))
    return list [index];

  fprintf (stderr, "GenericList::operator[]: Illegal index: %d\n", index);
  fprintf (stderr, "Abort with core dump...\n");
  abort ();
  return list [0]; // Dummy return to fool the compiler
}


/*F+ Generic &GenericList::operator+ (const GenericList& l)
*
*    PURPOSE:
*
*    Concatenates two lists together and returns the larger new list.
*
*    PARAMETERS 
*
*    l - The list to append to the end of the other list.
*
*    RETURN VALUES:
*
*    Returns the new list consisting of the two concatenated lists.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/10/90   Original creation.
*
*F-*/
GenericList GenericList::operator+ (const GenericList &l)
{
  GenericList new_list;

  for (int i=0; i < numItems; i++)
    new_list.addItem (list [i]);

  for (i=0; i < l.numItems; i++)
    new_list.addItem (l.list [i]);

  return new_list;
}


#ifdef TEST

#include <stream.h>

class GenericInteger : public Generic {
 public:
  int value;
};

main ()
{
  GenericList    l1, l2, l3;
  GenericInteger *I1 = new GenericInteger();
  GenericInteger *I2 = new GenericInteger();
  GenericInteger *I3 = new GenericInteger();
  GenericInteger *I4 = new GenericInteger();
  GenericInteger *I5 = new GenericInteger();
  GenericInteger *I6 = new GenericInteger();
  GenericInteger *I7 = new GenericInteger();
  GenericInteger *I8 = new GenericInteger();

  I1->value = 1;
  I2->value = 2;
  I3->value = 3;
  I4->value = 4;
  I5->value = 5;
  I6->value = 6;
  I7->value = 7;
  I8->value = 8;
  
  l1.addItem (I1);
  l1.addItem (I2);
  l1.addItem (I3);
  l1.addItem (I4);

  l2.addItem (I5);
  l2.addItem (I6);
  l2.addItem (I7);
  l2.addItem (I8);

  cout << "Correct result: 1-4:" << endl;
  for (int i=0; i < l1.getSize (); i++)
    cout << ((GenericInteger *) l1 [i])->value << endl;
  cout << endl;

  cout << "Correct result: 5-8:" << endl;
  for (i=0; i < l2.getSize (); i++)
    cout << ((GenericInteger *) l2 [i])->value << endl;
  cout << endl;

  cout << "Correct result: 1-8:" << endl;
  l1 = l1 + l2;
  for (i=0; i < l1.getSize (); i++)
    cout << ((GenericInteger *) l1 [i])->value << endl;
  cout << endl;

  cout << "Correct result: 0:" << endl;
  l1.dropItems ();
  cout << l1.getSize () << endl << endl;
  cout << endl;

  cout << "Correct result: 5-8:" << endl;
  l3 = l2;
  for (i=0; i < l3.getSize (); i++)
    cout << ((GenericInteger *) l3 [i])->value << endl;
  cout << endl;

  l2.dropItems ();
  l3.dropItems ();

  l1.addItem (I1);
  l1.addItem (I2);
  l1.addItem (I3);
  l1.addItem (I4);

  cout << "Correct result: 1-4:" << endl;
  for (i=0; i < l1.getSize (); i++)
    cout << ((GenericInteger *) l1 [i])->value << endl;
  cout << endl;

  cout << "Correct result: 1,2,4:" << endl;
  (void) l1.removeItem (2);
  for (i=0; i < l1.getSize (); i++)
    cout << ((GenericInteger *) l1 [i])->value << endl;
  cout << endl;

  cout << "Correct result: 1:" << endl;
  cout << l1.findItem (I2) << endl << endl;

  cout << "Correct result: 1,4:" << endl;
  l1.removeItem (I2);
  for (i=0; i < l1.getSize (); i++)
    cout << ((GenericInteger *) l1 [i])->value << endl;
  cout << endl;

  l1.dropItems ();

  l1.addItem (I1);
  l1.addItem (I2);
  l1.addItem (I3);
  l1.addItem (I4);

  cout << "Correct result: 1-4:" << endl;
  for (i=0; i < l1.getSize (); i++)
    cout << ((GenericInteger *) l1 [i])->value << endl;
  cout << endl;

  cout << "Correct result: 1,2,4:" << endl;
  (void) l1.removeItem (2);
  for (i=0; i < l1.getSize (); i++)
    cout << ((GenericInteger *) l1 [i])->value << endl;
  cout << endl;

  cout << "Correct result: 1:" << endl;
  cout << l1.findItem (I2) << endl << endl;

  cout << "Correct result: 1,4:" << endl;
  l1.removeItem (I2);
  for (i=0; i < l1.getSize (); i++)
    cout << ((GenericInteger *) l1 [i])->value << endl;
}

#endif /* TEST */
