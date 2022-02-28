/*H+ TextDisplay.h
*
*    SCCS ID: @(#)TextDisplay.h 1.2 92/05/23 SMI 
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the TextDisplay class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/07/91   Preparation for code review
*
*H-*/

/*C+ TextDisplay
*
*    DESCRIPTION:
*
*    The text displayed can be defined as editable.  The TextDisplay contains 
*    a scrollbar so that if the text cannot fit in the display area, the user 
*    may use the mouse pointer to scroll text up or down.  
*
*    INHERITED CLASSES:
*
*    UIDisplay - Supplies basic display capabilities.
*
*    ORDINARY DATA MEMBERS:
*
*    char		*file       - Pointer to the file being displayed. 
*    Boolean	 	 readOnly   - TRUE if the text is marked as
*                                     "read only".
*    Boolean		 ignoreEdits - TRUE if edits should be ignored
*				       when the display is destroyed.
*    Boolean	 	 modified   - TRUE if the text has been modified.
*    TextDisplayStatus	 loadStatus - The status of the last attempt to
*                                     load a file into this TextDisplay.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*            TextDisplayStatus	 loadFile (char *) - Loads the file and 
*            returns the status of the load operation.
*
*            void	         setReadOnly (Boolean) - Sets the file 
*            as read only.  Default: FALSE.
*
*	     void	         setIgnoreEdits (Boolean) - Specifies
*            to ignore edits when the display is destroyed.
*	     Default: FALSE.
*
*            Boolean	 	 getModified () - Checks if the file has 
*            been modified.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void 	 createObject (UIObject *) -  Create XView object.
*
*C-*/
#ifndef _TEXT_DISPLAY_H
#define _TEXT_DISPLAY_H

// Includes
#include "UIDisplay.h"

#include <xview/win_input.h>
#include <xview/textsw.h>

// Class Definition
class TextDisplay : public UIDisplay {
public:
    		 	 TextDisplay ();
			~TextDisplay ();

    TextDisplayStatus	 loadFile (char *);

    void	 	 setReadOnly (Boolean flag)
			 { readOnly = flag;
			   if (owObject) xv_set (owObject,
			                         TEXTSW_BROWSING, readOnly,
						 NULL); }

    void		 setIgnoreEdits (Boolean flag)
			 { ignoreEdits = flag;

			   if (owObject) {
			     if (!ignoreEdits)
			       xv_set (owObject, TEXTSW_IGNORE_LIMIT, 0, NULL);
			     else
			       xv_set (owObject,
				       TEXTSW_IGNORE_LIMIT, TEXTSW_INFINITY,
				       NULL);
			   }
			 }

    Boolean	 	 getModified ()
			 { if (owObject)
			     modified = (Boolean) xv_get (owObject, 
							  TEXTSW_MODIFIED);
			   else
			     modified = FALSE;
    			   return (modified); }

protected:
    char		*file;
    Boolean	 	 readOnly;
    Boolean	 	 ignoreEdits;
    Boolean	 	 modified;
    TextDisplayStatus	 loadStatus;

    virtual void 	 createObject (UIObject *);
};

#endif /* _TEXT_DISPLAY_H */

