/*H+ CSTempGrip.cc
*
*    SCCS ID: @(#)CSTempGrip.cc 1.1 93/02/08 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSTempGrip class.
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
static char sccsid [] = "@(#)CSTempGrip.cc 1.1 93/02/08 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSTempGrip.h"

/*F+ CSTempGrip::CSTempGrip ()
*
*    PURPOSE:
*
*    Constructor of the CSTempGrip class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSTempGrip::CSTempGrip ()
{
}

/*F+ CSTempGrip::~CSTempGrip ()
*
*    PURPOSE:
*
*    Destructor of the CSTempGrip class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSTempGrip::~CSTempGrip ()
{
}

/*F+ void CSTempGrip::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSTempGrip object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSTempGrip::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), TEMP_GRIP, NULL);

  setCSGripAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
