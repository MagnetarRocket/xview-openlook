/*H+ GenericHash.h
*
*    SCCS ID: @(#)GenericHash.h 1.3 93/02/08 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the GenericHash class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       09/12/90   Original creation.
*
*H-*/

/*C+ GenericHash
*
*    DESCRIPTION:
*
*    The GenericHash class a provides a way to create hash tables containing
*    any object sub-classed off of the Generic object class.  The hash table
*    can have objects added to it, deleted from it, accessed randomly,
*    traversed in reverse order, and have other hash tables added to it.
*    When the hash table is destructed, delete is called on each of the items
*    in the hash table.  The hash address for an object added to the table
*    is a character string.
*
*    INHERITED CLASSES:
*
*    Generic - Inheriting the Generic class will allow the GenericHash class
*              to be treated as a Generic object.  Therefore, a GenericHash
*              can contain a GenericHash.
*
*    LOCAL CLASSES:
*
*    HashTableEntry - Contains one entry in a hash table and the links to the
*                     previous and next items placed in the hash table as well
*                     as the next item in the current hash table bucket.
*
*    ORDINARY DATA MEMBERS:
*
*    int              numBuckets - Number of hash table buckets
*    HashTableEntry  *position   - Current traversal position
*    HashTableEntry  *start      - Pointer to the first hash table entry
*    HashTableEntry **table      - The actual hash table
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    GenericHash (int size=101)              - Creates a hash table.  The first
*                                              prime number >= to the specified
*                                              size is the number of hash table
*                                              buckets.
*    void addItem (const char *address, Generic *)
*                                            - Adds the specified item to the
*                                              hash table.
*    Generic *removeItem (const char *address)
*                                            - Removes and returns the
*                                              item from the hash table.
*    Generic *getItem (const char *address) const
*                                            - Gets the specified item from
*                                              the hash table.
*    Generic *traverse (Boolean reset_flag, char* &address)
*                                            - Traverses the hash table in
*                                              reverse order of placement
*                                              in the hash table.  If the
*                                              reset flag is set, traversal
*                                              starts at the beginning of the
*                                              hash table.  The function
*                                              returns both the object pointer
*                                              and the hash address.
*    int getSize () const                    - Returns the number of buckets
*                                              in the hash table.
*    void clear ()                           - Clears the hash table and
*                                              destroys each item in the hash
*                                              table.
*    void dropItems ()                       - Drops the items in the hash
*                                              table without destroying them.
*    GenericHash &operator= (const GenericHash&)
*                                            - Allows assignment of one hash
*                                              table to another.
*    Generic* &operator[] (const char *address) const
*                                            - Allows access to the specified
*                                              item in the hash table.  The
*                                              hash table entry can be read and
*                                              modified.
*    GenericHash &operator+ (const GenericHash&)
*                                            - Concatenates two hash tables.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    void initialize ()			     - Initialize the internals of
*					       the hash table.
*    int getPrime (int)			     - Return the closest prime >=
*					       to the specified integer.
*    int hashString (const char *) const     - Converts the specified string
*					       into a bucket number.
*    Generic *privateTraverse (HashTableEntry*&, char*&) const 
*                                            - Provides a const traverse for
*                                              internal use.
*
*C-*/
#ifndef _GENERICHASH_H
#define _GENERICHASH_H

// Includes
#include <stddef.h>

#include "Boolean.h"
#include "Generic.h"

// Local Class Definitions
class HashTableEntry : public Generic {
public:
  HashTableEntry ()
    {
      nextEntry = (HashTableEntry *) NULL;
      rightEntry = (HashTableEntry *) NULL;
      leftEntry = (HashTableEntry *) NULL;
      address = (char *) NULL;
      item = (Generic *) NULL;
    }

  ~HashTableEntry ()
    {
      if (address) delete address;
    }

  HashTableEntry *nextEntry;
  HashTableEntry *rightEntry;
  HashTableEntry *leftEntry;
  char           *address;
  Generic        *item;
};

// Class Definition
class GenericHash : public Generic {
 public:
  GenericHash (int size=101);
  GenericHash (const GenericHash&);
  ~GenericHash ();
  void         addItem (const char *, Generic *);
  Generic     *removeItem (const char *);
  Generic     *getItem (const char *) const;
  Generic     *traverse (Boolean, char*&);
  int          getSize () const { return numBuckets; }
  void         clear ();
  void         dropItems ();
  GenericHash &operator= (const GenericHash&);
  Generic*    &operator[] (const char *) const;
  GenericHash &operator+ (const GenericHash&);
 protected:
  int              numBuckets;
  HashTableEntry  *position;
  HashTableEntry  *start;
  HashTableEntry **table;

  void     initialize (); 
  int      getPrime (int) const;
  int      hashString (const char *) const;
  Generic *privateTraverse (HashTableEntry*&, char*&) const;
};

#endif /* _GENERICHASH_H */
