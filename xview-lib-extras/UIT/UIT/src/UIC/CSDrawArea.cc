/*H+ CSDrawArea.cc
*
*    SCCS ID: @(#)CSDrawArea.cc 1.1 93/02/08 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSDrawArea class.
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
static char sccsid [] = "@(#)CSDrawArea.cc 1.1 93/02/08 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSDrawArea.h"

/*F+ CSDrawArea::CSDrawArea ()
*
*    PURPOSE:
*
*    Constructor of the CSDrawArea class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawArea::CSDrawArea ()
{
  setCSDrawAreaDefaults ();
}

/*F+ CSDrawArea::setCSDrawAreaDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSDrawArea class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawArea::setCSDrawAreaDefaults ()
{
  leftX = (double) UIC_NOT_SET;
  rightX = (double) UIC_NOT_SET;
  upperY = (double) UIC_NOT_SET;
  lowerY = (double) UIC_NOT_SET;
  isDrawObject = TRUE;
}

/*F+ CSDrawArea::~CSDrawArea ()
*
*    PURPOSE:
*
*    Destructor of the CSDrawArea class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawArea::~CSDrawArea ()
{
}

/*F+ void CSDrawArea::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSDrawArea object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawArea::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), DRAWAREA, NULL);

  setCSDrawAreaAttributes (parent);
}

/*F+ void CSDrawArea::setCSDrawAreaAttributes (UIObject *parent)
*
*    PURPOSE:
*
*    Sets common CSDrawArea XView attributes that are required for creating
*    the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSDrawArea object.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawArea::setCSDrawAreaAttributes (UIObject *parent)
{
  if (leftX != (double) UIC_NOT_SET)
    xv_set (owObject, DRAWAREA_LEFT_X, &leftX, NULL);

  if (rightX != (double) UIC_NOT_SET)
    xv_set (owObject, DRAWAREA_RIGHT_X, &rightX, NULL);

  if (upperY != (double) UIC_NOT_SET)
    xv_set (owObject, DRAWAREA_UPPER_Y, &upperY, NULL);

  if (lowerY != (double) UIC_NOT_SET)
    xv_set (owObject, DRAWAREA_LOWER_Y, &lowerY, NULL);

  setCSComponentAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
