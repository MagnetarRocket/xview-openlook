/*H+ NumericInput.h
*
*    SCCS ID: @(#)NumericInput.h 1.2 92/05/23 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the NumericInput class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ NumericInput
*
*    DESCRIPTION:
*
*    A NumericInput object displays text and allows the user to enter any
*    number of numeric characters (integer value).  The user may click
*    on increment and decrement arrows to change the integer value.
*
*    INHERITED CLASSES:
*
*    UIInputTextItem - All input text items, numeric or Numeric,
*                      inherit this.  UIInputTextItem is used to
*                      provide a foundationof functionality specific
*                      to input text items.
*
*    ORDINARY DATA MEMBERS:
*
*    int     value;	  - The initial value of item.
*    Boolean valueSet;    - Specifies an initial value has been set.
*    int     minValue;    - The minimum value of the item.
*    Boolean minValueSet; - Specifies that a minimum value has been set.
*    int     maxValue;    - The maximum value of the item.
*    Boolean maxValueSet; - Specifies that a maximum value has been set.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    int   getValue (int&)
*          - Gets the value of the numeric field of the Numeric object.
*    void  setValue (int)
*          - Sets the value of the numeric field of the Numeric object.
*            Default: XView default.
*
*    void  setMinValue (int min)
*    void  setMaxValue (int max)
*    void  setMinAndMaxValues (int min, int max)
*          - Allows setting of the minimum and/or maximum values allowed by the
*            Numeric object.  Default: XView defaults.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *)
*          - Creates the XView object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setNumericInputDefaults ()
*         - Initializes data members.
*
*C-*/
#ifndef _NUMERIC_INPUT_H
#define _NUMERIC_INPUT_H

// Includes
#include "UIInputTextItem.h"

// Class Definition
class NumericInput : public UIInputTextItem {
public:
    		 NumericInput ();
    		 NumericInput (char *);
 		 NumericInput (Image &);
		~NumericInput ();

    int	 	 getValue (int &);

    void	 setValue (int new_value)
		 { value = new_value;
	           valueSet = TRUE;
		   if (owObject)
		     xv_set (owObject, PANEL_VALUE, value, NULL); }

    void	 setMinValue (int min)
		 { minValue = min;
		   minValueSet = TRUE;
		   if (owObject) {
		     if (resourceName)
		       xv_set (owObject,
			       XV_USE_DB,
			         PANEL_MIN_VALUE, minValue,
			         NULL,
			       NULL);
		     else
		       xv_set (owObject, PANEL_MIN_VALUE, minValue, NULL);
		   }
		 }

    void	 setMaxValue (int max)
		 { maxValue = max;
		   maxValueSet = TRUE;
		   if (owObject) {
		     if (resourceName)
		       xv_set (owObject,
			       XV_USE_DB,
			         PANEL_MAX_VALUE, maxValue,
			         NULL,
			       NULL);
		     else
		       xv_set (owObject, PANEL_MAX_VALUE, maxValue, NULL);
		   }
		 }

    void	 setMinAndMaxValues (int min, int max)
		 { setMinValue (min);
		   setMaxValue (max); }

protected:    
    int		 value;
    Boolean	 valueSet;
    int		 minValue;
    Boolean	 minValueSet;
    int		 maxValue;
    Boolean	 maxValueSet;

    virtual void createObject (UIObject *);

private:
    void	 setNumericInputDefaults ();
};

#endif /* _NUMERIC_INPUT_H */
