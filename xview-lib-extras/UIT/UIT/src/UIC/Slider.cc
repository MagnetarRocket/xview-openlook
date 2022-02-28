/*H+ Slider.cc
*
*    SCCS ID: @(#)Slider.cc 1.3 92/05/23 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the Slider class.
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
static char sccsid [] = "@(#)Slider.cc 1.3 92/05/23 SMI"; 
#endif

// Includes
#include "Slider.h"

/*F+ Slider::Slider ()
*
*    PURPOSE:
*
*    Slider constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Slider::Slider ()
{
  setSliderDefaults ();
}

/*F+ Slider::Slider (char *string_label)
*
*    PURPOSE:
*
*    Slider constructor.
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
Slider::Slider (char *string_label) : Gauge (string_label)
{
  setSliderDefaults ();
}

/*F+ Slider::Slider (Image &image_label)
*
*    PURPOSE:
*
*    Slider constructor.
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
Slider::Slider (Image &image_label) : Gauge (image_label)
{
  setSliderDefaults ();
}

/*F+ void Slider::setSliderDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the Slider class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Slider::setSliderDefaults ()
{
  displayValue = TRUE;
  displayEndBoxes = FALSE;
  minValueString = (char *) NULL;
  maxValueString = (char *) NULL;
  notifyHandler = (UICHandlerFunction) NULL;
}

/*F+ Slider::~Slider ()
*
*    PURPOSE:
*
*    Slider destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Slider::~Slider ()
{
  if (minValueString) delete minValueString;
  if (maxValueString) delete maxValueString;
}

/*F+ int Slider::getValue (int &return_value)
*
*    PURPOSE:
*
*    Returns the current value of the Slider.
*
*    PARAMETERS:
*
*    return_value - The current value of the Slider.
*
*    RETURN VALUES:
*
*    The current value of the Slider is returned.  If the XView object
*    has not been created and a value has not been specified, then
*    UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int Slider::getValue (int &return_value)
{
  return_value = value;

  if (owObject)
    return_value = (int) xv_get (owObject, PANEL_VALUE);

  return (return_value);
}

/*F+ void Slider::notifyProc (Panel_item item, int value, Event *event)
*
*    PURPOSE:
*
*    Internal wrapper for the XView slider notify proc.  When the slider's
*    value changes, XView calls this function.  This function calls
*    the user specified handler.
*
*    PARAMETERS:
*
*    item  - The XView object the notify occurred on.
*    value - The value of the XView object.
*    event - The XView event information structure.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Slider::notifyProc (Panel_item item, int value, Event *event)
{
  Slider *object = (Slider *) xv_get (item, XV_KEY_DATA, UIC_OBJECT_KEY);

  // Set the internal UIC event information
  object->setEventInformation (event);

  // Notify the user specified notify targets
  object->notifyNotifyTargets ();

  // Call the user specified notify handler
  if (object->useDefaultNotifyHandlerFlag) 
    object->defaultNotifyHandler (); 
  else if (object->notifyHandler)
    (*object->notifyHandler) (object);
  
  // Erase the internal UIC event information
  object->resetEventInformation ();

  // If the user specified a notify status
  if (object->notifyStatus != UIC_NOT_SET) {
    // Set the notify status
    xv_set (item, PANEL_NOTIFY_STATUS, object->notifyStatus, NULL);
 
    // Reset the internal notify status
    object->notifyStatus = UIC_NOT_SET;
  }
}

/*F+ void Slider::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView Slider object.
*
*    PARAMETER:
*
*    parent - The parent of the slider (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Slider::createObject (UIObject *parent)
{
  /* Create an XView slider taking into account font settings

     NOTE: Because of a bug in XView V3, the X and Y location of the
    	   Slider must be set in the xv_create call. */
  if ((xLoc != UIC_NOT_SET) && (yLoc != UIC_NOT_SET)) {
    if (resourceName)
      createWithFonts (parent, PANEL_SLIDER,
		       (void *) XV_INSTANCE_NAME, (void *) resourceName,
		       (void *) XV_USE_DB,
  		         (void *) XV_X, (void *) xLoc,
		         (void *) XV_Y, (void *) yLoc,
		         NULL,
		       NULL);
    else
      createWithFonts (parent, PANEL_SLIDER,
  		       (void *) XV_X, (void *) xLoc,
		       (void *) XV_Y, (void *) yLoc,
		       NULL);
  }
  else if (xLoc != UIC_NOT_SET) {
    if (resourceName)
      createWithFonts (parent, PANEL_SLIDER,
		       (void *) XV_INSTANCE_NAME, (void *) resourceName,
		       (void *) XV_USE_DB,
		         (void *) XV_X, (void *) xLoc,
		         NULL,
		       NULL);
    else
      createWithFonts (parent, PANEL_SLIDER,
		       (void *) XV_X, (void *) xLoc,
		       NULL);
  }
  else if (yLoc != UIC_NOT_SET) {
    if (resourceName)
      createWithFonts (parent, PANEL_SLIDER,
		       (void *) XV_INSTANCE_NAME, (void *) resourceName,
		       (void *) XV_USE_DB,
		         (void *) XV_Y, (void *) yLoc,
		         NULL,
		       NULL);
    else
      createWithFonts (parent, PANEL_SLIDER,
		       (void *) XV_Y, (void *) yLoc,
		       NULL);
  }
  else
    createWithFonts (parent, PANEL_SLIDER, NULL);

  // Set common XView attrbitutes
  setUIComponentAttributes (parent);

  xv_set (owObject, PANEL_NOTIFY_PROC, &Slider::notifyProc, NULL);

  if (sliderWidth != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject,
	      XV_USE_DB,
	        PANEL_SLIDER_WIDTH, sliderWidth,
	        NULL,
	      NULL);
    else
      xv_set (owObject, PANEL_SLIDER_WIDTH, sliderWidth, NULL);
  }
    
  if (orientation == VERTICAL)
    xv_set (owObject, PANEL_DIRECTION, PANEL_VERTICAL, NULL);
  
  if (labelPosition == ABOVE)
    xv_set (owObject, PANEL_LAYOUT, PANEL_VERTICAL, NULL);
  
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

  if (numTicks != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_TICKS, numTicks, NULL, NULL);
    else
      xv_set (owObject, PANEL_TICKS, numTicks, NULL);
  }

  xv_set (owObject, PANEL_SHOW_RANGE, displayRange, NULL);
  xv_set (owObject, PANEL_SHOW_VALUE, displayValue, NULL);
  xv_set (owObject, PANEL_SLIDER_END_BOXES, displayEndBoxes, NULL);

  if (minValueString)
    xv_set (owObject, PANEL_MIN_VALUE_STRING, minValueString, NULL);
  if (maxValueString)
    xv_set (owObject, PANEL_MAX_VALUE_STRING, maxValueString, NULL);

  if (minTickString)
    xv_set (owObject, PANEL_MIN_TICK_STRING, minTickString, NULL);
  if (maxTickString) 
    xv_set (owObject, PANEL_MAX_TICK_STRING, maxTickString, NULL); 
}
       
#ifdef TEST
#endif /* TEST */
