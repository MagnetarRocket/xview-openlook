/*H+ Gauge.cc
*
*    SCCS ID: @(#)Gauge.cc 1.2 92/05/23 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the Gauge class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       05/10/91   Preparation for code review. 
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)Gauge.cc 1.2 92/05/23 SMI"; 
#endif

// Includes
#include "Gauge.h"

/*F+ Gauge::Gauge ()
*
*    PURPOSE:
*
*    Gauge constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Gauge::Gauge ()
{
  setGaugeDefaults ();
}

/*F+ Gauge::Gauge (char *string_label)
*
*    PURPOSE:
*
*    Gauge constructor.
*
*    PARAMETERS:
*
*    string_label - Specifies a string for the label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Gauge::Gauge (char *string_label) : UIComponent (string_label)
{
  setGaugeDefaults ();
}

/*F+ Gauge::Gauge (Image &image_label)
*
*    PURPOSE:
*
*    Gauge constructor.
*
*    PARAMETERS:
*
*    image_label - Specifies an image for the label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Gauge::Gauge (Image &image_label) : UIComponent (image_label)
{
  setGaugeDefaults ();
}

/*F+ void Gauge::setGaugeDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the Gauge class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Gauge::setGaugeDefaults ()
{
  value = UIC_NOT_SET;
  valueSet = FALSE;
  minValue = UIC_NOT_SET;
  minValueSet = FALSE;
  maxValue = UIC_NOT_SET;
  maxValueSet = FALSE;
  numTicks = UIC_NOT_SET;
  minTickString = (char *) NULL;
  maxTickString = (char *) NULL;
  displayRange = TRUE;
  orientation = HORIZONTAL;
  sliderWidth = UIC_NOT_SET;
}

/*F+ Gauge::~Gauge ()
*
*    PURPOSE:
*
*    Gauge destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Gauge::~Gauge ()
{
  if (minTickString) delete minTickString;
  if (maxTickString) delete maxTickString;
}

/*F+ void Gauge::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the Gauge XView object.
*
*    PARAMETER:
*
*    parent - The parent of the Gauge (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Gauge::createObject (UIObject *parent)
{
  // Create an XView gauge taking into account font settings
  createWithFonts (parent, PANEL_GAUGE, NULL);
  
  // Set common XView attrbitutes
  setUIComponentAttributes (parent);
  
  if (sliderWidth != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_GAUGE_WIDTH, sliderWidth, NULL, NULL);
    else
      xv_set (owObject, PANEL_GAUGE_WIDTH, sliderWidth, NULL);
  }
  
  if (orientation == VERTICAL)
    xv_set (owObject, PANEL_DIRECTION, PANEL_VERTICAL, NULL);
  
  if (labelPosition == ABOVE)
    xv_set (owObject, PANEL_LAYOUT, PANEL_VERTICAL, NULL);
  
  if (numTicks != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_TICKS, numTicks, NULL, NULL);
    else
      xv_set (owObject, PANEL_TICKS, numTicks, NULL);
  }

  if (valueSet)
    xv_set (owObject, PANEL_VALUE, value, NULL);

  if (minValueSet) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_MIN_VALUE, minValue, NULL, NULL);
    else
      xv_set (owObject, PANEL_MIN_VALUE, minValue, NULL);
  }

  if (maxValueSet) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_MAX_VALUE, maxValue, NULL, NULL);
    else
      xv_set (owObject, PANEL_MAX_VALUE, maxValue, NULL);
  }

  xv_set (owObject, PANEL_SHOW_RANGE, displayRange, NULL);

  if (minTickString)
    xv_set (owObject, PANEL_MIN_TICK_STRING, minTickString, NULL);
  if (maxTickString)
    xv_set (owObject, PANEL_MAX_TICK_STRING, maxTickString, NULL);
}
       
#ifdef TEST
#endif /* TEST */
