/*H+ CSDrawLine.cc
*
*    SCCS ID: @(#)CSDrawLine.cc 1.2 93/06/01 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSDrawLine class.
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
static char sccsid [] = "@(#)CSDrawLine.cc 1.2 93/06/01 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSDrawLine.h"

/*F+ CSDrawLine::CSDrawLine ()
*
*    PURPOSE:
*
*    Constructor of the CSDrawLine class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawLine::CSDrawLine ()
{
  setCSDrawLineDefaults ();
}

/*F+ CSDrawLine::setCSDrawLineDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSDrawLine class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawLine::setCSDrawLineDefaults ()
{
  arrowAngle0 = UIC_NOT_SET;
  arrowAngle1 = UIC_NOT_SET;
  arrowInsetLength0 = UIC_NOT_SET;
  arrowInsetLength1 = UIC_NOT_SET;
  arrowHeadLength0 = UIC_NOT_SET;
  arrowHeadLength1 = UIC_NOT_SET;
  arrowStyle0 = (Arrow_style) UIC_NOT_SET;
  arrowStyle1 = (Arrow_style) UIC_NOT_SET;
  x0 = y0 = x1 = y1 = UIC_NOT_SET;
  isDrawObject = TRUE;
}

/*F+ CSDrawLine::~CSDrawLine ()
*
*    PURPOSE:
*
*    Destructor of the CSDrawLine class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSDrawLine::~CSDrawLine ()
{
}

/*F+ void CSDrawLine::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSDrawLine object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSDrawLine::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), DRAWLINE, NULL);

  if (arrowAngle0 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_ARROW_ANGLE, 0, arrowAngle0, NULL);

  if (arrowAngle1 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_ARROW_ANGLE, 1, arrowAngle1, NULL);

  if (arrowInsetLength0 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_ARROW_INSET_LENGTH, 0, arrowInsetLength0, NULL);

  if (arrowInsetLength1 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_ARROW_INSET_LENGTH, 1, arrowInsetLength1, NULL);

  if (arrowHeadLength0 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_ARROW_LENGTH, 0, arrowHeadLength0, NULL);

  if (arrowHeadLength1 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_ARROW_LENGTH, 0, arrowHeadLength1, NULL);

  if (arrowStyle0 != (Arrow_style) UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_ARROW_STYLE, 0, arrowStyle0, NULL);

  if (arrowStyle1 != (Arrow_style) UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_ARROW_STYLE, 1, arrowStyle1, NULL);

  if (x0 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_X, 0, x0, NULL);

  if (x1 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_X, 1, x1, NULL);

  if (y0 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_Y, 0, y0, NULL);

  if (y1 != UIC_NOT_SET)
    xv_set (owObject, DRAWLINE_Y, 1, y1, NULL);

  setCSComponentAttributes (parent);
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
