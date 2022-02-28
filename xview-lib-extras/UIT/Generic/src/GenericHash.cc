/*H+ GenericHash.cc
*
*    SCCS ID: @(#)GenericHash.cc 1.6 93/04/09 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the GenericHash class.
*
*    GLOBAL ROUTINES:
*
*    GenericHash (int size)       - Creates a hash table.  The first
*                                   prime number >= to the specified
*                                   size is the number of hash table
*                                   buckets.
*    GenericHash (const GenericHash&)
*                                 - Creates a hash table and initializes the
*                                   hash table with another hash table.
*    void GenericHash::addItem (const char *, Generic *)
*                                 - Adds the specified item to the hash table.
*    Generic *GenericHash::removeItem (const char *)
*                                 - Removes and returns the specified item
*                                   from the hash table.
*    Generic *GenericHash::getItem (const char *address) const
*                                 - Returns a pointer to the data corresponding
*                                   to the specified address.  Returns NULL
*				    if no data found.
*    Generic *GenericHash::traverse (Boolean reset_flag, char* &address)
*                                 - Traverses the hash table in reverse order.
*                                   Returns the object pointer and address.
*                                   If the reset flag is set, traversal starts
*                                   from the beginning of the hash table.
*                                   Additons to, removals from, and use of
*                                   the hash table with the + operator
*                                   automatically resets the traversal.
*    void clear ()                - Clears the hash table and destroys each
*                                   item in the hash table.
*    void dropItems ()            - Drops the items from a hash table without
*                                   destroying the items.
*    GenericHash &GenericHash::operator= (const GenericHash&)
*                                 - Allows assignment of one hash table to
*                                   another.
*    Generic* &GenericHash::operator[] (const char *) const
*                                 - Allows access to the specified item in the
*                                   hash table.  The hash table entry can be
*                                   read and modified.
*    GenericHash &GenericHash::operator+ (const GenericHash&)
*                                 - Concatenates two hash tables.
*
*    LOCAL ROUTINES:
*
*    void GenericHash::initialize ()
*				  - Initializes the internals of the table.
*    int GenericHash::getPrime (int) const
*				  - Returns the first prime number >= to the
*                                   input parameter.
*    int GenericHash::hashString (const char *) const
*				  - Converts a string into a bucket number.
*
*    Generic *privateTraverse (HashTableEntry* &position, char* &address) const
*                                 - Provides a const traverse for internal use.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       09/12/90   Original creation.
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)GenericHash.cc 1.6 93/04/09 SMI"; 
#endif

// Includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Boolean.h"
#include "GenericHash.h"
#include "GenericList.h"

/*F+ GenericHash::GenericHash (int size)
*
*    PURPOSE:
*
*    Creates a hash table.  The first prime number >= to the specified
*    size is the number of buckets.
*
*    PARAMETERS:
*
*    size - The size of the hash table.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
GenericHash::GenericHash (int size)
{
  numBuckets = getPrime (size);
  table = new HashTableEntry* [numBuckets];

  initialize ();
}

/*F+ GenericHash::GenericHash (const GenericHash &ht)
*
*    PURPOSE:
*
*    Creates a hash table and initailizes it with another hash table.  The
*    object pointers in the hash table are copied, not the actual objects.
*    To clear a hash table that is initialized by another hash table, use the
*    dropItems member function instead of the clear member function.
*
*    PARAMETERS:
*
*    ht - The hash table to be used to initialize.
*
*    RETURN VALUES:
*
*    Returns a hash table containing the same items as the input hash table.
*
*    PSEUDO CODE:
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
GenericHash::GenericHash (const GenericHash &ht)
{
  numBuckets = ht.numBuckets;
  table = new HashTableEntry* [numBuckets];
 
  initialize (); 

  char        *address;
  GenericList  addresses;
  GenericList  entries;
  Generic     *item;

  HashTableEntry *ht_position = ht.start;
  while (item = ht.privateTraverse (ht_position, address)) {
    entries.addItem (item);
    addresses.addItem ((Generic *) address);
  }

  for (int i = entries.getSize () - 1; i > -1; i--)
    addItem ((char *) addresses [i], entries [i]);
  
  position = start;

  entries.dropItems ();
  addresses.dropItems ();
}

/*F+ void GenericHash::initialize ()
*
*    PURPOSE:
*
*    Initialize the internals of the hash table.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
void GenericHash::initialize ()
{
  for (int i=0; i < numBuckets; i++)
    table [i] = (HashTableEntry *) NULL;

  position = (HashTableEntry *) NULL;
  start = (HashTableEntry *) NULL;
}

/*F+ GenericHash::~GenericHash ()
*
*    PURPOSE:
*
*    GanericHash destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
GenericHash::~GenericHash ()
{
  HashTableEntry *prev;
  HashTableEntry *tmp;

  // Delete only the data items inthe table, the deletion
  // of the actual table will delete the HashTableEntry
  // instances.
  tmp = start;
  while (tmp) {
    delete tmp->item;
    prev = tmp;
    tmp = tmp->rightEntry;
  }

  delete [] table;
}

/*F+ void GenericHash::addItem (const char *address, Generic *item)
*
*    PURPOSE:
*
*    Add a pointer to an object to the hash table.
*
*    PARAMETERS:
*
*    address - The hash address to place the object at.
*    item    - The pointer to the object to add to the hash table.
*
*    PSEUDO CODE:
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
void GenericHash::addItem (const char *address, Generic *item)
{
  int             bucket;
  HashTableEntry *new_entry;
  HashTableEntry *tmp;

  for (tmp = table [bucket = hashString (address)];
       tmp != (HashTableEntry *) NULL;
       tmp = tmp->nextEntry)
    if (!strcmp (tmp->address, address)) {
      tmp->item = item;
      return;
    }

  new_entry = new HashTableEntry;
  new_entry->address = strcpy (new char [strlen (address) + 1], address);

  new_entry->leftEntry = (HashTableEntry *) NULL;
  new_entry->rightEntry = start;

  if (start)
    start->leftEntry = new_entry;

  start = new_entry;
  position = start;

  new_entry->nextEntry = table [bucket];
  table [bucket] = new_entry;

  new_entry->item = item;
}

/*F+ Generic *GenericHash::removeItem (const char *address)
*
*    PURPOSE:
*
*    Removes the specified item from the hash table.
*
*    PARAMETERS:
*
*    address - The hash address of the object to remove.
*
*    RETURN VALUES:
*
*    Returns a pointer to the item removed from the hash table.  Returns NULL
*    if the hash address is illegal.
*
*    PSEUDO CODE:
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
Generic *GenericHash::removeItem (const char *address)
{
  int             bucket;
  HashTableEntry *prev = (HashTableEntry *) NULL;
  Generic        *ret;
  HashTableEntry *tmp;

  bucket = hashString (address);

  if (!(tmp = table [bucket]))
    return (Generic *) NULL;
  else {
    while (tmp) {
      if (!strcmp (tmp->address, address))
	break;

      prev = tmp;
      tmp = tmp->nextEntry;
    }

    if (!tmp)
      return (HashTableEntry *) NULL;
  }

  if (prev)
    prev->nextEntry = tmp->nextEntry;
  else
    table [bucket] = tmp->nextEntry;

  if (tmp->leftEntry) {
    tmp->leftEntry->rightEntry = tmp->rightEntry;

    if (tmp->rightEntry)
      tmp->rightEntry->leftEntry = tmp->leftEntry;
  }
  else {
    start = tmp->rightEntry;

    if (tmp->rightEntry)
      tmp->rightEntry->leftEntry = (HashTableEntry *) NULL;
  }

  position = start;
  ret = tmp->item;
  delete tmp;

  return ret;
}

/*F+ Generic *GenericHash::getItem (const char *address) const
*
*    PURPOSE:
*
*    Returns the specified item from the hash table.
*
*    PARAMETERS:
*
*    address - The hash address of the object to return.
*
*    RETURN VALUES:
*
*    Returns a pointer to the item in the hash table.  Returns NULL if the
*    hash address is illegal.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
Generic *GenericHash::getItem (const char *address) const
{
  int             bucket;
  HashTableEntry *tmp;

  for (tmp = table [bucket = hashString (address)];
       tmp != (HashTableEntry *) NULL;
       tmp = tmp->nextEntry)
    if (!strcmp (tmp->address, address))
      return tmp->item;

  return (Generic *) NULL;
}

/*F+ Generic *GenericHash::traverse (Boolean reset_flag, char* &address)
*
*    PURPOSE:
*
*    Traverses the hash table in reverse order.  If the reset flag is set,
*    then traversal is started from the beginning of the hash table.
*
*    PARAMETERS:
*
*    reset_flag - If set, traversal starts from the beginning of the hash
*                 table.
*    address    - The address of the entry that was traversed.
*
*    RETURN VALUES:
*
*    address - Returns the address of the traversed entry.  The address
*              returned is not a copy of the hash table address, just
*              a pointer to it.
*
*    Returns a pointer to an item in the hash table.  Returns NULL if there
*    are no more items in the hash table.
*
*    PSEUDO CODE:
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
Generic *GenericHash::traverse (Boolean reset_flag, char* &address)
{
  Generic *item;

  if (reset_flag)
    position = start;

  if (position) {
    item = position->item;
    address = position->address;
    position = position->rightEntry;

    return item;
  }

  address = (char *) NULL;
  return (Generic *) NULL;
}

/*F+ void GenericHash::clear ()
*
*    PURPOSE:
*
*    Clears the hash table and destroys each item.
*
*    PSEUDO CODE:
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
void GenericHash::clear ()
{
  HashTableEntry *prev;
  HashTableEntry *tmp;

  tmp = start;
  while (tmp) {
    delete tmp->item;

    prev = tmp;
    tmp = tmp->rightEntry;

    delete prev;
  }

  initialize ();
}

/*F+ void GenericHash::dropItems ()
*
*    PURPOSE:
*
*    Drops the items in the hash table without destroying the items.
*
*    PSEUDO CODE:
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
void GenericHash::dropItems ()
{
  HashTableEntry *prev;
  HashTableEntry *tmp;

  tmp = start;
  while (tmp) {
    prev = tmp;
    tmp = tmp->rightEntry;

    delete prev;
  }

  initialize ();
}

/*F+ GenericHash &GenericHash::operator= (const GenericHash& ht)
*
*    PURPOSE:
*
*    Allows assignment of one hash table to another hash table.  The object
*    pointers in the hash table are copied, not the actual objects.  To
*    clear a hash table that is a copy of another hash table, use the
*    dropItems member function instead of the clear member function.
*
*    PARAMETERS:
*
*    ht - The hash table to be assigned to (LHS of assignment).
*
*    RETURN VALUES:
*
*    Returns a hash table containing the same items as the input hash table.
*
*    PSEUDO CODE:
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
GenericHash &GenericHash::operator= (const GenericHash &ht)
{
  char        *address;
  GenericList  addresses;
  GenericList  entries;
  Generic     *item;

  if (&ht != this) {
    clear ();

    HashTableEntry *ht_position = ht.start;
    while (item = ht.privateTraverse (ht_position, address)) {
      entries.addItem (item);
      addresses.addItem ((Generic *) address);
    }

    for (int i = entries.getSize () - 1; i > -1; i--)
      addItem ((char *) addresses [i], entries [i]);

    position = start;

    entries.dropItems ();
    addresses.dropItems ();
  }

  return *this;
}

/*F+ Generic* &GenericHash::operator[] (const char *address) const
*
*    PURPOSE:
*
*    Returns the specified item by reference, thereby allowing modification
*    of the list entry.
*
*    PARAMETERS:
*
*    address - The hash address of the entry to return.
*
*    RETURN VALUES:
*
*    Returns the item in the hash table  by reference.  If the specified
*    hash address is illegal, then a message is issued and a core dump
*    is generated.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/11/90   Original creation.
*
*F-*/
Generic* &GenericHash::operator[] (const char *address) const
{
  int             bucket;
  HashTableEntry *tmp;

  for (tmp = table [bucket = hashString (address)];
       tmp != (HashTableEntry *) NULL;
       tmp = tmp->nextEntry)
    if (!strcmp (tmp->address, address))
      return tmp->item;

  fprintf (stderr, "GenericHash: Illegal hash address: %s\n", address);
  fprintf (stderr, "Abort with core dump...\n");
  abort ();
  return start->item; // Dummy return to fool the compiler
}

/*F+ GenericHash GenericHash::operator+ (const GenericHash& ht)
*
*    PURPOSE:
*
*    Concatenates two hash tables together and returns the larger new
*    hash table.
*
*    PARAMETERS 
*
*    ht - The hash table to append to the end of the other hash table.
*
*    RETURN VALUES:
*
*    Returns the new hash table consisting of the two concatenated hash table.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/10/90   Original creation.
*
*F-*/
GenericHash &GenericHash::operator+ (const GenericHash &ht)
{
  char        *address;
  GenericList  addresses;
  GenericList  entries;
  Generic     *item;

  HashTableEntry *ht_position = ht.start;
  while (item = ht.privateTraverse (ht_position, address)) {
    entries.addItem (item);
    addresses.addItem ((Generic *) address);
  }

  for (int i = entries.getSize () - 1; i > -1; i--)
    addItem ((char *) addresses [i], entries [i]);

  position = start;

  entries.dropItems ();
  addresses.dropItems ();

  return *this;
}

/*F+ int GenericHash::getPrime (int number) const
*
*    PURPOSE:
*
*    Find the first prime number >= to the specified integer.
*
*    PARAMETERS 
*
*    number - The integer to find a prime number for.
*
*    RETURN VALUES:
*
*    Returns the prime number.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
int GenericHash::getPrime (int number) const
{
  int found_prime = 0;
  int candidate = number;
  int divisible_flag;
  int divisor;
  int limit;

  /* Make sure the first candidate is an odd number */
  candidate = candidate + (1 - (candidate % 2));

  /* While the prime has not been found... */
  while (!found_prime) {
    /* Initialize flag indicating whether or not the current number
       is divisible by something */
    divisible_flag = 0;

    /* Test all of the odd numbers from 3 to the square root of the
       current candidate */
    limit = (int) sqrt ((double) candidate);
    for (divisor = 3; divisor <= limit; divisor += 2) {
      /* If the candidate is divisible by the divisor... */
      if (candidate % divisor == 0) {
	/* Set a flag and forget about testing any more divisors */
	divisible_flag = 1;
	break;
      }
    }

    /* If the number was not divisible by anything... */
    if (divisible_flag == 0) {
      /* It is a prime */
      found_prime = 1;
    }
    /* Else the candidate was not prime... */
    else {
      /* Get a new candidate */
      candidate += 2;
    }
  }

  /* Return the prime number */
  return candidate;
}

/*F+ int GenericHash::hashString (const char *string) const
*
*    PURPOSE:
*
*    Convert the specified string into a hash bucket number.
*
*    PARAMETERS 
*
*    string - The string to convert.
*
*    RETURN VALUES:
*
*    Returns a hash table bucket number.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       09/13/90   Original creation.
*
*F-*/
int GenericHash::hashString (const char *string) const
{
  int      i = 1;
  unsigned result = 0;

  /* Loop through the string */
  while (*string)
    /* Shift the Nth character by N bits and sum */
    result += *string++ << i++;

  /* Return the bucket number */
  return (result % numBuckets); 
}

/*F+ Generic *privateTraverse (HashTableEntry* &position, char* &address) const
*
*    PURPOSE:
*
*    Provide a const traverse for internal use.
*
*    PARAMETERS 
*
*    position - The current position in the form of a HashTableEntry pointer.
*               The next position is returned.
*
*    address  - The address the last item traversed.
*
*    RETURN VALUES:
*
*    Returns the last item traversed.  Returns NULL if there are no more
*    items.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*    MGS       11/22/92   Original creation.
*
*F-*/
Generic *GenericHash::privateTraverse (HashTableEntry* &position,
				       char*           &address) const
{
  Generic *item;
 
  if (position) {
    item = position->item;
    address = position->address;
    position = position->rightEntry;
 
    return item;
  }
 
  address = (char *) NULL;
  return (Generic *) NULL;
}


#ifdef TEST

#include <stream.h>

class GenericInteger : public Generic {
 public:
  int value;
};

main ()
{
  GenericHash     h1, h2;
  GenericInteger  *I1 = new GenericInteger();
  GenericInteger  *I2 = new GenericInteger();
  GenericInteger  *I3 = new GenericInteger();
  GenericInteger  *I4 = new GenericInteger();
  GenericInteger  *I5 = new GenericInteger();
  GenericInteger  *I6 = new GenericInteger();
  GenericInteger  *I7 = new GenericInteger();
  GenericInteger  *I8 = new GenericInteger();

  char           *address;
  Generic        *item;

  I1->value = 1;
  I2->value = 2;
  I3->value = 3;
  I4->value = 4;
  I5->value = 5;
  I6->value = 6;
  I7->value = 7;
  I8->value = 8;

  h1.addItem ("one", I1);
  h1.addItem ("two", I2);
  h1.addItem ("three", I3);
  h1.addItem ("four", I4);

  h2.addItem ("five", I5);
  h2.addItem ("six", I6);
  h2.addItem ("seven", I7);
  h2.addItem ("eight", I8);

  cout << "Correct result: 1-4:" << endl;
  cout << ((GenericInteger *) h1 ["one"])->value << endl;
  cout << ((GenericInteger *) h1 ["two"])->value << endl;
  cout << ((GenericInteger *) h1 ["three"])->value << endl;
  cout << ((GenericInteger *) h1 ["four"])->value << endl;
  cout << endl;

  cout << "Correct result: 5-8:" << endl;
  cout << ((GenericInteger *) h2 ["five"])->value << endl;
  cout << ((GenericInteger *) h2 ["six"])->value << endl;
  cout << ((GenericInteger *) h2 ["seven"])->value << endl;
  cout << ((GenericInteger *) h2 ["eight"])->value << endl;
  cout << endl;

  h1 = h1 + h2;

  cout << "Correct result: 1-8:" << endl;
  cout << ((GenericInteger *) h1 ["one"])->value << endl;
  cout << ((GenericInteger *) h1 ["two"])->value << endl;
  cout << ((GenericInteger *) h1 ["three"])->value << endl;
  cout << ((GenericInteger *) h1 ["four"])->value << endl;
  cout << ((GenericInteger *) h1 ["five"])->value << endl;
  cout << ((GenericInteger *) h1 ["six"])->value << endl;
  cout << ((GenericInteger *) h1 ["seven"])->value << endl;
  cout << ((GenericInteger *) h1 ["eight"])->value << endl;
  cout << endl;

  item = h1.removeItem ("three");
  cout << "Correct result: 3:" << endl;
  cout << ((GenericInteger *) item)->value << endl;
  cout << endl;

  item = h1.removeItem ("five");
  cout << "Correct result: 5:" << endl;
  cout << ((GenericInteger *) item)->value << endl;
  cout << endl;

  int reset_flag = 0;
  cout << "Correct result: Addresses and values for: 8,7,6,4,2,1:" << endl;
  while (item = h1.traverse (reset_flag, address)) {
    cout << "Address: " << (char *) address << "\t";
    cout << "Value: " << ((GenericInteger *) item)->value << endl;
  }
  cout << endl;

  h1 ["seven"] = I3;
  cout << "Correct result: 3:" << endl;
  cout << ((GenericInteger *) h1 ["seven"])->value << endl;
  cout << endl;

  h1.dropItems ();

  h1.addItem ("one", I1);
  h1.addItem ("two", I2);
  h1.addItem ("three", I3);
  h1.addItem ("four", I4);

  cout << "Correct result: 1-4:" << endl;
  cout << ((GenericInteger *) h1 ["one"])->value << endl;
  cout << ((GenericInteger *) h1 ["two"])->value << endl;
  cout << ((GenericInteger *) h1 ["three"])->value << endl;
  cout << ((GenericInteger *) h1 ["four"])->value << endl;
  cout << endl;

  reset_flag = 1;
  cout << "Correct result: Addresses and values for: 4,3,2,1:" << endl;
  while (item = h1.traverse (reset_flag, address)) {
    cout << "Address: " << (char *) address << "\t";
    cout << "Value: " << ((GenericInteger *) item)->value << endl;
    reset_flag = 0;
  }
}

#endif /* TEST */
