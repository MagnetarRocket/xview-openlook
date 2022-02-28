/*H+ Slider.h
*
*    SCCS ID: @(#)Slider.h 1.2 92/05/23 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the Slider class.
*
*    AUTHOR: Joe and Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/10/91   Preparation for code review
*
*H-*/

/*C+ Slider
*
*    DESCRIPTION:
*
*    A Slider object displays a value in graphical form and allows
*    a user to change a value by moving the slider bar to the value 
*    required.  A function can be written that will be triggered any 
*    time the user inputs a new value.  
*
*    INHERITED CLASSES:
*
*    Gauge - Provides basic capabilities to display a value in a 
*    graphical form, but does not allow the user to input values.
*
*    ORDINARY DATA MEMBERS:
*
*    Boolean	 displayValue - Specifies whether or not the slider value
*    should be displayed.
*
*    Boolean	 displayEndBoxes - Specifies whether or not end boxes should
*    be displayed.
*
*    char       *minValueString - The string on the min value end of the
*    slider.
*
*    char       *maxValueString - The string on the max value end of the
*    slider.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*            int      getValue (int &) - Returns the value of the Slider.
*
*            void     setSliderWidth (int) - Sets the width of the Slider.
*            Default: XView Default.
*
*            void     setDisplayValue (Boolean) - Sets whether or not the
*            value of the Slider should be displayed numerically.  
*            Default: Value displays (in characters).
*
*            void     setDisplayEndBoxes (Boolean) - Sets whether or not 
*            the end boxes should be displayed.  Default: Not displayed.
*
*            void     setMinValueString (char *) - Sets the value of the
*            string on the min value end of the slider.
*
*            void     setMaxValueString (char *) - Sets the value of the
*            string on the max value end of the slider.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void     createObject (UIObject *) - Create XView object.
*
*    static  void     notifyProc (Panel_item, int, Event *) - Internal 
*            wrapper around the XView event proc. for displaying objects.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*            void     setSliderDefaults () - Initializes the data members.
*
*C-*/
#ifndef _SLIDER_H
#define _SLIDER_H

// Includes
#include "Gauge.h"

// Class Definition
class Slider : public Gauge {
public:
    		 Slider ();
    		 Slider (char *);
		 Slider (Image &);
		~Slider ();

    int	 	 getValue (int &);

    void	 setSliderWidth (int newWidth)
                 { sliderWidth = newWidth;
	           if (owObject)
		     if (resourceName)
		       xv_set (owObject,
			       XV_USE_DB,
			         PANEL_SLIDER_WIDTH, sliderWidth,
				 NULL,
			       NULL);
		     else
		       xv_set (owObject,
			       PANEL_SLIDER_WIDTH, sliderWidth,
			       NULL);
		 }

    void	 setDisplayValue (Boolean display)
		 { displayValue = display;
		   if (owObject) xv_set (owObject,
		                         PANEL_SHOW_VALUE, displayValue,
					 NULL);
		 }

    void	 setDisplayEndBoxes (Boolean display)
		 { displayEndBoxes = display;
		   if (owObject)
		     xv_set (owObject,
		             PANEL_SLIDER_END_BOXES, displayEndBoxes,
			     NULL);
		 }

    void         setMinValueString (char *string)
                 { if (minValueString)
                     delete minValueString;
                   if (string)
                     minValueString =
                       strcpy (new char [strlen (string) + 1], string);
                   else   
                     minValueString = (char *) NULL;
                   if (owObject) xv_set (owObject,
                                         PANEL_MIN_VALUE_STRING, minValueString,
                                         NULL);
                 }
 
    void         setMaxValueString (char *string)
                 { if (maxValueString)
                     delete maxValueString;
                   if (string)
                     maxValueString =
                       strcpy (new char [strlen (string) + 1], string);
                   else   
                     maxValueString = (char *) NULL;
                   if (owObject) xv_set (owObject,
                                         PANEL_MAX_VALUE_STRING, maxValueString,
                                         NULL);
                 }
 
protected:
    Boolean	 displayValue;
    Boolean	 displayEndBoxes;
    char	*minValueString;
    char	*maxValueString;
    
    virtual void createObject (UIObject *);
    static  void notifyProc (Panel_item, int, Event *);

private:
    void		 setSliderDefaults ();
};

#endif /* _SLIDER_H */
