/*H+ Gauge.h
*
*    SCCS ID: @(#)Gauge.h 1.2 92/05/23 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the Gauge class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/09/91   Preparation for code review
*
*H-*/

/*C+ Gauge
*
*    DESCRIPTION:
*
*    A Gauge object horizontally or vertically displays a value 
*    in graphical form.  It resembles a thermometer and has a high 
*    and low value that the user can associate with  Tick marks can 
*    be added to the graphic to clarify the exact value for the user.
*
*    INHERITED CLASSES:
*
*    UIComponent - Defines the generic characteristics of
*    components in all GUI applications.
*
*    ORDINARY DATA MEMBERS:
*
*    int	   value - the current value of the Gauge that 
*    is being displayed.
*
*    Boolean	   valueSet - TRUE if the above value is set.
*
*    int	   minValue - the Gauge' s minimum value.
*
*    Boolean	   minValueSet - TRUE if the Gauge' s minimum
*    value is set.
*
*    int	   maxValue - the Gauge' s maximum value.
*
*    Boolean	   maxValueSet - TRUE if the Gauge' s maximum 
*    value is set.
*
*    int	   numTicks - Number of tick marks to be displayed.
*
*    char         *minTickString - The string underneath the min tick
*                                  mark.
*
*    char         *maxTickString - The string underneath the max tick
*                                  mark.
*
*    int	   gaugeWidth - the width of the Gauge.
*
*    int	   sliderWidth - the width of the slider associated 
*    with this Gauge.
*
*    Boolean	   displayRange - TRUE if the range to be displayed.
*
*    Orientation   orientation - Horizontal or vertical.
*    
*    PUBLIC MEMBER FUNCTIONS:
*
*            void   setTicks (int) - Sets the number of tick marks
*            to use on the Gauge.  Default: No tick marks displayed.
*
*	     void   setMinTickString (char *) - Sets the value of the
*	     string underneath the min tick mark.
*
*	     void   setMaxTickString (char *) - Sets the value of the
*	     string underneath the max tick mark.
*
*            void   setValue (int) - Changes the current value of 
*            the Gauge and redisplays it. Default: XView default.
*
*            void   setMinValue (int) - Sets the minimum value of the 
*            Gauge.  Default: XView default.
*
*            void   setMaxValue (int) - Sets the maximum value of the
*            Gauge.  Default: XView default.
*
*            void   setGaugeWidth (int) - Sets the width of the Gauge.
*
*            void   setMinAndMaxValues (int, int) - Sets the minimum 
*            and maximum value of the Gauge.  Default: XView default.
*
*            void   setOrientation (Orientation) - Sets Orientation 
*            (HORIZONTAL and VERTICAL) of this Gauge.  Default: 
*            HORIZONTAL.
*
*            void   setDisplayRange (Boolean) - Sets whether or not 
*            the range should be displayed.  Default: Range displayed.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void   createObject (UIObject *) - Create XView object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void	   setGaugeDefaults () - Initializes data members.
*
*C-*/
#ifndef _GAUGE_H
#define _GAUGE_H

// Includes
#include "UIComponent.h"

// Class Definition
class Gauge : public UIComponent {
public:
    		 Gauge ();
    		 Gauge (char *);
		 Gauge (Image &);
		~Gauge ();

    void	 setTicks (int num_ticks)
		 { numTicks = num_ticks;
		   if (owObject) {
		     if (resourceName)
		       xv_set (owObject,
			       XV_USE_DB,
				 PANEL_TICKS, numTicks,
				 NULL,
			       NULL);
		     else
		       xv_set (owObject,
			       PANEL_TICKS, numTicks,
			       NULL);
		   }
		 }

    void         setMinTickString (char *string) 
                 { if (minTickString) 
                     delete minTickString; 
                   if (string)
                     minTickString =
                       strcpy (new char [strlen (string) + 1], string);
                   else 
                     minTickString = (char *) NULL; 
                   if (owObject) xv_set (owObject, 
                                         PANEL_MIN_TICK_STRING, minTickString, 
                                         NULL); 
                 } 

    void         setMaxTickString (char *string)
                 { if (maxTickString)
                     delete maxTickString;
                   if (string)
                     maxTickString =
                       strcpy (new char [strlen (string) + 1], string);
                   else
                     maxTickString = (char *) NULL;
                   if (owObject) xv_set (owObject,
                                         PANEL_MAX_TICK_STRING, maxTickString,
                                         NULL);
                 }
 
    void	 setValue (int new_value)
		 { value = new_value;
		   valueSet = TRUE;
		   if (owObject) xv_set (owObject, PANEL_VALUE, value, NULL); }

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
		       xv_set (owObject,
			       PANEL_MIN_VALUE, minValue,
			       NULL);
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
		       xv_set (owObject,
			       PANEL_MAX_VALUE, maxValue,
			       NULL);
		   }
		 }

    void	 setGaugeWidth (int newWidth)
		 { sliderWidth = newWidth;
		   if (owObject) {
		     if (resourceName)
		       xv_set (owObject,
			       XV_USE_DB,
			         PANEL_GAUGE_WIDTH, sliderWidth,
			         NULL,
			       NULL);
		     else
		       xv_set (owObject,
			       PANEL_GAUGE_WIDTH, sliderWidth,
			       NULL);
		   }
		 }

    void	 setMinAndMaxValues (int min, int max)
		 { setMinValue (min);
		   setMaxValue (max); }

    void	 setOrientation (Orientation direction)
		 { orientation = direction;
		   if (owObject) {
		     if (direction == HORIZONTAL)
		       xv_set (owObject,
			       PANEL_DIRECTION, PANEL_HORIZONTAL,
			       NULL);
		     else
		       xv_set (owObject,
			       PANEL_DIRECTION, PANEL_VERTICAL,
			       NULL);
		   }
		 }

    void	 setDisplayRange (Boolean display_range)
		 { displayRange = display_range;
		   if (owObject) xv_set (owObject,
					 PANEL_SHOW_RANGE, displayRange,
					 NULL); }

protected:
    int		 value;
    Boolean	 valueSet;
    int		 minValue;
    Boolean	 minValueSet;
    int		 maxValue;
    Boolean	 maxValueSet;
    int		 numTicks;
    char        *maxTickString;
    char	*minTickString;
    int		 gaugeWidth;
    int		 sliderWidth;
    Boolean	 displayRange;
    Orientation	 orientation;

    virtual void createObject (UIObject *);

private:    
    void	 setGaugeDefaults ();
};

#endif /* _GAUGE_H */
