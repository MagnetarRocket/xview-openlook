/*H+ CSCenterBox.cc
*
*    SCCS ID: @(#)CSCenterBox.cc 1.1 93/02/08 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSCenterBox class.
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
static char sccsid [] = "@(#)CSCenterBox.cc 1.1 93/02/08 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSCenterBox.h"

/*F+ CSCenterBox::CSCenterBox ()
*
*    PURPOSE:
*
*    Constructor of the CSCenterBox class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSCenterBox::CSCenterBox ()
{
  setCSCenterBoxDefaults ();
}

/*F+ CSCenterBox::setCSCenterBoxDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSCenterBox class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSCenterBox::setCSCenterBoxDefaults ()
{
}

/*F+ CSCenterBox::~CSCenterBox ()
*
*    PURPOSE:
*
*    Destructor of the CSCenterBox class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSCenterBox::~CSCenterBox ()
{
}

/*F+ void CSCenterBox::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSCenterBox object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSCenterBox::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), CBOX, NULL);

  setCSBoxAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
