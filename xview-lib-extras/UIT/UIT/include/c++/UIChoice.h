/*H+ UIChoice.h
*
*    SCCS ID: @(#)UIChoice.h 1.2 93/02/08 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the UIChoice class.
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

/*C+ UIChoice
*
*    DESCRIPTION:
*
*    A UIChoice object displays a set of various text and/or image fields.
*    A user may select one or more entries from the list set on whether or
*    not it was specified as multiple choice.
*
*    INHERITED CLASSES:
*
*    UIComponent - All choice classes are UIComponent's as well.
*
*    ORDINARY DATA MEMBERS:
*
*    GenericList    choices         - The list of choice text values.
*    GenericList    images          - The list of choice image values.
*    GenericList    choiceColors    - The list of choice colors.
*    GenericList    selections      - The list of currently selected choices.
*    int	    lastSelected    - The last selected choice.
*    int	    numColumns      - The number of columns to display.
*    int	    numRows         - The number of rows to display.
*    Boolean	    selectionRequired 
*				    - Indicates whether or not this object
*				      requires a selection.
*    Boolean	    multipleChoice  - Indicates whether or not this is a
*                                     multiple choice object.
*    CharacterFont *choiceFont      - The user specified font for choice text.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    virtual void deleteChoice (int)
*    virtual void deleteChoice (char *)
*    virtual void deleteChoice (Image &)
*         - Deletes the specified choice from the object.  The choice
*           may be specified by choice position, text value or image value.
*
*    virtual int addChoice (char *)
*    virtual int addChoice (Image &)
*         - Adds the specified text or image value as the last choice
*           in the choice object.
*
*    virtual int addChoice (int, char *);
*    virtual int addChoice (int, Image &);
*    virtual int addChoice (int, char *, Image &);
*         - Adds the specified text or image value at the specified
*           position in the choice object.
*
*    virtual void setChoices (char *, ...)
*         - Specifies a list of text values to be used as the
*           choice values in this object.
*
*    virtual void setChoiceColor (int, Color)
*    virtual void setChoiceColor (int, char *)
*    virtual void setChoiceColor (char *, Color)
*    virtual void setChoiceColor (char *, char *)
*    virtual void setChoiceColor (Image &, Color)
*    virtual void setChoiceColor (Image &, char *)
*         - Sets the color of the specified choice.  The choice is specified
*           by text value, image value or position.  The color is specified
*           by name or a UIC color structure that contains the RGB components
*           of the color.
*
*    virtual Image *getChoice (Image* &)
*    virtual char  *getChoice (char* &)
*    virtual int    getChoice (int &)
*         - Returns the text value, image value or position of the
*           last choice selected.
*
*    virtual char  *getChoice (int, char* &)
*    virtual Image *getChoice (int, Image* &)
*    virtual int    getChoice (int, int &)
*         - Returns the text value, image value or position of the
*           specified selected choice.  The choice is specified by
*           its position in the list of selected choices for this object.
*
*    virtual void   replaceChoice (int, char *)
*    virtual void   replaceChoice (int, Image &)
*	  - Replaces the text or image value of the specified choice
*           with the specified value.
*
*    virtual int getNumberSelected ()
*         - Returns the number of currently selected choices.
*
*    virtual void setSelectionRequired (Boolean selection)
*         - Specifies whether or not a selection is required.  If set on,
*           at least one selection will always be set.
*           Default: TRUE.
*
*    virtual void setMultipleChoice (Boolean)
*         - Specifies whether or not this is a multiple choice object.
*           Default: FALSE.
*
*    virtual void setDisplayedColumns (int)
*         - Specifies the number of columns to display.
*           Default: XView default.
*
*    virtual void setDisplayedRows (int)
*         - Specifies the number of rows to display.
*           Default: XView default.
*
*    virtual void setChoiceFont (CharacterFont &)
*         - Specifies the font to use to display choice text.
*           Default: Not set.
*
*    virtual void setSelectedChoice (int, Boolean flag = TRUE)
*    virtual void setSelectedChoice (char *, Boolean flag = TRUE)
*    virtual void setSelectedChoice (Image &, Boolean flag = TRUE)
*        - Sets the specified choice either on or off.  The choice can
*          be specified by text value, image value, or position.
*
*    virtual Boolean isSelectedChoice (int)
*        - Determines if the specified choice is selected.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createWithFonts (UIObject *, Xv_pkg *, void *)
*    virtual void createWithFonts (UIObject *,
*				   Xv_pkg *,
*				   Panel_attr,
*				   Panel_attribute_value,
*				   void *)
*    virtual void createWithFonts (UIObject *,
*				   Xv_pkg *,
*				   Panel_attr,
*				   Panel_attribute_value,
*				   Panel_attr,
*				   Panel_attribute_value,
*				   void *)
*        - Creates the XView choice object with all of the font settings
*          specified in the actual xv_create call.
*
*    void setUIChoiceAttributes (UIObject *)
*        - Sets common XView attributes.
*
*    void updateSelections (int, int);
*        - Updates the row position of items in the list of selected items.
*
*    virtual void showFeedback (int, Boolean)
*        - Displays the feedback resulting from the  selection or deselection
*          of a choice.
*
*    static void notifyProc (Panel_item, int, Event *)
*        - XView notify proc wrapper.  This is called when an XView
*          notify occurs on this object.
*
*    PRIVATE:
*
*    void setUIChoiceDefaults ()
*        - Initializes the data members.
*
*C-*/
#ifndef _UICHOICE_H
#define _UICHOICE_H

// Includes
#include "CharacterFont.h"
#include "GenericList.h"
#include "UIComponent.h"

// Class Definition
class UIChoice : public UIComponent {
public:
    			  UIChoice ();
    			  UIChoice (char *);
			  UIChoice (Image &);
			 ~UIChoice ();

    virtual void	  deleteChoice (int);
    virtual void	  deleteChoice (char *);
    virtual void	  deleteChoice (Image &);

    virtual int	  	  addChoice (char *);
    virtual int	  	  addChoice (Image &);

    virtual int	  	  addChoice (int, char *);
    virtual int	  	  addChoice (int, Image &);

    virtual void	  setChoices (char *, ...);

    virtual void	  setChoiceColor (int, Color);
    virtual void	  setChoiceColor (int, char *);
    virtual void	  setChoiceColor (char *, Color);
    virtual void	  setChoiceColor (char *, char *);
    virtual void	  setChoiceColor (Image &, Color);
    virtual void	  setChoiceColor (Image &, char *);

    virtual char	 *getChoice (char* &);
    virtual int 	  getChoice (int &);
    virtual Image	 *getChoice (Image* &);

    virtual char	 *getChoice (int, char* &);
    virtual int           getChoice (int, int &);
    virtual Image	 *getChoice (int, Image* &);

    virtual void          replaceChoice (int, char *);
    virtual void          replaceChoice (int, Image &);

    virtual int		  getNumberSelected ();

    virtual void	  setSelectionRequired (Boolean);
    virtual void	  setMultipleChoice (Boolean);
    virtual void	  setDisplayedColumns (int);
    virtual void	  setDisplayedRows (int);
    virtual void          setChoiceFont (CharacterFont &);

    virtual void	  setSelectedChoice (int, Boolean flag = TRUE);
    virtual void	  setSelectedChoice (char *, Boolean flag = TRUE);
    virtual void	  setSelectedChoice (Image &, Boolean flag = TRUE);

    virtual Boolean	  isSelectedChoice (int);

protected:
    GenericList		  choices;
    GenericList		  images;
    GenericList		  choiceColors;
    GenericList		  selections;
    int			  lastSelected;
    int			  numColumns;
    int			  numRows;
    Boolean		  selectionRequired;
    Boolean		  multipleChoice;
    CharacterFont        *choiceFont;

    virtual void	  createWithFonts (UIObject *, Xv_pkg *, void *);
    virtual void	  createWithFonts (UIObject *,
					   Xv_pkg *,
					   Panel_attr,
					   Panel_attribute_value,
					   void *);
    virtual void	  createWithFonts (UIObject *,
					   Xv_pkg *,
					   Panel_attr,
					   Panel_attribute_value,
					   Panel_attr,
					   Panel_attribute_value,
					   void *);

    void	  	  setUIChoiceAttributes (UIObject *);

    void                  updateSelections (int, int);
    virtual void	  showFeedback (int, Boolean);
 
    static  void	  notifyProc (Panel_item, int, Event *);

private:			  
    void		  setUIChoiceDefaults ();
};

#endif /* _UICHOICE_H */
