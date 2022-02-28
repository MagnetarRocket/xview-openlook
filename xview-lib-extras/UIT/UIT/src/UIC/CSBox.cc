/*H+ CSBox.cc
*
*    SCCS ID: @(#)CSBox.cc 1.1 93/02/08 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSBox class.
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
static char sccsid [] = "@(#)CSBox.cc 1.1 93/02/08 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSBox.h"

/*F+ CSBox::CSBox ()
*
*    PURPOSE:
*
*    Constructor of the CSBox class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSBox::CSBox ()
{
  setCSBoxDefaults ();
}

/*F+ CSBox::setCSBoxDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSBox class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSBox::setCSBoxDefaults ()
{
  gap = UIC_NOT_SET;
  layout = (Orientation) UIC_NOT_SET;
}

/*F+ CSBox::~CSBox ()
*
*    PURPOSE:
*
*    Destructor of the CSBox class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSBox::~CSBox ()
{
}

/*F+ void CSBox::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSBox object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSBox::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), BOX, NULL);

  setCSBoxAttributes (parent);
}

/*F+ void CSBox::setCSBoxAttributes (UIObject *parent)
*
*    PURPOSE:
*
*    Sets the CSBox attributes.
*
*    PARAMETERS:
*
*    parent - The parent of the CSBox object.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void CSBox::setCSBoxAttributes (UIObject *parent)
{
  if (gap != UIC_NOT_SET)
    xv_set (owObject, BOX_GAP, gap, NULL);

  if (layout == VERTICAL)
    xv_set (owObject, BOX_LAYOUT, BOX_LAYOUT_VERTICAL, NULL);
  else if (layout == HORIZONTAL)
    xv_set (owObject, BOX_LAYOUT, BOX_LAYOUT_HORIZONTAL, NULL);

  setCSComponentAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
