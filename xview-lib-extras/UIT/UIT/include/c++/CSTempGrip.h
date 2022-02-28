/*H+ CSTempGrip.h
*
*    SCCS ID: @(#)CSTempGrip.h 1.1 93/02/08 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSTempGrip class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSTempGrip
*
*    DESCRIPTION:
*
*    The CSTempGrip renders a temporary graphical Grip.
*
*    INHERITED CLASSES:
*
*    CSGrip - A CSTempGrip object is a CSGrip.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    Temp Grip encapsulated by this object.
*
*/

#ifndef _CS_TEMP_GRIP_H
#define _CS_TEMP_GRIP_H

// Includes
#include "CSGrip.h"

// Class Definition
class CSTempGrip : public CSGrip {
public:
  CSTempGrip ();
  ~CSTempGrip ();

protected:
  virtual void createObject (UIObject *parent);
};

#endif /* _CS_TEMP_GRIP_H */
