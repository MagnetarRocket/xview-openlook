/*H+ UIChoice.cc
*
*    SCCS ID: @(#)UIChoice.cc 1.6 93/06/01 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the UIChoice class.
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
static char sccsid [] = "@(#)UIChoice.cc 1.6 93/06/01 SMI"; 
#endif

// Includes
#include "CharacterFont.h"
#include "ColorMap.h"
#include "UIChoice.h"

/*F+ UIChoice::UIChoice ()
*
*    PURPOSE:
*
*    Constructor of the UIChoice class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIChoice::UIChoice ()
{
  setUIChoiceDefaults ();
}

/*F+ UIChoice::UIChoice (char *string_label)
*
*    PURPOSE:
*
*    Constructor of the UIChoice class.
*
*    PARAMETERS:
*
*    string_label - Specifies a string for the UIChoice label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIChoice::UIChoice (char *string_label) : UIComponent (string_label)
{
  setUIChoiceDefaults ();
}

/*F+ UIChoice::UIChoice (Image &image_label)
*
*    PURPOSE:
*
*    Constructor of the UIChoice class.
*
*    PARAMETERS:
*
*    image_label - Specifies an image for the UIChoice label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIChoice::UIChoice (Image &image_label) : UIComponent (image_label)
{
  setUIChoiceDefaults ();
}

/*F+ UIChoice::setUIChoiceDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the UIChoice class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setUIChoiceDefaults ()
{
  selectionRequired = TRUE;
  multipleChoice = FALSE;
  numColumns = UIC_NOT_SET;
  numRows = UIC_NOT_SET;
  choiceFont = (CharacterFont *) NULL;
  lastSelected = UIC_NOT_SET;
}

/*F+ UIChoice::~UIChoice ()
*
*    PURPOSE:
*
*    Destructor of the UIChoice class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
UIChoice::~UIChoice ()
{
  selections.dropItems ();
  choiceColors.dropItems ();
  choices.dropItems ();
  images.dropItems ();
}

/*F+ void UIChoice::setChoiceColor (int choice_index, Color color)
*
*    PURPOSE:
*
*    Sets the color of the specified choice item.
*
*    PARAMETERS:
*
*    choice_index - The index of the choice.
*    color        - A UIC color structure containing the color's RGB values.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setChoiceColor (int choice_index, Color color)
{
  if ((choice_index > 0) && (choice_index <= choiceColors.getSize ())) {
    if (!colorMap) useDefaultColorMap ();
    
    choiceColors [choice_index - 1] = (Generic *) colorMap->findColor (color);

    if (owObject) {
      initializeColorMap (serverParent, TRUE);
      xv_set (owObject,
	      PANEL_CHOICE_COLOR, choice_index-1,
	                          (int) choiceColors [choice_index-1],
	      NULL);
    }
  }
}
 
/*F+ void UIChoice::setChoiceColor (int choice_index, char *color)
*
*    PURPOSE:
*
*    Sets the color of the specified choice item.
*
*    PARAMETERS:
*
*    choice_index - The index of the choice.
*    color        - The name of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setChoiceColor (int choice_index, char *color)
{
  if ((choice_index > 0) && (choice_index <= choiceColors.getSize ())) {
    if (!colorMap) useDefaultColorMap ();
    
    choiceColors [choice_index - 1] = (Generic *) colorMap->findColor (color);

    if (owObject) {
      initializeColorMap (serverParent, TRUE);
      xv_set (owObject,
	      PANEL_CHOICE_COLOR, choice_index-1,
	                          (int) choiceColors [choice_index-1],
	      NULL);
    }
  }
}
 
/*F+ void UIChoice::setChoiceColor (char *choice_value, Color color)
*
*    PURPOSE:
*
*    Sets the color of the specified choice item.
*
*    PARAMETERS:
*
*    choice_value - The string value of the choice.
*    color        - A UIC color structure containing the color's RGB values.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setChoiceColor (char *choice_value, Color color)
{
  int num_choices = choices.getSize ();
  
  for (int i = 0; i < num_choices ; i++) {
    char *temp_string = (char *) choices [i];
    if (strcmp (temp_string, choice_value) == 0) {
      if (!colorMap) useDefaultColorMap ();

      choiceColors [i] = (Generic *) colorMap->findColor (color);

      if (owObject) {
        initializeColorMap (serverParent, TRUE);
        xv_set (owObject,
		PANEL_CHOICE_COLOR, i, (int) choiceColors [i],
		NULL);
      }

      return;
    }
  }
}
 
/*F+ void UIChoice::setChoiceColor (char *choice_value, char *color)
*
*    PURPOSE:
*
*    Sets the color of the specified choice item.
*
*    PARAMETERS:
*
*    choice_value - The string value of the choice.
*    color        - The name of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setChoiceColor (char *choice_value, char *color)
{
  int num_choices = choices.getSize ();
  
  for (int i = 0; i < num_choices ; i++) {
    char *temp_string = (char *) choices [i];
    if (strcmp (temp_string, choice_value) == 0) {
      if (!colorMap) useDefaultColorMap ();

      choiceColors [i] = (Generic *) colorMap->findColor (color);

      if (owObject) {
        initializeColorMap (serverParent, TRUE);
        xv_set (owObject,
		PANEL_CHOICE_COLOR, i, (int) choiceColors [i],
		NULL);
      }

      return;
    }
  }
}
 
/*F+ void UIChoice::setChoiceColor (Image &choice_value, Color color)
*
*    PURPOSE:
*
*    Sets the color of the specified choice item.
*
*    PARAMETERS:
*
*    choice_value - The image value of the choice.
*    color        - A UIC color structure containing the color's RGB values.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setChoiceColor (Image &choice_value, Color color)
{
  if (!colorMap) useDefaultColorMap ();
  
  int choice_index = images.findItem ((Generic *) &choice_value);
  choiceColors [choice_index] = (Generic *) colorMap->findColor (color);

  if (owObject) {
    initializeColorMap (serverParent, TRUE);
    xv_set (owObject,
	    PANEL_CHOICE_COLOR, choice_index,
	                        (int) choiceColors [choice_index],
	    NULL);
  }
}
 
/*F+ void UIChoice::setChoiceColor (Image &choice_value, char *color)
*
*    PURPOSE:
*
*    Sets the color of the specified choice item.
*
*    PARAMETERS:
*
*    choice_value - The image value of the choice.
*    color        - The name of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setChoiceColor (Image &choice_value, char *color)
{
  if (!colorMap) useDefaultColorMap ();
  
  int choice_index = images.findItem ((Generic *) &choice_value);
  choiceColors [choice_index] = (Generic *) colorMap->findColor (color);

  if (owObject) {
    initializeColorMap (serverParent, TRUE);
    xv_set (owObject,
	    PANEL_CHOICE_COLOR, choice_index,
	                        (int) choiceColors [choice_index],
	    NULL);
  }
}

/*F+ void UIChoice::deleteChoice (int choice_index)
*
*    PURPOSE:
*
*    Deletes the specified row from the choice object.
*
*    PARAMETERS:
*
*    choice_index - The number of the row to delete.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::deleteChoice (int choice_index)
{
  if (!owObject) {
    int num_choices = choices.getSize ();
    
    if ((choice_index <= num_choices) && (choice_index > 0)) { 
      // Make sure the choice is not in a selected state
      setSelectedChoice (choice_index, FALSE);
      
      // Update the choices in the selections list
      updateSelections (choice_index, -1);

      choice_index--;
      
      (void) images.removeItem (choice_index);
      (void) choiceColors.removeItem (choice_index);

      char *string_entry = (char *) choices.removeItem (choice_index);
      if (string_entry) delete string_entry;
    }
  }
}

/*F+ void UIChoice::deleteChoice (char *entry)
*
*    PURPOSE:
*
*    Deletes the first row with the specified string value.
*
*    PARAMETERS:
*
*    entry - The string value of the row to delete.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::deleteChoice (char *entry)
{
  if (!owObject) {
    int num_choices = choices.getSize ();
    
    for (int choice_index = num_choices-1; choice_index > -1; choice_index--) {
      char *temp_string = (char *) choices [choice_index];
      
      // If this is the item of interest...
      if (temp_string && !strcmp (temp_string, entry)) {
	// Make sure the choice is not in a selected state
	setSelectedChoice (choice_index+1, FALSE);
      
	// Update the choices in the selections list
	updateSelections (choice_index+1, -1);

	(void) images.removeItem (choice_index);
	(void) choiceColors.removeItem (choice_index);
	
	char *string_entry = (char *) choices.removeItem (choice_index);
	if (string_entry) delete string_entry;
	break;
      }
    }
  }
}

/*F+ void UIChoice::deleteChoice (Image &entry)
*
*    PURPOSE:
*
*    Deletes the first row with the specified image value.
*
*    PARAMETERS:
*
*    entry - The image value of the row to delete.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::deleteChoice (Image &entry)
{
  if (!owObject) {
    int num_choices = choices.getSize ();
    
    for (int choice_index = num_choices-1; choice_index > -1; choice_index--) {
      Image *temp_image = (Image *) images [choice_index];
      
      // If this is the item of interest...
      if (temp_image == &entry) {
	// Make sure the choice is not in a selected state
	setSelectedChoice (choice_index+1, FALSE);
      
	// Update the choices in the selections list
	updateSelections (choice_index+1, -1);

	(void) images.removeItem (choice_index);
	(void) choiceColors.removeItem (choice_index);
	
	char *string_entry = (char *) choices.removeItem (choice_index);
	if (string_entry) delete string_entry;
	break;
      }
    }
  }
}

/*F+ int UIChoice::addChoice (char *entry)
*
*    PURPOSE:
*
*    Adds a row with the specified value to the end of the choice object.
*
*    PARAMETERS:
*
*    entry - The string value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the choice object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIChoice::addChoice (char *entry)
{
  if (!owObject) { 
    char *temp_string = strcpy (new char [strlen (entry) + 1], entry);

    choices.addItem ((Generic *) temp_string);
    images.addItem ((Generic *) NULL);
    choiceColors.addItem ((Generic *) UIC_NOT_SET);

    return (choices.getSize ());
  }
  
  return (UIC_NOT_SET);
}

/*F+ int UIChoice::addChoice (Image &entry)
*
*    PURPOSE:
*
*    Adds a row with the specified value to the end of the choice object.
*
*    PARAMETERS:
*
*    entry - The image value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the choice object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIChoice::addChoice (Image &entry)
{
  if (!owObject) { 
    images.addItem ((Generic *) &entry);
    choices.addItem ((Generic *) NULL);
    choiceColors.addItem ((Generic *) UIC_NOT_SET);

    return (images.getSize ());
  }

  return (UIC_NOT_SET);
}
    
/*F+ int UIChoice::addChoice (int choice_index, char *entry)
*
*    PURPOSE:
*
*    Inserts a row with the specified value at the specified position
*    in the choice object.
*
*    PARAMETERS:
*
*    choice_index - The position of the new row.
*    entry        - The string value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the choice object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIChoice::addChoice (int choice_index, char *entry)
{
  if (!owObject) { 
    int num_choices = choices.getSize ();
    
    // If the row number is greater then the number of items in the list...
    if (choice_index > num_choices)
      // Add the item to the end of the list
      return addChoice (entry);
    else {
      choices.addItem (choices [num_choices - 1]);
      images.addItem (images [num_choices - 1]);
      choiceColors.addItem (choiceColors [num_choices - 1]);
      
      for (int i = num_choices - 1; i >= choice_index; i--) {
	choices [i] = choices [i - 1];
	images [i] = images [i - 1];
	choiceColors [i] = choiceColors [i - 1];
      }
      
      char *temp_value = strcpy (new char [strlen (entry) + 1], entry);
      
      choices [choice_index - 1] = (Generic *) temp_value;
      images [choice_index - 1] = (Generic *) NULL;
      choiceColors [choice_index - 1] = (Generic *) UIC_NOT_SET;
 
      // Update the row numbers of the pre-selected items
      updateSelections (choice_index, 1);

      return (choice_index);
    }
  }
  
  return (UIC_NOT_SET);
}

/*F+ int UIChoice::addChoice (int choice_index, Image &entry)
*
*    PURPOSE:
*
*    Inserts a row with the specified value at the specified position
*    in the choice object.
*
*    PARAMETERS:
*
*    choice_index - The position of the new row.
*    entry        - The image value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the choice object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIChoice::addChoice (int choice_index, Image &entry)
{
  if (!owObject) { 
    int num_choices = images.getSize ();
    
    // If the row number is greater then the number of items in the list...
    if (choice_index > num_choices)
      // Add the item to the end of the list
      return addChoice (entry);
    else {
      images.addItem (images [num_choices - 1]);
      choices.addItem (choices [num_choices - 1]);
      choiceColors.addItem (choiceColors [num_choices - 1]);
      
      for (int i = num_choices - 1; i >= choice_index; i--) {
	images [i] = images [i - 1];
	choices [i] = choices [i - 1];
	choiceColors [i] = choiceColors [i - 1];
      }
      
      images [choice_index - 1] = (Generic *) &entry;
      choices [choice_index - 1] = (Generic *) NULL;
      choiceColors [choice_index - 1] = (Generic *) UIC_NOT_SET;
      
      // Update the row numbers of the pre-selected items
      updateSelections (choice_index, 1); 
 
      return (choice_index);
    }
  }
  
  return (UIC_NOT_SET);
}

/*F+ void UIChoice::setChoices (char *choice, ...)
*
*    PURPOSE:
*
*    Sets all of the choice objects choices with a variable length
*    argument list of strings.
*
*    PARAMETERS:
*
*    choice - The first string in the list.
*    ...    - The rest of the choice strings.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the choice object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setChoices (char *choice, ...)
{
  va_list  ap;
  char    *entry;
  char 	  *temp_entry;
  
  if (!owObject) {
    // Get the first argument in the arg list
    va_start (ap, choice);
    temp_entry = choice;
  
    while (temp_entry) {
      entry = strcpy (new char [strlen (temp_entry) + 1], temp_entry);

      choices.addItem ((Generic *) entry);
      images.addItem ((Generic *) NULL);
      choiceColors.addItem ((Generic *) UIC_NOT_SET);

      // Get the next argument
      temp_entry = va_arg (ap, char *);
    }

    va_end (ap);
  }
}
       
/*F+ char *UIChoice::getChoice (char* &return_value)
*
*    PURPOSE:
*
*    Returns the string value associated with the last selected choice.
*
*    PARAMETERS:
*
*    return_value - The string value of the last selected item.
*
*    RETURN VALUES:
*
*    The string value of the last selected item is returned.
*    Memory to store the string is allocated on the heap and needs to be
*    deleted by the caller.  If there was no last selected item,
*    NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *UIChoice::getChoice (char *&return_value)
{
  return_value = (char *) NULL;

  if (lastSelected != UIC_NOT_SET) {
    char *temp_string = (char *) choices [lastSelected];
    return_value = strcpy (new char [strlen (temp_string) + 1], temp_string);
  }

  return (return_value);
}

/*F+ Image *UIChoice::getChoice (Image* &return_value)
*
*    PURPOSE:
*
*    Returns a pointer to the image value associated with the last choice item
*    chosen.
*
*    PARAMETERS:
*
*    return_value - A pointer to the image value of the last selected item.
*
*    RETURN VALUES:
*
*    A pointer to the image value of the last selected item is returned.
*    If there was no last selected item, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Image *UIChoice::getChoice (Image *&return_value)
{
  return_value = (Image *) NULL;

  if (lastSelected != UIC_NOT_SET)
    return_value =  (Image *) images [lastSelected];

  return (return_value);
}

/*F+ int UIChoice::getChoice (int &return_value)
*
*    PURPOSE:
*
*    Returns the row number associated with the last choice item chosen.
*
*    PARAMETERS:
*
*    return_value - The row number of the last selected item.
*
*    RETURN VALUES:
*
*    The row number of the last selected item is returned.
*    If there was no last selected item, UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIChoice::getChoice (int &return_value)
{
  return_value = UIC_NOT_SET;

  if (lastSelected != UIC_NOT_SET)
    return_value = lastSelected + 1;

  return (return_value);
}

/*F+ char *UIChoice::getChoice (int selection, char* &return_value)
*
*    PURPOSE:
*
*    Primarily used by UIChoice objects that allow multiple choice.
*    This version of getChoice returns the string value associated
*    with the selection specified out the list of current selections.
*
*    PARAMETERS:
*
*    return_value - The string value of the specified selection out of
*                   the list of selections.
*
*    RETURN VALUES:
*
*    The string value of the selection is returned.  The memory containing
*    the string is allocated off of the heap.  The caller must delete
*    memory.  If the selection specified does not exist, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *UIChoice::getChoice (int selection, char *&return_value)
{
  return_value = (char *) NULL;

  if ((selection > 0) && (selection <= selections.getSize ())) {
    int row_number = (int) selections [selection-1];
    char *temp_string = (char *) choices [row_number];

    if (temp_string)
      return_value = strcpy (new char [strlen (temp_string) + 1], temp_string);
  }

  return (return_value);
}     

/*F+ Image *UIChoice::getChoice (int selection, Image* &return_value)
*
*    PURPOSE:
*
*    Primarily used by UIChoice objects that allow multiple choice.
*    This version of getChoice returns a pointer to the image value associated
*    with the selection specified out the list of current selections.
*
*    PARAMETERS:
*
*    return_value - A pointer to the image value of the specified selection
*                   out of the list of selections.
*
*    RETURN VALUES:
*
*    A pointer to the image value of the selection is returned.  If the
*    selection specified does not exist, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Image *UIChoice::getChoice (int selection, Image *&return_value)
{
  return_value = (Image *) NULL;

  if ((selection > 0) && (selection <= selections.getSize ())) {
    int row_number = (int) selections [selection-1];
    return_value = (Image *) images [row_number];
  }

  return (return_value);
}     

/*F+ int UIChoice::getChoice (int selection, int &return_value)
*
*    PURPOSE:
*
*    Primarily used by UIChoice objects that allow multiple choice.
*    This version of getChoice returns the row number associated
*    with the selection specified out the list of current selections.
*
*    PARAMETERS:
*
*    return_value - The row number of the specified selection out of
*                   the list of selections.
*
*    RETURN VALUES:
*
*    The row number of the selection is returned.  If the selection
*    specified does not exist, UIC_NOT_SET is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIChoice::getChoice (int selection, int &return_value)
{
  return_value = UIC_NOT_SET;

  if ((selection > 0) && (selection <= selections.getSize ()))
    return_value = ((int) selections [selection-1]) + 1;

  return (return_value);
}     

/*F+ void UIChoice::replaceChoice (int row_number, char *entry)
*
*    PURPOSE:
*
*    Replaces the string value of the specified row.  The row
*    must already exist in the UIChoice object.
*
*    PARAMETERS:
*
*    row_number - The row to replace.
*    entry      - The new string value.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::replaceChoice (int row_number, char *entry)
{
  // If the row exists...
  if ((row_number > 0) && (row_number <= choices.getSize ())) {
    row_number--;
   
    // If the XView object already exists...
    if (owObject) {   
      if (choiceFont)
        xv_set (owObject,
                PANEL_CHOICE_FONT, row_number, choiceFont->getXViewObject (),
                NULL);

      xv_set (owObject, PANEL_CHOICE_STRING, row_number, entry, NULL);
    }    
    // Else, pre-XView list...
    else {
      char *temp_string = strcpy (new char [strlen (entry) + 1], entry);

      choices [row_number] = (Generic *) temp_string;
      images [row_number] = (Generic *) NULL;
    }
  }
}    

/*F+ void UIChoice::replaceChoice (int row_number, Image &entry)
*
*    PURPOSE:
*
*    Replaces the image value of the specified row.  The row
*    must already exist in the UIChoice object.
*
*    PARAMETERS:
*
*    row_number - The row to replace.
*    entry      - The new image value.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::replaceChoice (int row_number, Image &entry)
{
  // If the row exists...
  if ((row_number > 0) && (row_number <= choices.getSize ())) {
    row_number--;   
  
    // If the XView object already exists...
    if (owObject) { 
      ((UIObject *) &entry)->createObject (parentObject);
      xv_set (owObject,
              PANEL_CHOICE_IMAGE, row_number, entry.getXViewObject (),
              NULL);
    }
    // Else, pre-XView list...
    else {
      choices [row_number] = (Generic *) NULL;
      images [row_number] = (Generic *) &entry;
    }
  }
}    
 
/*F+ int UIChoice::getNumberSelected ()
*
*    PURPOSE:
*
*    Returns the number of currently selected choices.
*
*    RETURN VALUES:
*
*    Returns the number of currently selected choices.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int UIChoice::getNumberSelected ()
{
  return (selections.getSize ());
}
 
/*F+ void UIChoice::setSelectionRequired (Boolean flag)
*
*    PURPOSE:
*
*    Specifies whether or not this UIChoice object requires a selection.
*
*    PARAMETERS:
*
*    flag - TRUE if this object requires a selection.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setSelectionRequired (Boolean flag)
{
  selectionRequired = flag;

  if (owObject) {
    if (selectionRequired)
      xv_set (owObject, PANEL_CHOOSE_NONE, FALSE, NULL);
    else
      xv_set (owObject, PANEL_CHOOSE_NONE, TRUE, NULL);
  }
}  

/*F+ void UIChoice::setMultipleChoice (Boolean flag)
*
*    PURPOSE:
*
*    Specifies whether or not this UIChoice object is multiple choice.
*
*    PARAMETERS:
*
*    flag - TRUE if this object is multiple choice.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setMultipleChoice (Boolean flag)
{
  multipleChoice = flag;

  if (owObject) {
    if (multipleChoice)
      xv_set (owObject, PANEL_CHOOSE_ONE, FALSE, NULL);
    else
      xv_set (owObject, PANEL_CHOOSE_ONE, TRUE, NULL);
  }
}
 
/*F+ void UIChoice::setDisplayedColumns (int num_columns)
*
*    PURPOSE:
*
*    Specifies the number of columns to be displayed by this object.
*
*    PARAMETERS:
*
*    num_columns - The number of columns to display.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setDisplayedColumns (int num_columns)
{
  numColumns = num_columns;

  if (owObject) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_CHOICE_NCOLS, numColumns, NULL, NULL);
    else
      xv_set (owObject, PANEL_CHOICE_NCOLS, numColumns, NULL);
  }
}
 
/*F+ void UIChoice::setChoiceFont (CharacterFont &font)
*
*    PURPOSE:
*
*    Specifies the font to use for displaying choices.
*
*    PARAMETERS:
*
*    font - The font to use.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setChoiceFont (CharacterFont &font)
{
  choiceFont = &font;

  if (owObject) {
    ((UIObject *) choiceFont)->createObject (this);
    xv_set (owObject, PANEL_CHOICE_FONT, choiceFont->getXViewObject (), NULL);
  }
}
 
/*F+ void UIChoice::setDisplayedColumns (int num_rows)
*
*    PURPOSE:
*
*    Specifies the number of rows to be displayed by this object.
*
*    PARAMETERS:
*
*    num_rows - The number of rows to display.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setDisplayedRows (int num_rows)
{
  numRows = num_rows;

  if (owObject) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_CHOICE_NROWS, numRows, NULL, NULL);
    else
      xv_set (owObject, PANEL_CHOICE_NROWS, numRows, NULL);
  }
}
 
/*F+ void UIChoice::setSelectedChoice (int choice_index, Boolean flag)
*
*    PURPOSE:
*
*    Sets the selection status of the specified choice.
*
*    PARAMETERS:
*
*    choice_index - The number of the choice.
*    flag         - TRUE if the choice is to set as selected, FALSE if not
*                   selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setSelectedChoice (int choice_index, Boolean flag)
{
  int num_choices = selections.getSize ();
    
  if (flag) {
    int temp_int = choice_index-1;
    
    // If a single choice item...
    if (!multipleChoice && selections.getSize ()) {
      // Turn off the old choice
      int old_item = (int) selections.removeItem (0);
      if (owObject) showFeedback (old_item+1, FALSE);
    }
    else {
      /* If the selected item is already in the list, remove it
         and place it at the end of the list */
 
      // Loop through the selections list
      for (int i=0; i < num_choices; i++) {
        // If this is the item of interest...
        if (((int) selections [i]) == temp_int) {
          // Remove it from the list
          (void) selections.removeItem (i);
          break;
        }
      }
    }   

    selections.addItem ((Generic *) temp_int);
    lastSelected = temp_int;
  }
  else {
    // Loop through the selections list
    for (int i=0; i < num_choices; i++) {
      // If this is the item of interest...
      if (((int) selections [i]) == choice_index-1) {
        // Remove it from the list
        (void) selections.removeItem (i);
        break;
      }
    }
  }

  if (owObject)
    showFeedback (choice_index, flag);
}

/*F+ void UIChoice::setSelectedChoice (char *choice_value, Boolean flag)
*
*    PURPOSE:
*
*    Sets the selection status of the specified choice.
*
*    PARAMETERS:
*
*    choice_value - The string value of the choice.
*    flag         - TRUE if the choice is to set as selected, FALSE if not
*                   selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setSelectedChoice (char *choice_value, Boolean flag)
{
  int   num_choices = choices.getSize ();
  char *temp_string;

  for (int i = 0; i < num_choices ; i++) {
    temp_string = (char *) choices [i];
      
    if (!strcmp (temp_string, choice_value)) {
      setSelectedChoice (i+1, flag);
      break;
    }
  }
}

/*F+ void UIChoice::setSelectedChoice (Image &choice_value, Boolean flag)
*
*    PURPOSE:
*
*    Sets the selection status of the specified choice.
*
*    PARAMETERS:
*
*    choice_value - The image value of the choice.
*    flag         - TRUE if the choice is to set as selected, FALSE if not
*                   selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setSelectedChoice (Image &choice_value, Boolean flag)
{
  int row_index = images.findItem ((Generic *) &choice_value);
  setSelectedChoice (row_index+1, flag);
}

/*F+ Boolean UIChoice::isSelectedChoice (int row_number)
*
*    PURPOSE:
*
*    Determines whether or not the specified choice has been selected.
*
*    PARAMETERS:
*
*    row_number - The row number of the choice.
*
*    RETURN VALUES:
*
*    Returns TRUE if the specified choice has been selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Boolean UIChoice::isSelectedChoice (int row_number)
{
  int choice = row_number - 1;
  int num_selections = selections.getSize ();

  // Loop throught the selections...
  for (int i=0; i < num_selections; i++)
    // If this one matches...
    if (choice == (int) selections [i])
      return TRUE;

  return FALSE;
}

/*F+ void UIChoice::notifyProc (Panel_item  item,
*                               int         choice,
*                               Event      *event)
*
*    PURPOSE:
*
*    Internal notify proc wrapper for the UIChoice class.  This is
*    called by XView when a notify occurs.  This proc then calls the user
*    specified UIC target and notify handlers.
*
*    PARAMETERS:
*
*    item        - The XView object the notify occurred on.
*    choice      - The index of the item chosen in the choice object.
*    event       - The XView event information structure
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::notifyProc (Panel_item item, int choice, Event *event)
{
  UIChoice *object = (UIChoice *) xv_get (item, XV_KEY_DATA, UIC_OBJECT_KEY);

  /* In XView V3, some of the components are compund items and the event
     may occur on the child item instead of the parent item.  If this
     happens, the parent (XV_OWNER) must be obtained in order to
     obtain a "this" pointer */
  if (!object) {
    Panel_item compound_item = xv_get (item, PANEL_ITEM_OWNER);
    object = (UIChoice *)
             xv_get (compound_item, XV_KEY_DATA, UIC_OBJECT_KEY);
  }

  int num_choices = object->selections.getSize ();

  /* Add the new selection to or delete the deselection from the
     internal selections list */

  // If this is a single choice object...
  if (!object->multipleChoice) {
    // Save the choice value
    object->lastSelected = choice;

    // If a choice already existed, delete it
    if (num_choices) (void) object->selections.removeItem (0);

    // Add the new choice to the list
    object->selections.addItem ((Generic *) choice);
  }
  // Else, multiple choice...
  else {
    int selection_value = 0;
      
    // Calculate the old choice value mask
    for (int i = 0 ; i < num_choices; i++) 
      selection_value += (1 << ((int) object->selections [i]));

    // Subtract the two masks
    int choice_value = choice - selection_value;

    int bit = 0;
    int selection_index = -1;

    // If the difference is positive, a new choice has been added
    if (choice_value > 0) {
      // Determine the index by shifting the value 1 bit at a time
      do {
	bit = choice_value % 2;
	choice_value >>= 1;
	selection_index++;
      }
      while ((bit == 0) && (selection_index < 32));

      if (selection_index < 32)
        object->selections.addItem ((Generic *)
				    (object->lastSelected = selection_index));
      else
        object->lastSelected = UIC_NOT_SET;
    }
    // Else, a choice has been taken away
    else {
      choice_value = -choice_value;

      // Determine the index by shifting the value 1 bit at a time
      do {
	bit = choice_value % 2;
	choice_value >>= 1;
	selection_index++;
      }
      while ((bit == 0) && (selection_index < 32));

      // Find the item in the selections list and delete it
      int remove_index = object->selections.findItem ((Generic *)
						      selection_index);
      if (remove_index > -1) {
        (void) object->selections.removeItem (remove_index);
        object->lastSelected = selection_index;
      }
      else
	object->lastSelected = UIC_NOT_SET;
    }
  }

  object->setEventInformation (event);
  object->notifyNotifyTargets ();

  if (object->useDefaultNotifyHandlerFlag) 
    object->defaultNotifyHandler ();  
  else if (object->notifyHandler)
    (*object->notifyHandler) (object);
  
  object->resetEventInformation ();

  // If the user specified a notify status
  if (object->notifyStatus != UIC_NOT_SET) { 
    // Set the notify status 
    xv_set (item, PANEL_NOTIFY_STATUS, object->notifyStatus, NULL);
  
    // Reset the internal notify status 
    object->notifyStatus = UIC_NOT_SET; 
  }
}

/*F+ void UIChoice::updateSelections (int choice_index, int delta)
*
*    PURPOSE:
*
*    Internal function for updating choice selection indexes in the selections
*    list due to the addition or deletion of a choice item from the object.
*
*    PARAMETERS:
*
*    choice_index - All selected items indexes above this value are updated.
*    delta        - The change in the value of the selected items indexes.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::updateSelections (int choice_index, int delta)
{
  int selection = choice_index - 1; 
  int num_selected = selections.getSize ();
  int value;
 
  for (int i=0; i < num_selected; i++)
    if ((value = (int) selections [i]) >= selection)
      selections [i] = (Generic *) (value + delta);
}

/*F+ void UIChoice::showFeedback (int choice_index, Boolean flag)
*
*    PURPOSE:
*
*    Internal function for physically showing the selection state feedback
*    of the choice object.
*
*    PARAMETERS:
*
*    choice_index - The index of the item to show the feedback for.
*    flag         - TRUE if the item is in a selected state.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::showFeedback (int choice_index, Boolean flag)
{
  if (!multipleChoice) {
    if (flag)
      xv_set (owObject, PANEL_VALUE, choice_index-1, NULL);
    else
      xv_set (owObject, PANEL_VALUE, -1, NULL);
  }
  else {
    int value = (int) xv_get (owObject, PANEL_VALUE);

    if (flag)
      value |= (1 << (choice_index-1));
    else
      value &= (int) (0xFFFFFFFF - (1 << (choice_index-1)));
    xv_set (owObject, PANEL_VALUE, value, NULL);
  }
}

/*F+ void UIChoice::createWithFonts (UIObject *parent,
*                                    Xv_pkg   *package,
*                                    void     *terminator)
*
*    PURPOSE:
*
*    Internal function for creating an XView object that sets the values
*    of fonts in the xv_create call.  This function allows the setting of
*    PANEL_CHOICE_FONT, PANEL_LABEL_FONT, and PANEL_VALUE_FONT.
*
*    PARAMETERS:
*
*    parent     - The UIC parent of the object.
*    package    - The XView package that is being created.
*    terminator - Argument list terminator (allows compatibility with
*                 the use of var args in the future).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::createWithFonts (UIObject  *parent,
				Xv_pkg    *package,
				void      *terminator)
{
  if (choiceFont)
    ((UIObject *) choiceFont)->createObject (parent);

  if (labelFont)
    ((UIObject *) labelFont)->createObject (parent);

  if (valueFont)
    ((UIObject *) valueFont)->createObject (parent);

  if (labelFont && valueFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  PANEL_LABEL_FONT, labelFont->getXViewObject (),
			  PANEL_VALUE_FONT, valueFont->getXViewObject (),
			  NULL);
  else if (labelFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  PANEL_LABEL_FONT, labelFont->getXViewObject (),
			  NULL);
  else if (valueFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  PANEL_VALUE_FONT, valueFont->getXViewObject (),
			  NULL);
  else
    owObject = xv_create (parent->getXViewObject (), package, NULL);
}

/*F+ void UIChoice::createWithFonts (UIObject              *parent,
*                                    Xv_pkg                *package,
*                                    Panel_attr             attribute,
*                                    Panel_attribute_value  value,
*                                    void                  *terminator)
*
*    PURPOSE:
*
*    Internal function for creating an XView object that sets the values
*    of fonts in the xv_create call.  This function allows the setting of
*    PANEL_CHOICE_FONT, PANEL_LABEL_FONT, and PANEL_VALUE_FONT.  This
*    version of this function can handler package names that are actually
*    macros with an XView package and attribute specified.
*
*    PARAMETERS:
*
*    parent     - The UIC parent of the object.
*    package    - The XView package that is being created.
*    attribute  - An XView attribute name.
*    value      - The value of the XView attribute.
*    terminator - Argument list terminator (allows compatibility with
*                 the use of var args in the future).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::createWithFonts (UIObject	      *parent,
				Xv_pkg                *package,
				Panel_attr             attribute,
				Panel_attribute_value  value,
				void                  *terminator)
{
  if (choiceFont)
    ((UIObject *) choiceFont)->createObject (parent);

  if (labelFont)
    ((UIObject *) labelFont)->createObject (parent);

  if (valueFont)
    ((UIObject *) valueFont)->createObject (parent);

  if (labelFont && valueFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  attribute,        value,
			  PANEL_LABEL_FONT, labelFont->getXViewObject (),
			  PANEL_VALUE_FONT, valueFont->getXViewObject (),
			  NULL);
  else if (labelFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  attribute,        value,
			  PANEL_LABEL_FONT, labelFont->getXViewObject (),
			  NULL);
  else if (valueFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  attribute,        value,
			  PANEL_VALUE_FONT, valueFont->getXViewObject (),
			  NULL);
  else
    owObject = xv_create (parent->getXViewObject (), package,
			  attribute, value,
			  NULL);
}

/*F+ void UIChoice::createWithFonts (UIObject              *parent,
*                                    Xv_pkg                *package,
*                                    Panel_attr             attribute1,
*                                    Panel_attribute_value  value1,
*                                    Panel_attr             attribute2,
*                                    Panel_attribute_value  value2,
*                                    void                  *terminator)
*
*    PURPOSE:
*
*    Internal function for creating an XView object that sets the values
*    of fonts in the xv_create call.  This function allows the setting of
*    PANEL_CHOICE_FONT, PANEL_LABEL_FONT, and PANEL_VALUE_FONT.  This
*    version of this function can handler package names that are actually
*    macros with an XView package and attribute specified.
*
*    PARAMETERS:
*
*    parent     - The UIC parent of the object.
*    package    - The XView package that is being created.
*    attribute1 - An XView attribute name.
*    value1     - The value of the XView attribute.
*    attribute2 - Another XView attribute name.
*    value2     - The value of the second XView attribute.
*    terminator - Argument list terminator (allows compatibility with
*                 the use of var args in the future).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::createWithFonts (UIObject	      *parent,
				Xv_pkg                *package,
				Panel_attr             attribute1,
				Panel_attribute_value  value1,
				Panel_attr             attribute2,
				Panel_attribute_value  value2,
				void                  *terminator)
{
  if (choiceFont)
    ((UIObject *) choiceFont)->createObject (parent);

  if (labelFont)
    ((UIObject *) labelFont)->createObject (parent);

  if (valueFont)
    ((UIObject *) valueFont)->createObject (parent);

  if (labelFont && valueFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  attribute1,        value1,
			  attribute2,        value2,
			  PANEL_LABEL_FONT,  labelFont->getXViewObject (),
			  PANEL_VALUE_FONT,  valueFont->getXViewObject (),
			  NULL);
  else if (labelFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  attribute1,        value1,
			  attribute2,        value2,
			  PANEL_LABEL_FONT,  labelFont->getXViewObject (),
			  NULL);
  else if (valueFont)
    owObject = xv_create (parent->getXViewObject (), package,
			  attribute1,        value1,
			  attribute2,        value2,
			  PANEL_VALUE_FONT,  valueFont->getXViewObject (),
			  NULL);
  else
    owObject = xv_create (parent->getXViewObject (), package,
			  attribute1, value1,
			  attribute2, value2,
			  NULL);
}

/*F+ UIChoice::setUIChoiceAttributes (UIObject *parent)
*
*    PURPOSE:
*
*    Sets common UIChoice XView attributes that are required for creating
*    the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of this UIChoice object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void UIChoice::setUIChoiceAttributes (UIObject *parent)
{
  setUIComponentAttributes (parent);

  xv_set (owObject, PANEL_NOTIFY_PROC, &UIChoice::notifyProc, NULL);

  if (numColumns > 0) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_CHOICE_NCOLS, numColumns, NULL, NULL);
    else
      xv_set (owObject, PANEL_CHOICE_NCOLS, numColumns, NULL);
  }

  if (numRows > 0) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_CHOICE_NROWS, numRows, NULL, NULL);
    else
      xv_set (owObject, PANEL_CHOICE_NROWS, numRows, NULL);
  }

  if (labelPosition == ABOVE)
    xv_set (owObject, PANEL_LAYOUT, PANEL_VERTICAL, NULL);

  if (selectionRequired)
    xv_set (owObject, PANEL_CHOOSE_NONE, FALSE, NULL);
  else
    xv_set (owObject, PANEL_CHOOSE_NONE, TRUE, NULL);
   
  if (multipleChoice)
    xv_set (owObject, PANEL_CHOOSE_ONE, FALSE, NULL);
  else
    xv_set (owObject, PANEL_CHOOSE_ONE, TRUE, NULL);
   
  int num_choices = choices.getSize ();
  int i;
  for (i = 0; i < num_choices ; i++)
    if (choices [i]) {
      xv_set (owObject, PANEL_CHOICE_STRING, i, (char *) choices [i], NULL);

      if (choiceFont)
	xv_set (owObject,
	        PANEL_CHOICE_FONT, i, choiceFont->getXViewObject (),
		NULL);
    }
    else {
      ((UIObject *) images [i])->createObject (parent);
      xv_set (owObject,
	      PANEL_CHOICE_IMAGE, i, ((Image *) images [i])->getXViewObject (),
	      NULL);
    }
  
  int color_index;
  for (i = 0; i < num_choices; i++) 
    if ((color_index =  (int) choiceColors [i]) != UIC_NOT_SET) {
      initializeColorMap (parent, TRUE);
      xv_set (owObject, PANEL_CHOICE_COLOR, i, color_index, NULL);
    }
  
  int num_selected = selections.getSize ();

  // If there are preselected choices...
  if (num_selected > 0) {
    // If single choice object...
    if ((!multipleChoice) && (num_selected == 1))
      xv_set (owObject, PANEL_VALUE, (int) selections [0], NULL);
    // Else, multiple choice object...
    else {
      int selection_value = 0;
      
      // Create a bit mask representing the on choices
      for (i = 0 ; i < num_selected; i++) 
	selection_value += (1 << ((int) selections [i]));

      xv_set (owObject, PANEL_VALUE, selection_value, NULL);
    }
  }
  // Else, no preselected choice, if this is a single choice object
  // and a choice is required...
  else if (!multipleChoice && selectionRequired) {
    // Single choice objects always require a choice to be set...
    selections.addItem ((Generic *) 0);
    lastSelected = 0;

    xv_set (owObject, PANEL_VALUE, 0, NULL);
  }
}

#ifdef TEST
#endif /* TEST */
