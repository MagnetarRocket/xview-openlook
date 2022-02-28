/*H+ CheckBoxChoice.h
*
*    SCCS ID: @(#)CheckBoxChoice.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CheckBoxChoice class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       05/10/91   Preparation for code review.
*
*H-*/

/*C+ CheckBoxChoice
*
*    DESCRIPTION:
*
*    A CheckBoxChoice object displays one ore more choices and includes
*    a box next to each choice.  When the user selects a choice, a checkmark
*    appears in the box.  More than one choice may be selected at a time.
*
*    INHERITED CLASSES:
*
*    UIObject - In addition to the regular benefit of inheritance, inheriting
*               the UIObject class allows the CheckBoxChoice to be treated
*               as a UIObject inside of handlers.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *parent) - After being supplied
*    with the parent of the CheckBoxChoice object (a ComponentDisplay),
*    this function creates the XView object.
*
*C-*/
#ifndef _CHECK_BOX_CHOICE_H
#define _CHECK_BOX_CHOICE_H

// Includes
#include "UIChoice.h"

// Class Definition
class CheckBoxChoice : public UIChoice {
public:
                CheckBoxChoice ();
                CheckBoxChoice (char *);
                CheckBoxChoice (Image &);
               ~CheckBoxChoice ();

protected:
  virtual void  createObject (UIObject *);
};

#endif /* _CHECK_BOX_CHOICE_H */
