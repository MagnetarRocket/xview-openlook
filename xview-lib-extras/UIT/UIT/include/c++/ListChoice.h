/*H+ ListChoice.h
*
*    SCCS ID: @(#)ListChoice.h 1.4 93/06/01 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the ListChoice class.
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

/*C+ ListChoice
*
*    DESCRIPTION:
*
*    A ListChoice object displays a list of various text and/or image fields
*    in a scrolling region.  A user may select one or more entries from the
*    the list depending on whether or not it was specified as a multiple
*    choice list.
*
*    INHERITED CLASSES:
*
*    UIChoice - Abstract choice class.
*
*    LOCAL CLASSES:
*
*    ListClientData - The ListClientData class is used to store a unique
*                     row ID, the UIC image and the choice data corresponding
*		      to each row in the list.  The structure is attached to
*		      the row via the XView PANEL_LIST_CLIENT_DATA attribute.
*
*    ORDINARY DATA MEMBERS:
*
*    int	      rowHeight         - The pixel height of a row.
*    Boolean	      readOnly          - Specifies that the user can only
*                                         read the list or if the list can
*                                         be edited via the default XView
*                                         list menu.
*    SelectionStatus  selectStatus      - Saves the selection status for the
*                                          last operation on the list.
*    char            *listTitle	        - The list title.
*    UIMenu	     *listMenu          - Pointer to a user specified menu.
*    int              rowID             - Used for generating unique row IDs.
*    CharacterFont   *labelFont         - Pointer to the user specified font
*                                          for the panel list label.
*    CharacterFont   *listFont          - Pointer to the user specified font
*                                          for panel list row text.
*    Scroller	     *scroller		- Ths object's scrollbar.
*    GenericList      choiceData        - This object's choice data.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setRowHeight (int height) - Sets the pixel height of a row.
*                                     Default: XView default.
*
*    int  getRowHeight () - Gets the pixel height of a row.
*
*    void setDisplayedRows (int num_rows)
*         - Sets the number of rows to be displayed.  Default: 5.
*
*    void setReadOnly (Boolean readWrite)
*         - Allows specification of whether or not this list is read only
*           and will allow the user to edit the list via the edit menu.
*           Default: FALSE.
*
*    void setLabelFont (CharacterFont &font)
*         - Specifies the font to be used for the label.
*           Default: Not set.
*
*    void setListFont (CharacterFont &font)
*         - Specifies the font to be used for row text.
*           Default: Not set.
*
*    void setTitle (char *)
*	  - Sets the title of the list.
*
*    void setMenu (UIMenu &)
*         - Allows specification of a menu to be shown when the mouse menu
*           button is pressed.
*           Default: Not set (use XView default menu).
*
*    SelectionStatus getOperation ()
*         - Gets the the last operation performed on the list.
*
*    void deleteAllChoices ()
*         - Deletes all of the choices in the list.
*
*    virtual void deleteChoice (int)
*    virtual void deleteChoice (char *)
*    virtual void deleteChoice (Image &)
*            - Deletes the specified choice from the list.  The choice
*              may be specified by row position, text value or image value.
*
*    virtual int addChoice (char *)
*    virtual int addChoice (Image &)
*    virtual int addChoice (char *, Image &)
*            - Adds the specified text and/or image value as the last choice
*              in the list.
*
*    virtual int addChoice (int, char *)
*    virtual int addChoice (int, Image &)
*    virtual int addChoice (int, char *, Image &)
*            - Adds the specified text and/or image value at the specified
*              row position in the list.
*
*    virtual void setChoiceData (int, Generic*);
*    virtual void setChoiceData (int, Generic&);
*            - Specifies data to associate with the specified choice.
*
*    virtual Generic *getChoiceData (int);
*            - Gets the data associated with the specified choice.
*
*    virtual Image *getChoice (Image* &)
*    virtual char  *getChoice (char* &)
*    virtual int    getChoice (int &)
*            - Returns the text value, image value or row position of the
*              last choice operated on.
*
*    virtual char  *getChoice (int, char* &)
*    virtual Image *getChoice (int, Image* &)
*    virtual int    getChoice (int, int &)
*            - Returns the text value, image value or row position of the
*              specified selected choice.  The choice is specified by
*              its position in the list of selected items in this object.
*
*    virtual int getNumberSelected ()
*            - Returns the number of currently selected items.
*
*    virtual char  *getValue (char* &)
*    virtual Image *getValue (Image* &)
*            - Returns the text value, image value or row position of the
*              last choice operated on.
*    
*    virtual char  *getValue (int, char* &)
*    virtual Image *getValue (int, Image* &)
*            - Returns the text value, image value of the specified
*              choice.  The choice is specified by its row position in
*              the list.
*
*    Scroller *getScroller () - Returns a pointer to this object's scrollbar.
*
*    void replaceChoice (int, char *)
*    void replaceChoice (int, Image &)
*    void replaceChoice (int, char *, Image &)
*         - Replaces the text and/or image value of the specified choice.
*           The choice is specified by its row position in the list.
*
*    int getNumberOfChoices ()
*        - Returns the number of choices in the list.
*
*    virtual void setSelectedChoice (int, Boolean flag = TRUE)
*    virtual void setSelectedChoice (char *, Boolean flag = TRUE)
*    virtual void setSelectedChoice (Image &, Boolean flag = TRUE)
*        - Sets the specified choice either on or off.  The choice can
*          be specified by text value, image value, or row position.
*
*    virtual Boolean isSelectedChoice (int row_number)
*        - Determines if the specified choice is selected.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *)
*        - Creates the XView object.
*
*    static int notifyProc (Panel_item,
*                           char *,
*                           void *,
*                           Panel_list_op,
*                           Event *,
*			    int)
*               - Wrapper for the notify proc that XView will call
*                 for PANEL_LIST notification.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setListChoiceDefaults ()
*                   - Initializes the data memebers.
*
*C-*/
#ifndef _LIST_CHOICE_H
#define _LIST_CHOICE_H

// Includes
#include "CharacterFont.h"
#include "Scroller.h"
#include "UIChoice.h"
#include "UIMenu.h"

// Local Class Definitions
class ListClientData : public Generic {
public:
  ListClientData (int      row_id,
		  Image   *image_pointer = (Image *) NULL,
		  Generic *choice_data = (Generic *) NULL)
    {
      rowID = row_id;
      image = image_pointer;
      choiceData = choice_data;
    }

  int      rowID;
  Image   *image;
  Generic *choiceData;
};

// Class Definition
class ListChoice : public UIChoice {
public:
			 ListChoice ();
    			 ListChoice (char *);
			 ListChoice (Image &);
			~ListChoice ();

    void		 setWidth (int w)
			 { width = w;
			   if (owObject) {
			     if (resourceName)
			       xv_set (owObject,
				       XV_USE_DB,
					 PANEL_LIST_WIDTH, width,
				         NULL,
				       NULL);
			     else
			       xv_set (owObject,
				       PANEL_LIST_WIDTH, width,
				       NULL);
			   }
			 }

    void		 setRowHeight (int height)
			 { rowHeight = height;
			   if (owObject) {
			     if (resourceName)
			       xv_set (owObject,
				       XV_USE_DB,
				         PANEL_LIST_ROW_HEIGHT, rowHeight,
				         NULL,
				       NULL);
			     else
			       xv_set (owObject,
				       PANEL_LIST_ROW_HEIGHT, rowHeight,
				       NULL);
			   }
			 }

    int 		 getRowHeight ()
			 { return (owObject ?
				   (int) xv_get (owObject,
						 PANEL_LIST_ROW_HEIGHT,
						 NULL) :
				   rowHeight);
			 }

    void		 setDisplayedRows (int num_rows)
			 { numRows = num_rows;
			   if (owObject) {
			     if (resourceName)
			       xv_set (owObject, 
				       XV_USE_DB,
				         PANEL_LIST_DISPLAY_ROWS, numRows,
				         NULL,
				       NULL);
			     else
			       xv_set (owObject, 
				       PANEL_LIST_DISPLAY_ROWS, numRows,
				       NULL);
			   }
			 }

    void		 setReadOnly (Boolean readWrite)
		 	 { readOnly = readWrite;
			   if (owObject)
			     xv_set (owObject,
				     PANEL_READ_ONLY, readWrite,
				     NULL); }

    void                 setLabelFont (CharacterFont &font)
                         { labelFont = &font;
			   if (owObject) {
			     ((UIObject *) labelFont)->createObject (this);
			     xv_set (owObject,
				     PANEL_LABEL_FONT,
				     labelFont->getXViewObject (),
				     NULL);
			   }
			 }

    void                 setListFont (CharacterFont &font)
			 { listFont = &font; }

    void		 setTitle (char *);
    void		 setMenu (UIMenu &);

    SelectionStatus	 getOperation ()
			 { return (selectStatus); }

    void		 deleteAllChoices ();

    virtual void	 deleteChoice (int);
    virtual void	 deleteChoice (char *);
    virtual void	 deleteChoice (Image &);

    virtual int		 addChoice (char *);
    virtual int	 	 addChoice (Image &);
    virtual int		 addChoice (char *, Image &);

    virtual int 	 addChoice (int, char *);
    virtual int	 	 addChoice (int, Image &);
    virtual int	 	 addChoice (int, char *, Image &);

    virtual void         setChoiceData (int, Generic*);
    virtual void         setChoiceData (int, Generic&);

    virtual Generic     *getChoiceData (int);

    virtual Image	*getChoice (Image* &);
    virtual char	*getChoice (char* &);
    virtual int	 	 getChoice (int &);

    virtual char	*getChoice (int, char* &);
    virtual Image	*getChoice (int, Image* &);
    virtual int 	 getChoice (int, int &);

    virtual int		 getNumberSelected ();

    virtual char        *getValue (char* &);
    virtual Image       *getValue (Image* &);

    virtual char        *getValue (int, char* &);
    virtual Image	*getValue (int, Image* &);

    Scroller            *getScroller ()
			 { if (owObject && !scroller) {
			     scroller = new Scroller (VERTICAL);
			     scroller->setXViewObject
			       (xv_get (owObject, PANEL_LIST_SCROLLBAR));
			   }

			   return scroller;
			 }

    void		 replaceChoice (int, char *);
    void		 replaceChoice (int, Image &);
    void		 replaceChoice (int, char *, Image &);

    int			 getNumberOfChoices ()
			 { int num_choices;
			   if (owObject)
			     num_choices = (int)
					   xv_get (owObject, PANEL_LIST_NROWS);
			   else
			     num_choices = choices.getSize ();
			   return num_choices; }

    virtual void	 setSelectedChoice (int, Boolean flag = TRUE);
    virtual void	 setSelectedChoice (char *, Boolean flag = TRUE);
    virtual void	 setSelectedChoice (Image &, Boolean flag = TRUE);

    virtual Boolean	 isSelectedChoice (int row_number);

protected:
    int			 rowHeight;
    Boolean		 readOnly;
    SelectionStatus	 selectStatus;
    char		*listTitle;
    UIMenu		*listMenu;
    int                  rowID;
    CharacterFont       *labelFont;
    CharacterFont       *listFont;
    Scroller		*scroller;
    GenericList          choiceData;

    virtual void	 createObject (UIObject *);

    static  int          notifyProc (Panel_item,
				     char *,
				     void *,
				     Panel_list_op,
				     Event *,
				     int);

private:
    void		 setListChoiceDefaults ();
};

#endif /* _LIST_CHOICE_H */
