/*H+ CSBox.h
*
*    SCCS ID: @(#)CSBox.h 1.1 93/02/08 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSBox class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSBox
*
*    DESCRIPTION:
*
*    The CSBox component is a collection object that creates a box
*    around its children.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSBox object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setGap (int) - Specifies the distance between children.
*
*    void setLayout (Orientation) - Specifies the layout direction of the children.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    BOX encapsulated by this object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSBoxDefaults () - Initializes data members.
*
*/

#ifndef _CS_BOX_H
#define _CS_BOX_H

// Includes
#include "CSComponent.h"

#include <sspkg/box.h>

// Class Definition
class CSBox : public CSComponent {
public:
  CSBox ();
  ~CSBox ();

  void setGap (int new_gap)
    {
      gap = new_gap;

      if (owObject)
	xv_set (owObject, BOX_GAP, gap, NULL);
    }

  void setLayout (Orientation new_layout)
    {
      layout = new_layout;

      if (owObject) {
	if (layout == VERTICAL)
	  xv_set (owObject, BOX_LAYOUT, BOX_LAYOUT_VERTICAL, NULL);
	else if (layout == HORIZONTAL)
	  xv_set (owObject, BOX_LAYOUT, BOX_LAYOUT_HORIZONTAL, NULL);
      }
    }

protected:
  int         gap;
  Orientation layout;

  virtual void createObject (UIObject *parent);
  void         setCSBoxAttributes (UIObject *parent);

private:    
  void setCSBoxDefaults ();
};

#endif /* _CS_BOX_H */
