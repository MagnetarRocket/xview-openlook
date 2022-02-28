/*H+ CSBag.cc
*
*    SCCS ID: @(#)CSBag.cc 1.1 93/02/08 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSBag class.
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
static char sccsid [] = "@(#)CSBag.cc 1.1 93/02/08 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSBag.h"

/*F+ CSBag::CSBag ()
*
*    PURPOSE:
*
*    Constructor of the CSBag class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSBag::CSBag ()
{
  setCSBagDefaults ();
}

/*F+ CSBag::setCSBagDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSBag class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSBag::setCSBagDefaults ()
{
  anchored = FALSE;
  anchoredSet = FALSE;
  autoShrink = FALSE;
  autoShrinkSet = FALSE;
}

/*F+ CSBag::~CSBag ()
*
*    PURPOSE:
*
*    Destructor of the CSBag class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSBag::~CSBag ()
{
}

/*F+ void CSBag::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSBag object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSBag::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), BAG, NULL);

  if (anchoredSet)
    xv_set (owObject, BAG_ANCHORED, anchored, NULL);

  if (autoShrinkSet)
    xv_set (owObject, BAG_AUTO_SHRINK, autoShrink, NULL);

  setCSComponentAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
