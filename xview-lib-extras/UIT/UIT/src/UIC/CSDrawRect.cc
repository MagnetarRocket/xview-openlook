/*H+ CSDrawRect.cc
*
*    SCCS ID: @(#)CSDrawRect.cc 1.1 93/02/08 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSDrawRect class.
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
static char sccsid [] = "@(#)CSDrawRect.cc 1.1 93/02/08 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSDrawRect.h"

/*F+ CSDrawRect::CSDrawRect ()
*
*    PURPOSE:
*
*    Constructor of the CSDrawRect class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawRect::CSDrawRect ()
{
  setCSDrawRectDefaults ();
}

/*F+ CSDrawRect::setCSDrawRectDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSDrawRect class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawRect::setCSDrawRectDefaults ()
{
  borderDistanceFromXViewRect = UIC_NOT_SET;
  borderSize = UIC_NOT_SET;
  borderSpaceBetweenChildren = UIC_NOT_SET;
  isDrawObject = TRUE;
}

/*F+ CSDrawRect::~CSDrawRect ()
*
*    PURPOSE:
*
*    Destructor of the CSDrawRect class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawRect::~CSDrawRect ()
{
}

/*F+ void CSDrawRect::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSDrawRect object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawRect::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), DRAWRECT, NULL);

  if (borderDistanceFromXViewRect != UIC_NOT_SET)
    xv_set (owObject, DRAWRECT_BORDER1, borderDistanceFromXViewRect, NULL);

  if (borderSize != UIC_NOT_SET)
    xv_set (owObject, DRAWRECT_BORDER2, borderSize, NULL);

  if (borderSpaceBetweenChildren != UIC_NOT_SET)
    xv_set (owObject, DRAWRECT_BORDER3, borderSpaceBetweenChildren, NULL);

  setCSComponentAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
