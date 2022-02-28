/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#ifndef _PROPERTIESWINDOW_H
#define _PROPERTIESWINDOW_H

#include "Button.h"
#include "ComponentDisplay.h"
#include "PopUpWindow.h"
#include "UIComponent.h"

enum NotifyType {APPLY, RESET, PROPERTY};

class PropertiesWindow : public PopUpWindow {
public:
  PropertiesWindow ()
    {
      initializeWindow ();
    }
  PropertiesWindow (char *window_label) : PopUpWindow (window_label)
    {
      initializeWindow ();
    }
  ~PropertiesWindow () { if (propertyName) delete propertyName; }

  void addProperty (char *name, UIComponent &property)
    {
      // Set the internal notify handler
      property.setNotifyHandler ((UICHandlerFunction) propertyHandler);

      // Store the PropertiesWindow pointer and the property name
      property.setObjectData ("PropertiesWindow", this);
      property.setObjectData ("PropertyName", (Generic *) name);

      // Add the property to the property display
      propertyDisplay.addComponent (property);
      setObjectData (name, property);
    }

  UIComponent* &getProperty (char *name)
    {
      return (UIComponent* &) getObjectData (name);
    }

  UIComponent  *traverseProperties (Boolean, char*&);

  void          setNotifyHandler (UICHandlerFunction handler)
    { notifyHandler = handler; }

  NotifyType    getNotifyType () { return notifyType; }
  char         *getPropertyName () { return propertyName; }

protected:
  void                setNotifyType (NotifyType type) { notifyType = type; }
  void                setPropertyName (char *name)
    {
      if (propertyName) delete propertyName;
      if (name)
	propertyName = strcpy (new char [strlen (name) + 1], name);
      else
	propertyName = (char *) NULL;
    }

  ComponentDisplay    propertyDisplay;
  Button              applyButton;
  Button              resetButton;
  NotifyType          notifyType;
  char               *propertyName;
  UICHandlerFunction  notifyHandler;

  virtual void  createObject (UIObject *parent = (UIObject *) NULL);

private:
  void initializeWindow ()
    {
      // Initialize the PropertiesWindow attributes
      notifyType = (NotifyType) UIC_NOT_SET;
      propertyName = (char *) NULL;

      // Initialize the property display attributes and add it to the window
      propertyDisplay.setX (0);
      propertyDisplay.setY (0);
      propertyDisplay.setExtendToEdge (WIDTH);
      propertyDisplay.setExtendToEdge (HEIGHT);
      addDisplay (propertyDisplay);

      // Initialize the Apply button attributes and add it to the display
      applyButton.setLabel ("Apply");
      applyButton.setNotifyHandler ((UICHandlerFunction) propertyHandler);
      applyButton.setObjectData ("PropertiesWindow", this);
      propertyDisplay.addComponent (applyButton);
      setObjectData ("ApplyButton", applyButton);

      // Initialize the Reset button attributes and add it to the display
      resetButton.setLabel ("Reset");
      resetButton.setNotifyHandler ((UICHandlerFunction) propertyHandler);
      resetButton.setObjectData ("PropertiesWindow", this);
      propertyDisplay.addComponent (resetButton);
      setObjectData ("ResetButton", resetButton);
    }

  static void propertyHandler (UIObject *);
};

#endif /* _PROPERTIESWINDOW_H */
