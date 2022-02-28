/*H+ Generic.h
*
*    SCCS ID: @(#)Generic.h 1.2 93/02/08 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the Generic class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       09/10/90   Original creation.
*
*H-*/

/*C+ Generic
*
*    DESCRIPTION:
*
*    The Generic class provides a common base class for all other classes
*    to be derived from.  This allows the "delete" operator to understand
*    how to delete a specific class from a polymorhic collection of Generic
*    derived classes.
*
*C-*/
#ifndef _GENERIC_H
#define _GENERIC_H

// Class Definition
class Generic {
public:
  Generic ();
  virtual ~Generic (); 
};

#endif /* _GENERIC_H */
