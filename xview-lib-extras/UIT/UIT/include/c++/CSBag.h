/*H+ CSBag.h
*
*    SCCS ID: @(#)CSBag.h 1.1 93/02/08 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSBag class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSBag
*
*    DESCRIPTION:
*
*    The CSBag component is a collection object that provides anchoring
*    and simple geometry control.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSBag object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setAnchored (Boolean) - Specifies that the upper left edge of
*    the bag will remain fixed when children are added and deleted.
*
*    void setAutoShrink (Boolean) - Specifies that the bag should shrink when
*    children are deleted.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    BAG encapsulated by this object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSBagDefaults () - Initializes data members.
*
*/

#ifndef _CS_BAG_H
#define _CS_BAG_H

// Includes
#include "CSComponent.h"

#include <sspkg/rectobj.h>

// Class Definition
class CSBag : public CSComponent {
public:
  CSBag ();
  ~CSBag ();

  void setAnchored (Boolean flag)
    {
      anchored = flag;
      anchoredSet = TRUE;

      if (owObject)
	xv_set (owObject, BAG_ANCHORED, anchored, NULL);
    }

  void setAutoShrink (Boolean flag)
    {
      autoShrink = flag;
      autoShrinkSet = TRUE;

      if (owObject)
	xv_set (owObject, BAG_AUTO_SHRINK, autoShrink, NULL);
    }

protected:
  Boolean        anchored;
  Boolean        anchoredSet;
  Boolean        autoShrink;
  Boolean        autoShrinkSet;

  virtual void createObject (UIObject *parent);

private:    
  void setCSBagDefaults ();
};

#endif /* _CS_BAG_H */
