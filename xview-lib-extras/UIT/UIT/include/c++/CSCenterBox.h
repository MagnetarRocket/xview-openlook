/*H+ CSCenterBox.h
*
*    SCCS ID: @(#)CSCenterBox.h 1.1 93/02/08 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSCenterBox class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSCenterBox
*
*    DESCRIPTION:
*
*    The CSCenterBox component is a collection object that creates a box
*    around its children and centers the children.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSCenterBox object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    CenterBox encapsulated by this object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSCenterBoxDefaults () - Initializes data members.
*
*/

#ifndef _CS_CENTER_BOX_H
#define _CS_CENTER_BOX_H

// Includes
#include "CSBox.h"

// Class Definition
class CSCenterBox : public CSBox {
public:
  CSCenterBox ();
  ~CSCenterBox ();

protected:
  virtual void createObject (UIObject *parent);

private:    
  void setCSCenterBoxDefaults ();
};

#endif /* _CS_CENTER_BOX_H */
