/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "PropertiesWindow.h"

// Allow the developer to traverse the properties without knowing the name
UIComponent *PropertiesWindow::traverseProperties (Boolean reset_flag,
						   char* &name)
{
  UIComponent *property;

  // Get the next property - The Apply and Reset buttons are NOT properties
  while (property = (UIComponent *) traverseObjectData (reset_flag, name))
    if (strcmp (name, "ApplyButton") && strcmp (name, "ResetButton"))
      break;

  return property;
}

// Create the PropertiesWindow XView object
void PropertiesWindow::createObject (UIObject *parent)
{
  // Get the height and width of the properties window
  int h = getHeight ();
  int w = getWidth ();

  // Put the Apply button just left of the middle and near the bottom
  applyButton.setX (w / 2 - 65);
  applyButton.setY (h - 20);

  // Put the Reset button just right of the middle and near the bottom
  resetButton.setX (w / 2 + 20);
  resetButton.setY (h - 20);

  // Make sure the window comes up pinned
  setPinned (TRUE);

  // Create the PopUpWindow XView object
  PopUpWindow::createObject (parent);
}

// The notify handler for all of the properties and the Apply amd Reset buttons
void PropertiesWindow::propertyHandler (UIObject *property)
{
  /* Obtain the pointer to this object that was previously stored in the
     object data of each property */
  PropertiesWindow *object = (PropertiesWindow *)
			     property->getObjectData ("PropertiesWindow");

  NotifyType type;

  /* Determine if the "property" parameter is really a property, or just
     an Apply or Reset button. */
  if (property == object->getObjectData ("ApplyButton"))
    type = APPLY;
  else if (property == object->getObjectData ("ResetButton"))
    type = RESET;
  else
    type = PROPERTY;

  /* Set the notify type so that the developer can retrieve the notify
     type from developer specified handler. */
  object->setNotifyType (type);
  if (type == PROPERTY)
    // If the notify type is PROPERTY, get the property name
    object->setPropertyName ((char *) property->getObjectData ("PropertyName"));
  else
    object->setPropertyName ((char *) NULL);

  // Call the developer specified notify handler
  if (object->notifyHandler)
    (*object->notifyHandler) (object);
}
