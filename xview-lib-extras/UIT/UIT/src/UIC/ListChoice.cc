/*H+ ListChoice.cc
*
*    SCCS ID: @(#)ListChoice.cc 1.5 93/06/01 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the ListChoice class.
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
static char sccsid [] = "@(#)ListChoice.cc 1.5 93/06/01 SMI"; 
#endif

// Includes
#include <memory.h>

#include "CharacterFont.h"
#include "ListChoice.h"

/*F+ ListChoice::ListChoice ()
*
*    PURPOSE:
*
*    Constructor of the ListChoice class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ListChoice::ListChoice ()
{
  setListChoiceDefaults ();
}

/*F+ ListChoice::ListChoice (char *string_label)
*
*    PURPOSE:
*
*    Constructor of the ListChoice class.
*
*    PARAMETERS:
*
*    string_label - Specifies a string for the ListChoice label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ListChoice::ListChoice (char *string_label) : UIChoice (string_label)
{
  setListChoiceDefaults ();
}

/*F+ ListChoice::ListChoice (Image &image_label)
*
*    PURPOSE:
*
*    Constructor of the ListChoice class.
*
*    PARAMETERS:
*
*    image_label - Specifies an image for the ListChoice label.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ListChoice::ListChoice (Image &image_label) : UIChoice (image_label)
{
  setListChoiceDefaults ();
}

/*F+ ListChoice::setListChoiceDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the ListChoice class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::setListChoiceDefaults ()
{
  rowHeight = UIC_NOT_SET;
  numRows = 5;
  readOnly = FALSE;
  lastSelected = UIC_NOT_SET;
  listTitle = (char *) NULL;
  listMenu = (UIMenu *) NULL;
  rowID = 0;
  labelFont = (CharacterFont *) NULL;
  listFont = (CharacterFont *) NULL;
  scroller = (Scroller *) NULL;
  selectStatus = UNKNOWN;
}

/*F+ ListChoice::~ListChoice ()
*
*    PURPOSE:
*
*    Destructor of the ListChoice class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ListChoice::~ListChoice ()
{
  if (listTitle) delete listTitle;
  if (scroller) delete scroller;
}

/*F+ void ListChoice::deleteAllChoices ()
*
*    PURPOSE:
*
*    Deletes all of the rows in the list.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::deleteAllChoices ()
{
  int num_choices = getNumberOfChoices ();
  
  // Get rid of all of the selections
  selections.dropItems ();

  // If the XView object exists...
  if (owObject)
    xv_set (owObject, PANEL_LIST_DELETE_ROWS, 0, num_choices, NULL);
  // Else, this is pre-XView list manipulation...
  else {
    // Delete all of the strings
    for (int i=num_choices-1; i > -1; i--) {
      char *string_entry = (char *) choices.removeItem (i);
      if (string_entry) delete (string_entry);
    }

    // Get rid of all the images
    images.dropItems ();

    // Get rid of the choice data
    choiceData.dropItems ();
  }

  lastSelected = UIC_NOT_SET;
  selectStatus = UNKNOWN;
}

/*F+ void ListChoice::deleteChoice (int row_number)
*
*    PURPOSE:
*
*    Deletes the specified row from the list.
*
*    PARAMETERS:
*
*    row_number - The number of the row to delete.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::deleteChoice (int row_number)
{
  int num_choices = getNumberOfChoices ();
  
  // If the row exists...
  if ((row_number <= num_choices) && (row_number > 0)) {
    // Internal row number is 0 to n-1
    row_number--;
    
    // If the XView object exists...
    if (owObject) {
      xv_set (owObject, PANEL_LIST_DELETE, row_number, NULL);
      
      // If the deleted row was the last row selected...
      if (lastSelected == row_number) {
	lastSelected = UIC_NOT_SET;
	selectStatus = UNKNOWN;
      }
      // Else, if the last selected row was after the deleted row...
      else if (lastSelected > row_number)
	lastSelected--;
    }
    // Else, this is pre-XView list manipulation...
    else {
      // If the deleted row was a pre-selected row, unselect it
      setSelectedChoice (row_number+1, FALSE);

      // Update the row numbers of the pre-selected items
      updateSelections (row_number+1, -1);
      
      // Delete the row entry from the internal lists
      char *string_entry = (char *) choices.removeItem (row_number);
      Image *image_entry = (Image *) images.removeItem (row_number);
      Generic *choice_data = choiceData.removeItem (row_number);
      
      if (string_entry) delete (string_entry);
    }
  }
}

/*F+ void ListChoice::deleteChoice (char *entry)
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
void ListChoice::deleteChoice (char *entry)
{
  int num_choices = getNumberOfChoices ();
  int row_number;
  
  // If the XView object already exists...
  if (owObject) {
    // Loop through the list items...
    for (row_number=0; row_number < num_choices; row_number++) {
      char *temp_string = (char *) xv_get (owObject,
					   PANEL_LIST_STRING,
					   row_number);
      
      // If this is the row of interest...
      if (temp_string && !strcmp (entry, temp_string)) {
	xv_set (owObject, PANEL_LIST_DELETE, row_number, NULL);
	
	// If the deleted row was the last row selected...
	if (lastSelected == row_number) {
	  lastSelected = UIC_NOT_SET;
	  selectStatus = UNKNOWN;
	}
	// Else, if the last selected row was after the deleted row...
	else if (lastSelected > row_number)
	  lastSelected--;

	break;
      }
    }
  } 
  // Else, this is pre-XView list manipulation...
  else {
    // Loop through the list items...
    for (row_number=0; row_number < num_choices; row_number++) {
      char *temp_string = (char *) choices [row_number];
      
      // If this is the row of interest...
      if (!strcmp (entry, temp_string)) {
	// Unselect the row to be deleted
	setSelectedChoice (row_number+1, FALSE);

	// Update the row numbers of the pre-selected items
	updateSelections (row_number+1, -1);
	
	// Delete the item from the internal lists
	char *string_entry = (char *) choices.removeItem (row_number);
	Image *image_entry = (Image *) images.removeItem (row_number);
	Generic *choice_data = choiceData.removeItem (row_number);
	
	if (string_entry) delete (string_entry);
	break;
      }
    }
  }
}

/*F+ void ListChoice::deleteChoice (Image &entry)
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
void ListChoice::deleteChoice (Image &entry)
{
  int num_choices = getNumberOfChoices ();
  int row_number;
  
  // If the XView object already exists...
  if (owObject) {
    // Loop the items in the list...
    for (row_number=0; row_number < num_choices; row_number++) {
      ListClientData *data = (ListClientData *)
			     xv_get (owObject,
				     PANEL_LIST_CLIENT_DATA,
				     row_number);
      
      // If this is the row of interest...
      if (data->image == &entry) {
	xv_set (owObject, PANEL_LIST_DELETE, row_number, NULL);
	
	// If the deleted row was the last row selected...
	if (lastSelected == row_number) {
	  lastSelected = UIC_NOT_SET;
	  selectStatus = UNKNOWN;
	}
	// Else, if the last selected row was after the deleted row...
	else if (lastSelected > row_number)
	  lastSelected--;

	break;
      }
    }
  } 
  // Else, this is pre-XView list manipulation...
  else {
    // Loop through the items in the list...
    for (row_number=0; row_number < num_choices; row_number++) {
      Image *temp_image = (Image *) choices [row_number];
      
      // If this is the row of interest...
      if (temp_image == &entry) {
	// Unselect the row to be deleted
	setSelectedChoice (row_number+1, FALSE);

	// Update the row numbers of the pre-selected items
	updateSelections (row_number+1, -1);
	
	// Delete the item from the internal lists
	char *string_entry = (char *) choices.removeItem (row_number);
	Image *image_entry = (Image *) images.removeItem (row_number);
	Generic *choice_data = choiceData.removeItem (row_number);
	
	if (string_entry) delete (string_entry);
	break;
      }
    }
  }
}

/*F+ int ListChoice::addChoice (char *entry)
*
*    PURPOSE:
*
*    Adds a row with the specified value to the end of the list.
*
*    PARAMETERS:
*
*    entry - The string value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the list.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ListChoice::addChoice (char *entry)
{
  int row_number = getNumberOfChoices ();	
  
  // If the XView object already exists...
  if (owObject) {
    // Create a client data structure to be stored with the new row
    ListClientData *client_data = new ListClientData (++rowID);
    
    // If a font has been specified...
    if (listFont) {
      // Create the XView font object, if necessary
      if (!listFont->getXViewObject ())
	((UIObject *) listFont)->createObject (this);
      
      xv_set (owObject, PANEL_LIST_INSERT, row_number, 
	                PANEL_LIST_FONT,   row_number,
	                                   listFont->getXViewObject (),
	                PANEL_LIST_STRING, row_number, entry,
	                PANEL_LIST_CLIENT_DATA, row_number, client_data,
	      NULL);
    }
    // Else, no font was specified...
    else
      xv_set (owObject, PANEL_LIST_INSERT, row_number, 
	                PANEL_LIST_STRING, row_number, entry,
	                PANEL_LIST_CLIENT_DATA, row_number, client_data,
	      NULL);
  }
  // Else, this is pre-XView object list construction...
  else {
    char *temp_string = strcpy (new char [strlen (entry) + 1], entry);
    
    // Add the entry to the internal lists
    choices.addItem ((Generic *) temp_string);
    images.addItem ((Generic *) NULL);
    choiceData.addItem ((Generic *)NULL);
  }
  
  // Return the row number of the new item
  return (row_number + 1);
}

/*F+ int ListChoice::addChoice (Image &entry)
*
*    PURPOSE:
*
*    Adds a row with the specified value to the end of the list.
*
*    PARAMETERS:
*
*    entry - The image value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the list.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ListChoice::addChoice (Image &entry)
{
  int row_number = getNumberOfChoices ();	
  
  // If the XView object already exists...
  if (owObject) {
    // Create a client data structure to be stored with the new row
    ListClientData *client_data = new ListClientData (++rowID, &entry);
    
    ((UIObject *) &entry)->createObject (this);
    xv_set (owObject, PANEL_LIST_INSERT, row_number, 
	              PANEL_LIST_STRING, row_number, " ",
	              PANEL_LIST_GLYPH,  row_number, entry.getXViewObject (), 
	              PANEL_LIST_CLIENT_DATA, row_number, client_data,
	    NULL);
  }
  // Else, this is pre-XView object list construction...
  else {
    // Add the entry to the internal lists
    choices.addItem ((Generic *) NULL);
    images.addItem ((Generic *) &entry);
    choiceData.addItem ((Generic *) NULL);
  }
  
  // Return the row number of the new item
  return (row_number + 1);
}

/*F+ int ListChoice::addChoice (char *string, Image &image)
*
*    PURPOSE:
*
*    Adds a row with the specified values to the end of the list.
*
*    PARAMETERS:
*
*    string - The string value of the row to add.
*    image  - The image value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the list.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ListChoice::addChoice (char *string, Image &image)
{
  int row_number = getNumberOfChoices ();
  
  // If the XView object already exists...
  if (owObject) {
    // Create a client data structure to be stored with the new row
    ListClientData *client_data = new ListClientData (++rowID, &image);
    
    // Create the XView image
    ((UIObject *) &image)->createObject (this);

    // If a font has been specified...
    if (listFont) {
      // Create the XView font object, if necessary
      if (!listFont->getXViewObject ())
	((UIObject *) listFont)->createObject (this);
      
      xv_set (owObject, PANEL_LIST_INSERT, row_number, 
	                PANEL_LIST_FONT,   row_number,
	                                   listFont->getXViewObject (),
	                PANEL_LIST_STRING, row_number, string,
	                PANEL_LIST_GLYPH,  row_number, image.getXViewObject (),
	                PANEL_LIST_CLIENT_DATA, row_number, client_data,
	      NULL);
    }
    // Else, this is pre-XView object list construction...
    else
      xv_set (owObject, PANEL_LIST_INSERT, row_number, 
	                PANEL_LIST_STRING, row_number, string,
	                PANEL_LIST_GLYPH,  row_number, image.getXViewObject (),
	                PANEL_LIST_CLIENT_DATA, row_number, client_data,
	      NULL);
  }
  // Else, this is pre-XView object list construction...
  else {
    char *temp_string = strcpy (new char [strlen (string) + 1], string);
    
    // Add the item to the internal lists
    choices.addItem ((Generic *) temp_string);
    images.addItem ((Generic *) &image);
    choiceData.addItem ((Generic *) NULL);
  }

  // Return the row number of the new item
  return (row_number + 1);
}

/*F+ void ListChoice::addChoice (int row_number, char *entry)
*
*    PURPOSE:
*
*    Inserts a row with the specified value at the specified position
*    in the list.
*
*    PARAMETERS:
*
*    row_number - The position of the new row.
*    entry      - The string value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the list.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ListChoice::addChoice (int row_number, char *entry)
{
  int num_items = getNumberOfChoices ();

  // Determine the actual offset into the list
  row_number--;
  
  // If the row number is greater then the number of items in the list...
  if (row_number > num_items-1)
    // Add the item to the end of the list
    row_number = addChoice (entry) - 1;
  else {
    // If the XView object already exists...
    if (owObject) {
      // Create a client data structure to be stored with the new row
      ListClientData *client_data =
		      new ListClientData (++rowID);
    
      // If a font is specified...
      if (listFont) {
	// Create the XView font object, if necessary
        if (!listFont->getXViewObject ())
	  ((UIObject *) listFont)->createObject (this);
      
	xv_set (owObject, PANEL_LIST_INSERT, row_number, 
	                  PANEL_LIST_FONT,   row_number,
	                                     listFont->getXViewObject (),
	                  PANEL_LIST_STRING, row_number, entry,
	                  PANEL_LIST_CLIENT_DATA, row_number, client_data,
		NULL);
      }
      else
	xv_set (owObject, PANEL_LIST_INSERT, row_number, 
	                  PANEL_LIST_STRING, row_number, entry,
	                  PANEL_LIST_CLIENT_DATA, row_number, client_data,
		NULL);
    }
    // Else, this is pre-XView object list construction...
    else {
      char  *temp_string = strcpy (new char [strlen (entry) + 1], entry);

      // Add the current last item to the end of the list
      choices.addItem (choices [num_items-1]);
      images.addItem (images [num_items-1]);
      choiceData.addItem (choiceData [num_items-1]);

      // Shift all of the items after the new row
      for (int i=num_items-2; i >= row_number; i--) {
	choices [i+1] = choices [i];
	images [i+1] = images [i];
	choiceData [i+1] = choiceData [i];
      }

      // Put the new row into the list
      choices [row_number] = (Generic *) temp_string;
      images [row_number] = (Generic *) NULL;
      choiceData [row_number] = (Generic *) NULL;

      // Update the the selected row numbers that are greater then the new one
      updateSelections (row_number+1, 1);
    }
  }
  
  return (row_number + 1);
}

/*F+ void ListChoice::addChoice (int row_number, Image &entry)
*
*    PURPOSE:
*
*    Inserts a row with the specified value at the specified position
*    in the list.
*
*    PARAMETERS:
*
*    row_number - The position of the new row.
*    entry      - The image value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the list.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ListChoice::addChoice (int row_number, Image &entry)
{
  int num_items = getNumberOfChoices ();

  // Determine the actual offset into the list
  row_number--;
  
  // If the row number is greater then the number of items in the list...
  if (row_number > num_items-1)
    // Add the item to the end of the list
    row_number = addChoice (entry) - 1;
  else {
    // If the XView object already exists
    if (owObject) {
      // Create a client data structure to be stored with the new row
      ListClientData *client_data = new ListClientData (++rowID, &entry);

      ((UIObject *) &entry)->createObject (this);
      xv_set (owObject, PANEL_LIST_INSERT, row_number,
		        PANEL_LIST_STRING, row_number, " ",
		        PANEL_LIST_GLYPH,  row_number, entry.getXViewObject (),
		        PANEL_LIST_CLIENT_DATA, row_number, client_data,
	      NULL);
    }
    else {
      // Add the current last item to the end of the list
      choices.addItem (choices [num_items-1]);
      images.addItem (images [num_items-1]);
      choiceData.addItem (choiceData [num_items-1]);

      // Shift all of the items after the new row
      for (int i=num_items-2; i >= row_number; i--) {
	choices [i+1] = choices [i];
	images [i+1] = images [i];
	choiceData [i+1] = choiceData [i];
      }

      // Put the new row into the list
      choices [row_number] = (Generic *) NULL;
      images [row_number] = (Generic *) &entry;
      choiceData [row_number] = (Generic *) NULL;

      // Update the the selected row numbers that are greater then the new one
      updateSelections (row_number+1, 1);
    }
  }

  return (row_number + 1);
}

/*F+ void ListChoice::addChoice (int row_number, char *string, Image &image)
*
*    PURPOSE:
*
*    Inserts a row with the specified value at the specified position
*    in the list.
*
*    PARAMETERS:
*
*    row_number - The position of the new row.
*    string     - The string value of the row to add.
*    image      - The image value of the row to add.
*
*    RETURN VALUES:
*
*    Returns the position of the entry in the list.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ListChoice::addChoice (int row_number, char *string, Image &image)
{
  int num_items = getNumberOfChoices ();

  // Determine the actual offset into the list
  row_number--;
  
  // If the row number is greater then the number of items in the list...
  if (row_number > num_items-1)
    // Add the item to the end of the list
    row_number = addChoice (string, image) - 1;
  else {
    // If the XView object already exists...
    if (owObject) {
      // Create a client data structure to be stored with the new row
      ListClientData *client_data = new ListClientData (++rowID, &image);
    
      // Create the XView image
      ((UIObject *) &image)->createObject (this);

      // If a font is specified...
      if (listFont) {
	// Create the XView font object, if necessary
        if (!listFont->getXViewObject ())
	  ((UIObject *) listFont)->createObject (this);
      
	xv_set (owObject, PANEL_LIST_INSERT, row_number, 
			  PANEL_LIST_FONT,   row_number,
			  		     listFont->getXViewObject (),
			  PANEL_LIST_STRING, row_number, string,
	                  PANEL_LIST_GLYPH,  row_number,
		                             image.getXViewObject (),
			  PANEL_LIST_CLIENT_DATA, row_number, client_data,
		NULL);
      }
      else
	xv_set (owObject, PANEL_LIST_INSERT, row_number, 
			  PANEL_LIST_STRING, row_number, string,
			  PANEL_LIST_GLYPH,  row_number,
		                             image.getXViewObject (),
			  PANEL_LIST_CLIENT_DATA, row_number, client_data,
		NULL);
    }
    // Else, this is pre-XView object list construction...
    else {
      char  *temp_string = strcpy (new char [strlen (string) + 1], string);

      // Add the current last item to the end of the list
      choices.addItem (choices [num_items-1]);
      images.addItem (images [num_items-1]);
      choiceData.addItem (choiceData [num_items-1]);

      // Shift all of the items after the new row
      for (int i=num_items-2; i >= row_number; i--) {
	choices [i+1] = choices [i];
	images [i+1] = images [i];
	choiceData [i+1] = choiceData [i];
      }

      // Put the new row into the list
      choices [row_number] = (Generic *) temp_string;
      images [row_number] = (Generic *) &image;
      choiceData [row_number] = (Generic *) NULL;

      // Update the the selected row numbers that are greater then the new one
      updateSelections (row_number+1, 1);
    }
  }
  
  return (row_number + 1);
}

/*F+ void ListChoice::setChoiceData (int row_number, Generic *data)
*
*    PURPOSE:
*
*    Associate data with the specified choice.  The choice must already
*    exist.
*
*    PARAMETERS:
*
*    row_number - the row number of the choice
*    data       - the data
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::setChoiceData (int row_number, Generic *data)
{
  // If the row exists...
  if ((row_number > 0) && (row_number <= getNumberOfChoices ())) {
    row_number--;
    
    // If the XView object already exists...
    if (owObject) {
      // Get the client data for the row
      ListClientData *client_data = (ListClientData *)
	                            xv_get (owObject,
					    PANEL_LIST_CLIENT_DATA, row_number,
					    NULL);

      // Set the client data image value
      client_data->choiceData = data;
    }
    // Else, pre-XView list...
    else
      choiceData [row_number] = data;
  }
}

/*F+ void ListChoice::setChoiceData (int row_number, Generic &data)
*
*    PURPOSE:
*
*    Associate data with the specified choice.  The choice must already
*    exist.
*
*    PARAMETERS:
*
*    row_number - the row number of the choice
*    data       - the data
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::setChoiceData (int row_number, Generic &data)
{
  // If the row exists...
  if ((row_number > 0) && (row_number <= getNumberOfChoices ())) {
    row_number--;
    
    // If the XView object already exists...
    if (owObject) {
      // Get the client data for the row
      ListClientData *client_data = (ListClientData *)
	                            xv_get (owObject,
					    PANEL_LIST_CLIENT_DATA, row_number,
					    NULL);

      // Set the client data image value
      client_data->choiceData = &data;
    }
    // Else, pre-XView list...
    else
      choiceData [row_number] = &data;
  }
}

/*F+ void ListChoice::setChoiceData (int row_number, Generic &data)
*
*    PURPOSE:
*
*    Get the data associated with the specified choice.
*
*    PARAMETERS:
*
*    row_number - the row number of the choice
*
*    RETURN VALUES:
*
*    Returns a pointer to the data.  Returns NULL if there
*    is no data or the specified choice does not exist.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
Generic *ListChoice::getChoiceData (int row_number)
{
  Generic *data = (Generic *)NULL;

  // If the row exists...
  if ((row_number > 0) && (row_number <= getNumberOfChoices ())) {
    row_number--;
    
    // If the XView object already exists...
    if (owObject) {
      // Get the client data for the row
      ListClientData *client_data = (ListClientData *)
                                    xv_get (owObject,
                                            PANEL_LIST_CLIENT_DATA, row_number,
                                            NULL);
 
      // Get the client data image value
      data = client_data->choiceData;
    }
    // Else, pre-XView list...
    else
      data = choiceData [row_number];
  }    

  return data;
}
 
/*F+ void ListChoice::setTitle (char *title)
*
*    PURPOSE:
*
*    Specifies the list title.
*
*    PARAMETERS:
*
*    title - The list title.
*
*    REVISION HISTORY:
*
*    Who       When       Why
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::setTitle (char *title)
{
  if (listTitle)
    delete listTitle;

  if (title)
    listTitle = strcpy (new char [strlen (title) + 1], title);
  else
    listTitle = (char *) NULL;
 
  // If the XView object already exists...
  if (owObject)
    xv_set (owObject, PANEL_LIST_TITLE, listTitle, NULL);
}

/*F+ void ListChoice::setMenu (UIMenu &menu)
*
*    PURPOSE:
*
*    Specifies the menu to be shown if the user presses the menu
*    mouse button while on top of the ListChoice object.
*
*    PARAMETERS:
*
*    menu - The menu to show.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::setMenu (UIMenu &menu)
{
  listMenu = &menu;
  
  // If the XView object already exists...
  if (owObject) {
    // If an old menu exists, delete it
    Menu old_menu = (Menu) xv_get (owObject, PANEL_ITEM_MENU);
    if (old_menu) xv_destroy_safe (old_menu);
    
    // Create the new XView menu object and set the menu
    ((UIObject *) listMenu)->createObject (parentObject);
    xv_set (owObject, PANEL_ITEM_MENU, listMenu->getXViewObject (), NULL);
  }	
}

/*F+ char *ListChoice::getChoice (char* &return_value)
*
*    PURPOSE:
*
*    Returns the string value associated with the last list item
*    operated on (selected or deselected).
*
*    PARAMETERS:
*
*    return_value - The string value of the last selected item.
*
*    RETURN VALUES:
*
*    The string value of the last selected item is returned.  Memory
*    to store the string is allocated on the heap and needs to be
*    deleted by the caller.  If there was no last selected item,
*    NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *ListChoice::getChoice (char* &return_value)
{
  return_value = (char *) NULL;
  
  if (lastSelected != UIC_NOT_SET)
    (void) getValue (lastSelected+1, return_value);
  
  return (return_value);
}

/*F+ Image *ListChoice::getChoice (Image* &return_value)
*
*    PURPOSE:
*
*    Returns a pointer to the image value associated with the last list item
*    operated on (selected or deselected).
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
Image *ListChoice::getChoice (Image* &return_value)
{
  return_value = (Image *) NULL;
  
  if (lastSelected != UIC_NOT_SET)
    (void) getValue (lastSelected+1, return_value);
  
  return (return_value);
}

/*F+ int ListChoice::getChoice (int &return_value)
*
*    PURPOSE:
*
*    Returns the row number associated with the last list item 
*    operated on (selected or deselected).
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
int ListChoice::getChoice (int &return_value)
{
  return (return_value = (lastSelected != UIC_NOT_SET) ?
	                 lastSelected + 1 : UIC_NOT_SET);
}

/*F+ char *ListChoice::getChoice (int selection, char* &return_value)
*
*    PURPOSE:
*
*    Primarily used by ListChoice objects that allow multiple choice.
*    This version of getChoice returns the string value associated
*    with the selection specified out the list of current selections.
*
*    PARAMETERS:
*    selection    - The position in the selection list.
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
char *ListChoice::getChoice (int selection, char* &return_value)
{
  int num_items = getNumberOfChoices ();
  int num_selected = 0;
  
  return_value = (char *) NULL;
  
  // Loop through all of the items in the list
  for (int i = 1; i <= num_items; i++) {
    // If this is a selected item...
    if (isSelectedChoice (i)) {
      num_selected++;

      // If this is the specified selection...
      if (num_selected == selection) {
	(void) getValue (i, return_value);
	break;
      }
    }
  }

  return (return_value);
}     

/*F+ Image *ListChoice::getChoice (int selection, Image* &return_value)
*
*    PURPOSE:
*
*    Primarily used by ListChoice objects that allow multiple choice.
*    This version of getChoice returns a pointer to the image value associated
*    with the selection specified out the list of current selections.
*
*    PARAMETERS:
*
*    selection    - The position in the selection list.
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
Image *ListChoice::getChoice (int selection, Image* &return_value)
{
  int num_items = getNumberOfChoices ();
  int num_selected = 0;
  
  return_value = (Image *) NULL;
  
  // Loop through all of the items in the list
  for (int i = 1; i <= num_items; i++) {
    // If this is a selected item...
    if (isSelectedChoice (i)) {
      num_selected++;

      // If this is the specified selection...
      if (num_selected == selection) {
	(void) getValue (i, return_value);
	break;
      }
    }
  }
  
  return (return_value);
}     

/*F+ int ListChoice::getChoice (int selection, int &return_value)
*
*    PURPOSE:
*
*    Primarily used by ListChoice objects that allow multiple choice.
*    This version of getChoice returns the row number associated
*    with the selection specified out the list of current selections.
*
*    PARAMETERS:
*
*    selection    - The position in the selection list.
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
int ListChoice::getChoice (int selection, int &return_value)
{
  int num_items = getNumberOfChoices ();
  int num_selected = 0;
  
  return_value = UIC_NOT_SET;
  
  // Loop through all of the items in the list
  for (int i = 1; i <= num_items; i++) {
    // If this is a selected item...
    if (isSelectedChoice (i)) {
      num_selected++;

      // If this is the specified selection...
      if (num_selected == selection) {
	return_value = i;
	break;
      }
    }
  }
  
  return (return_value);
}     

/*F+ int ListChoice::getNumberSelected ()
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
int ListChoice::getNumberSelected ()
{
  int num_selected = 0;

  // If the XView object already exists...
  if (owObject) {
    int num_choices = getNumberOfChoices ();

    // Loop through the choices and count the selected ones
    for (int i = 1; i <= num_choices; i++)
      if (isSelectedChoice (i)) 
	num_selected++;
  }
  // Else, pre-XView list...
  else
    // get the size of the selections list
    num_selected = selections.getSize ();
    
  return (num_selected);
}
 
/*F+ char *ListChoice::getValue (char* &return_value)
*
*    PURPOSE:
*
*    Returns the string value associated with the last list item
*    chosen.
*
*    PARAMETERS:
*
*    return_value - The string value of the last selected item.
*
*    RETURN VALUES:
*
*    The string value of the last selected item is returned.  Memory
*    to store the string is allocated on the heap and needs to be
*    deleted by the caller.  If there was no last selected item,
*    NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *ListChoice::getValue (char* &return_value)
{ 
  return (getChoice (return_value));
}

/*F+ Image *ListChoice::getValue (Image* &return_value)
*
*    PURPOSE:
*
*    Returns a pointer to the image value associated with the last list item
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
Image *ListChoice::getValue (Image* &return_value)
{
  return (getChoice (return_value));
}

/*F+ char *ListChoice::getValue (int row_number, char* &return_value)
*
*    PURPOSE:
*
*    Returns the string value associated with the specified row.
*
*    PARAMETERS:
*
*    return_value - The string value of the specified row.
*
*    RETURN VALUES:
*
*    The string value of the specified row is returned.  Memory
*    to store the string is allocated on the heap and needs to be
*    deleted by the caller.  If the specified row does not exist,
*    NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *ListChoice::getValue (int row_number, char* &return_value)
{ 
  char *temp_string;

  return_value = (char *) NULL;

  // If the row exists...
  if ((row_number > 0) && (row_number <= getNumberOfChoices ())) {
    row_number--;

    // If the XView object already exists...
    if (owObject)
      temp_string = (char *) xv_get (owObject,
				     PANEL_LIST_STRING, row_number,
				     NULL);
    // Else, this is a pre-XView list...
    else
      temp_string = (char *) choices [row_number];
    
    if (temp_string)
      return_value = strcpy (new char [strlen (temp_string) + 1], temp_string);
  }
    
  return (return_value);
}

/*F+ Image *ListChoice::getValue (int row_number, Image* &return_value)
*
*    PURPOSE:
*
*    Returns a pointer to the image value associated with the specified row.
*
*    PARAMETERS:
*
*    return_value - A pointer to the image value of the specified row.
*
*    RETURN VALUES:
*
*    A pointer to the image value of the specified row is returned.
*    If the specified row does not exist, NULL is returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Image *ListChoice::getValue (int row_number, Image* &return_value)
{ 
  return_value = (Image *) NULL;

  // If the row exists...
  if ((row_number > 0) && (row_number <= getNumberOfChoices ())) {
    row_number--;

    // If the XView object already exists...
    if (owObject) {
      // Get the items client data
      ListClientData *client_data = (ListClientData *)
	                            xv_get (owObject,
					    PANEL_LIST_CLIENT_DATA, row_number,
					    NULL);

      // Get the image stored in the client data
      return_value = ((client_data) ? client_data->image : (Image *) NULL);
    }
    // Else, this is a pre-XView list...
    else
      return_value = (Image *) images [row_number];
  }
    
  return (return_value);
}

/*F+ void ListChoice::replaceChoice (int row_number, char *entry)
*
*    PURPOSE:
*
*    Replaces the string value of the specified row.  If the row number
*    is higher then the existing number of rows, a new item is placed
*    at the end of the list.
*
*    PARAMETERS:
*
*    entry - The new string value.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::replaceChoice (int row_number, char *entry)
{
  // If the row exists...
  if ((row_number > 0) && (row_number <= getNumberOfChoices ())) {
    row_number--;
    
    // If the XView object already exists...
    if (owObject) {
      // If a font is specified...
      if (listFont) {
	// Create the XView font object, if necessary
	if (!listFont->getXViewObject ())
	  ((UIObject *) listFont)->createObject (this);
      
	xv_set (owObject, PANEL_LIST_STRING, row_number, entry,
		          PANEL_LIST_FONT,   row_number,
		                             listFont->getXViewObject (),
		NULL);
      }
      // Else, no font specified
      else
	xv_set (owObject, PANEL_LIST_STRING, row_number, entry, NULL);
    }
    // Else, pre-XView list...
    else {
      char *temp_string = strcpy (new char [strlen (entry) + 1], entry);

      delete ((char *) choices [row_number]);
      choices [row_number] = (Generic *) temp_string;
    }
  }
  // Else, add a new row...
  else
    (void) addChoice (entry);
}

/*F+ void ListChoice::replaceChoice (int row_number, Image &entry)
*
*    PURPOSE:
*
*    Replaces the image value of the specified row.  If the row number
*    is higher then the existing number of rows, a new item is placed
*    at the end of the list.
*
*    PARAMETERS:
*
*    entry - The new image value.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::replaceChoice (int row_number, Image &entry)
{
  // If the row exists...
  if ((row_number > 0) && (row_number <= getNumberOfChoices ())) {
    row_number--;
    
    // If the XView object already exists...
    if (owObject) {
      // Get the client data for the row
      ListClientData *client_data = (ListClientData *)
	                            xv_get (owObject,
					    PANEL_LIST_CLIENT_DATA, row_number,
					    NULL);

      // Set the client data image value
      client_data->image = &entry;

      // Set the row XView image
      ((UIObject *) &entry)->createObject (this);
      xv_set (owObject,
	      PANEL_LIST_GLYPH, row_number, entry.getXViewObject (),
	      NULL);
    }
    // Else, pre-XView list...
    else
      images [row_number] = (Generic *) &entry;
  }
  // Else, add a new row...
  else
    (void) addChoice (entry);
}

/*F+ void ListChoice::replaceChoice (int    row_number,
*                                    char  *string,
*                                    Image &image)
*
*    PURPOSE:
*
*    Replaces the string and image values of the specified row.  If the row
*    number is higher then the existing number of rows, a new item is placed
*    at the end of the list.
*
*    PARAMETERS:
*
*    entry - The new image value.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::replaceChoice (int row_number, char *string, Image &image)
{
  // If the row exists...
  if ((row_number > 0) && (row_number <= getNumberOfChoices ())) {
    row_number--;
    
    // If the XView object already exists...
    if (owObject) {
      // Get the client data for the row
      ListClientData *client_data = (ListClientData *)
	                            xv_get (owObject,
					    PANEL_LIST_CLIENT_DATA, row_number,
					    NULL);

      // Set the client data UIC image value
      client_data->image = &image;

      // Create the XView image
      ((UIObject *) &image)->createObject (this);

      // If a font is specified...
      if (listFont) {
	// Create the XView font object, if necessary
	if (!listFont->getXViewObject ())
	  ((UIObject *) listFont)->createObject (this);
      
	xv_set (owObject, PANEL_LIST_STRING, row_number, string,
	                  PANEL_LIST_GLYPH,  row_number,
                                             image.getXViewObject (),
		          PANEL_LIST_FONT,   row_number,
		                             listFont->getXViewObject (),
		NULL);
      }
      // Else, no font specified
      else
	xv_set (owObject, PANEL_LIST_STRING, row_number, string,
	                  PANEL_LIST_GLYPH,  row_number,
                                             image.getXViewObject (),
                NULL);
    }
    // Else, pre-XView list...
    else {
      char *temp_string = strcpy (new char [strlen (string) + 1], string);

      delete ((char *) choices [row_number]);

      choices [row_number] = (Generic *) temp_string;
      images [row_number] = (Generic *) &image;
      choiceData [row_number] = (Generic *) NULL;
    }
  }
  // Else, add a new row...
  else
    (void) addChoice (string, image);
}

/*F+ void ListChoice::setSelectedChoice (int row_number, Boolean flag)
*
*    PURPOSE:
*
*    Sets the selection status of the specified row.
*
*    PARAMETERS:
*
*    row_number - The number of the row to set the selection status of.
*    flag       - TRUE if the row is to set as selected, FALSE if not
*                 selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::setSelectedChoice (int row_number, Boolean flag)
{
  int num_choices = selections.getSize ();
  int selection = row_number-1;
      
  // If the XView object doesn't exist...
  if (!owObject) {
    // If the item is to be set on...
    if (flag) {
      // If not multiple choice...
      if (!multipleChoice && selections.getSize ())
        // Get rid of the old selection
	(void) selections.removeItem (0);
      // Else, multiple choice...
      else {
        /* If the selected item is already in the list, remove it
	   and place it at the end of the list */

        // Loop through the selections list...
        for (int i=0; i < num_choices; i++) {
  	  // If this is the specified item...
	  if (((int) selections [i]) == selection) {
	    // Remove the item from the list
	    (void) selections.removeItem (i);
	    break;
	  }
        }
      }

      // Add the new selection to the selections list
      selections.addItem ((Generic *) selection);
    }
    // Else turn the selection off...
    else {
      // Loop through the selections list...
      for (int i=0; i < num_choices; i ++) {
	// If this is the specified item...
	if (((int) selections [i]) == selection) {
	  (void) selections.removeItem (i);
	  break;
	}
      }
    }
  }
  // Else, the XView object exists...
  else {
    xv_set (owObject, PANEL_LIST_SELECT, selection, flag, NULL);
  }

  lastSelected = selection;
  selectStatus = (flag ? SELECT : DESELECT);
}

/*F+ void ListChoice::setSelectedChoice (char *row_value, Boolean flag)
*
*    PURPOSE:
*
*    Sets the selection status of the specified row.
*
*    PARAMETERS:
*
*    row_value - The string value of the row to set the selection status of.
*    flag      - TRUE if the row is to set as selected, FALSE if not
*                selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::setSelectedChoice (char *row_value, Boolean flag)
{
  int   num_choices = getNumberOfChoices ();
  char *temp_string;

  // If the XView object doesn't exist...
  if (!owObject) {
    // Loop through all of the choices...
    for (int i = 0; i < num_choices ; i++) {
      temp_string = (char *) choices [i];
	
      // If this is the specified item...
      if (!strcmp (temp_string, row_value)) {
	// Turn the choice on or off
	setSelectedChoice (i+1, flag);
	break;
      }
    }
  }
  // Else, the XView object already exists...
  else {
    char *temp_string;

    // Loop through all the choices...
    for (int i = 0; i < num_choices; i++) {
      temp_string = (char *) xv_get (owObject, PANEL_LIST_STRING, i);

      // If this is the specified choice...
      if (!strcmp (temp_string, row_value)) {
	xv_set (owObject, PANEL_LIST_SELECT, i, flag, NULL);

        // Save the selection information...
	lastSelected = i;
	selectStatus = (flag ? SELECT : DESELECT);
	break;
      }
    }
  }
}

/*F+ void ListChoice::setSelectedChoice (Image &row_value, Boolean flag)
*
*    PURPOSE:
*
*    Sets the selection status of the specified row.
*
*    PARAMETERS:
*
*    row_value - The image value of the row to set the selection status of.
*    flag      - TRUE if the row is to set as selected, FALSE if not
*                selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::setSelectedChoice (Image &row_value, Boolean flag)
{
  // If the XView object doesn't exist...
  if (!owObject) {
    // Find the image in the choice list
    int row_index = images.findItem ((Generic *) &row_value);

    // Turn the selection on or off
    setSelectedChoice (row_index+1, flag);
  }
  else {
    ListClientData *client_data;
    int             num_choices = getNumberOfChoices ();

    // Loop through all of the choices...
    for (int i = 0; i < num_choices; i++) {
      // Get the client data for the list item
      client_data = (ListClientData *) xv_get (owObject,
					       PANEL_LIST_CLIENT_DATA,
					       i);

      // If this is the specified item...
      if (client_data->image == &row_value) {
	xv_set (owObject, PANEL_LIST_SELECT, i, flag, NULL);

        // Save the selection information...
	lastSelected = i;
	selectStatus = (flag ? SELECT : DESELECT);
	break;
      }
    }
  }
}

/*F+ Boolean ListChoice::isSelectedChoice (int row_number)
*
*    PURPOSE:
*
*    Determines whether or not the specified row has been selected.
*
*    RETURN VALUES:
*
*    Returns TRUE if the specified row has been selected.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Boolean ListChoice::isSelectedChoice (int row_number)
{
  int     choice = row_number - 1;
  Boolean selected = FALSE;
  
  // If the XView object doesn't exist...
  if (!owObject) {
    int num_selected = selections.getSize ();

    // Loop throught the selections...
    for (int i=0; i < num_selected; i++)
      // If this one matches...
      if (choice == (int) selections [i])
	selected = TRUE;
  }
  else {
    // Use XView to see if the choice has been selected
    selected = (Boolean) xv_get (owObject, PANEL_LIST_SELECTED, choice);
  }

  return selected;
}

/*F+ int ListChoice::notifyProc (Panel_item     item,
*                                char          *string,
*                                void          *client_data,
*                                Panel_list_op  op,
*                                Event         *event,
*				 int		row)
*
*    PURPOSE:
*
*    Internal notify proc wrapper for the ListChoice class.  This is
*    called by XView when a notify occurs.  This proc then calls the user
*    specified UIC target and notify handlers.
*
*    PARAMETERS:
*
*    item        - The XView object the notify occurred on.
*    string      - The string value of the selected item in the XView object.
*    client_data - The client data associated with the object the notify
*                  occurred on.
*    op          - The XView operation:
*
*                      PANEL_LIST_OP_SELECT
*                      PANEL_LIST_OP_DESELECT
*                      PANEL_LIST_OP_DELETE
*                      PANEL_LIST_OP_VALIDATE
*
*    event       - The XView event information structure
*    row	 - The row being operated on.
*
*    RETURN VALUES:
*
*    The use specified notify result is returned.  XV_OK is returned by
*    default.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ListChoice::notifyProc (Panel_item     item,
			    char          *string,
			    void          *client_data,
			    Panel_list_op  op,
			    Event         *event,
			    int            row)
{
  ListClientData *data;

  ListChoice *object = (ListChoice *) xv_get (item,
					      XV_KEY_DATA,
					      UIC_OBJECT_KEY);
  
  // Save the number of the row being operated on
  object->lastSelected = row;
  
  if (op == PANEL_LIST_OP_VALIDATE) {
    // Create a new client data structure to be stored with the new item
    data = new ListClientData (++object->rowID);

    // Set the new client data on the newly inserted item
    xv_set (item, PANEL_LIST_CLIENT_DATA, object->lastSelected, data, NULL);

    // Set the value of the newly inserted item
    xv_set (item, PANEL_LIST_STRING, object->lastSelected, string, NULL);
  }
  
  // Set the UIC selection status
  if (op == PANEL_LIST_OP_SELECT)
    object->selectStatus = SELECT;
  else if (op == PANEL_LIST_OP_DESELECT)
    object->selectStatus = DESELECT;
  else if (op == PANEL_LIST_OP_VALIDATE)
    object->selectStatus = VALIDATE;
  else if (op == PANEL_LIST_OP_DELETE)
    object->selectStatus = DELETE;
  else
    object->selectStatus = UNKNOWN;
  
  // Set the UIC internal event information
  object->setEventInformation (event);

  // Notify any targets associated with this object
  object->notifyNotifyTargets ();
  
  // Call the user specified handler
  if (object->useDefaultNotifyHandlerFlag)
    object->defaultNotifyHandler ();
  else if (object->notifyHandler)
    (*object->notifyHandler) (object);
  
  // If this was a delete operation or an insert that was not validated
  if (((op == PANEL_LIST_OP_VALIDATE) && (object->notifyResult == XV_ERROR)) ||
      (op == PANEL_LIST_OP_DELETE)) {
    // Zero out the client data for this item
    xv_set (item, PANEL_LIST_CLIENT_DATA, object->lastSelected, 0, NULL); 

    // If old client data existed, delete it
    if (data) delete data;
  }
  
  // Erase the UIC internal event information
  object->resetEventInformation ();

  // If the user specified a notify status
  if (object->notifyStatus != UIC_NOT_SET) {
    // Set the notify status
    xv_set (item, PANEL_NOTIFY_STATUS, object->notifyStatus, NULL);

    // Reset the internal notify status
    object->notifyStatus = UIC_NOT_SET;
  }

  // If the user specified a notify result
  if (object->notifyResult != UIC_NOT_SET) {
    // Save the result
    int result = object->notifyResult;

    // Reset the internal notify result
    object->notifyResult = UIC_NOT_SET;

    return (result);
  }

  return (XV_OK);
}

/*F+ void ListChoice::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object of the ListChoice object.
*
*    PARAMETERS:
*
*    parent - The UIC parent object (a ComponentDisplay).
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ListChoice::createObject (UIObject *parent)
{
  /* NOTE: row height, label fonts, and read only status must be specified
           at the time of the xv_create call... */

  if (resourceName) {
    // If row height was specified...
    if (rowHeight != UIC_NOT_SET) {
      // If a label font was specified...
      if (labelFont) {
	// Create the XView label font object...
	((UIObject *) labelFont)->createObject (parent);
	
	// If this list is read only...
	if (readOnly)
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				XV_INSTANCE_NAME, resourceName,
				PANEL_LABEL_FONT, labelFont->getXViewObject (),
				PANEL_READ_ONLY,	 TRUE,
				XV_USE_DB,
				  PANEL_LIST_DISPLAY_ROWS, numRows,
				  PANEL_LIST_ROW_HEIGHT,   rowHeight,
				  NULL,
				NULL);
	// Else, not read only...
	else
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				XV_INSTANCE_NAME, resourceName,
				PANEL_LABEL_FONT, labelFont->getXViewObject (),
				XV_USE_DB,
				  PANEL_LIST_DISPLAY_ROWS, numRows,
				  PANEL_LIST_ROW_HEIGHT,   rowHeight,
				  NULL,
				NULL);
      }
      // Else, no label font specified...
      else {
	// If this list is read only...
	if (readOnly)
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				XV_INSTANCE_NAME, resourceName,
				PANEL_READ_ONLY,	 TRUE,
				XV_USE_DB,
				  PANEL_LIST_DISPLAY_ROWS, numRows,
				  PANEL_LIST_ROW_HEIGHT,   rowHeight,
				  NULL,
				NULL);
	// Else, not read only...
	else
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				XV_INSTANCE_NAME, resourceName,
				XV_USE_DB,
				  PANEL_LIST_DISPLAY_ROWS, numRows,
				  PANEL_LIST_ROW_HEIGHT,   rowHeight,
				  NULL,
				NULL);
      }
    }
    // Else, row height was not specified...
    else {
      // If a label font was specified...
      if (labelFont) {
	// Create the XView label font object...
	((UIObject *) labelFont)->createObject (parent);
	
	// If read only was specified...
	if (readOnly)
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				XV_INSTANCE_NAME, resourceName,
				PANEL_LABEL_FONT, labelFont->getXViewObject (),
				PANEL_READ_ONLY,	 TRUE,
				XV_USE_DB,
				  PANEL_LIST_DISPLAY_ROWS, numRows,
				  NULL,
				NULL);
	// Else, read only was not specified...
	else
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				XV_INSTANCE_NAME, resourceName,
				PANEL_LABEL_FONT, labelFont->getXViewObject (),
				XV_USE_DB,
				  PANEL_LIST_DISPLAY_ROWS, numRows,
				  NULL,
				NULL);
      }
      // Else, no label font specified
      else {
	// If read only was specified...
	if (readOnly)
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				XV_INSTANCE_NAME, resourceName,
				PANEL_READ_ONLY,	 TRUE,
				XV_USE_DB,
				  PANEL_LIST_DISPLAY_ROWS, numRows,
				  NULL,
				NULL);
	// Else, read only was not specified...
	else
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				XV_INSTANCE_NAME, resourceName,
				XV_USE_DB,
				  PANEL_LIST_DISPLAY_ROWS, numRows,
				  NULL,
				NULL);
      }
    }
  }
  else {
    // If row height was specified...
    if (rowHeight != UIC_NOT_SET) {
      // If a label font was specified...
      if (labelFont) {
	// Create the XView label font object...
	((UIObject *) labelFont)->createObject (parent);
	
	// If this list is read only...
	if (readOnly)
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				PANEL_LABEL_FONT, labelFont->getXViewObject (),
				PANEL_READ_ONLY,	 TRUE,
				PANEL_LIST_DISPLAY_ROWS, numRows,
				PANEL_LIST_ROW_HEIGHT,	 rowHeight,
				NULL);
	// Else, not read only...
	else
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				PANEL_LABEL_FONT, labelFont->getXViewObject (),
				PANEL_LIST_DISPLAY_ROWS, numRows,
				PANEL_LIST_ROW_HEIGHT,	 rowHeight,
				NULL);
      }
      // Else, no label font specified...
      else {
	// If this list is read only...
	if (readOnly)
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				PANEL_READ_ONLY,	 TRUE,
				PANEL_LIST_DISPLAY_ROWS, numRows,
				PANEL_LIST_ROW_HEIGHT,	 rowHeight,
				NULL);
	// Else, not read only...
	else
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				PANEL_LIST_DISPLAY_ROWS, numRows,
				PANEL_LIST_ROW_HEIGHT,	 rowHeight,
				NULL);
      }
    }
    // Else, row height was not specified...
    else {
      // If a label font was specified...
      if (labelFont) {
	// Create the XView label font object...
	((UIObject *) labelFont)->createObject (parent);
	
	// If read only was specified...
	if (readOnly)
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				PANEL_LABEL_FONT, labelFont->getXViewObject (),
				PANEL_READ_ONLY,	 TRUE,
				PANEL_LIST_DISPLAY_ROWS, numRows,
				NULL);
	// Else, read only was not specified...
	else
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				PANEL_LABEL_FONT, labelFont->getXViewObject (),
				PANEL_LIST_DISPLAY_ROWS, numRows,
				NULL);
      }
      // Else, no label font specified
      else {
	// If read only was specified...
	if (readOnly)
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				PANEL_READ_ONLY,	 TRUE,
				PANEL_LIST_DISPLAY_ROWS, numRows,
				NULL);
	// Else, read only was not specified...
	else
	  owObject = xv_create (parent->getXViewObject (), PANEL_LIST,
				PANEL_LIST_DISPLAY_ROWS, numRows,
				NULL);
      }
    }
  }

  // Set the XView notify proc
  xv_set (owObject, PANEL_NOTIFY_PROC, &ListChoice::notifyProc, NULL);
 
  // Save the "this" pointer as key data
  xv_set (owObject, XV_KEY_DATA, UIC_OBJECT_KEY, this, NULL);
  
  // Set the XView help data
  setHelpData ();

  if (listTitle)
    xv_set (owObject, PANEL_LIST_TITLE, listTitle, NULL);
  
  if (multipleChoice)
    xv_set (owObject, PANEL_CHOOSE_ONE, FALSE, NULL);
  else
    xv_set (owObject, PANEL_CHOOSE_ONE, TRUE, NULL);
  
  if (selectionRequired)
    xv_set (owObject, PANEL_CHOOSE_NONE, FALSE, NULL);
  else
    xv_set (owObject, PANEL_CHOOSE_NONE, TRUE, NULL);
  
  if (xLoc != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_X, xLoc, NULL, NULL);
    else
      xv_set (owObject, XV_X, xLoc, NULL);
  }
  
  if (yLoc != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_Y, yLoc, NULL, NULL);
    else
      xv_set (owObject, XV_Y, yLoc, NULL);
  }
  
  if (width != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_LIST_WIDTH, width, NULL, NULL);
    else
      xv_set (owObject, PANEL_LIST_WIDTH, width, NULL);
  }
  
  if (height != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, XV_HEIGHT, height, NULL, NULL);
    else
      xv_set (owObject, XV_HEIGHT, height, NULL);
  }
  
  if (valueX != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_VALUE_X, valueX, NULL, NULL);
    else
      xv_set (owObject, PANEL_VALUE_X, valueX, NULL);
  }

  if (valueY != UIC_NOT_SET) {
    if (resourceName)
      xv_set (owObject, XV_USE_DB, PANEL_VALUE_Y, valueY, NULL, NULL);
    else
      xv_set (owObject, PANEL_VALUE_Y, valueY, NULL);
  }

  initializeColorMap (parent);
  
  if (foregroundColor != UIC_NOT_SET)
    xv_set (owObject, PANEL_ITEM_COLOR, foregroundColor, NULL);
  
  if (label != (char *) NULL)
    xv_set (owObject, PANEL_LABEL_STRING, label, NULL);
  else if (imageLabel) {
    ((UIObject *) imageLabel)->createObject (parent);
    xv_set (owObject,
	    PANEL_LABEL_IMAGE, imageLabel->getXViewObject (),
	    NULL);
  }
  
  if (labelPosition == ABOVE)
    xv_set (owObject, PANEL_LAYOUT, PANEL_VERTICAL, NULL);
  
  if (listMenu) {
    ((UIObject *) listMenu)->createObject (parent);
    xv_set (owObject, PANEL_ITEM_MENU, listMenu->getXViewObject (), NULL);
  }

  // The XV_SHOW setting should be done before items are placed in the list
  if (!showFlag) xv_set (owObject, XV_SHOW, FALSE, NULL);
  
  if (listFont) ((UIObject *) listFont)->createObject (parent);
  
  ListClientData *data;
  int num_choices = choices.getSize ();
  for (int i = 0; i < num_choices; i++) {
    xv_set (owObject, PANEL_LIST_INSERT, i, NULL);
    
    if (listFont)
      xv_set (owObject,
	      PANEL_LIST_FONT, i, listFont->getXViewObject (),
	      NULL);
    
    if ((char *) choices [i]) {
      xv_set (owObject,
	      PANEL_LIST_STRING, i, (char *) choices [i],
	      NULL);
      
      /* Once the XView object has the string, we no longer need an
	 internal copy */
      delete ((char *) choices [i]);
      choices [i] = (Generic *) NULL;
    }
    else
      xv_set (owObject, PANEL_LIST_STRING, i, " ", NULL);
    
    if ((Image *) images [i]) {
      ((UIObject *) images [i])->createObject (parent);
      xv_set (owObject,
	      PANEL_LIST_GLYPH, i, ((Image *) images [i])->getXViewObject (),
	      NULL);
    }

    /* All entries have client data that is used to provide a unique
       ID for the row and to store the UIC image associated with the row */
    data = new ListClientData (++rowID,
			       (Image *) images [i],
			       choiceData [i]); 
    xv_set (owObject, PANEL_LIST_CLIENT_DATA, i, data, NULL);
  }
  
  // If the user specified an event handler...
  if (eventHandler || useDefaultEventHandlerFlag || eventTargets.getSize () ||
      draggable || hotRegions.getSize ()) {
    xv_set (owObject, PANEL_EVENT_PROC, &UIComponent::eventProc, NULL);
    
    // Specify the events to consume...
    int num_events = events.getSize ();
    for (i = 0; i < num_events; i++)
      xv_set (owObject,
	      WIN_CONSUME_EVENT, (int) events [i],
	      NULL);
  }
  
  // Set the pre-selected selections
  int num_selected = selections.getSize ();
  for (i = 0 ; i < num_selected; i++)
    xv_set (owObject,
	    PANEL_LIST_SELECT, (int) selections [i], TRUE,
	    NULL);

  // Set the pre-selected "last" selection
  if (num_selected > 0) {
    lastSelected = (int) selections [num_selected-1];
    selectStatus = SELECT;
  }
  else if (selectionRequired) {
    lastSelected = 0; 
    selectStatus = SELECT;
  }
  
  // Clear all of the internal lists, the XView object maintains all of this
  selections.dropItems ();
  choiceColors.dropItems ();
  choices.dropItems ();
  images.dropItems ();
  choiceData.dropItems ();

  if (!active) xv_set (owObject, PANEL_INACTIVE, TRUE, NULL);

  UIObject::setUIObjectAttributes (parent);
}

#ifdef TEST
#endif /* TEST */
