/*H+ FileChooser.h
*
*    SCCS ID: @(#)FileChooser.h 1.2 93/04/09 SMI
*
*    Copyright (c) 1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the FileChooser class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ FileChooser
*
*    DESCRIPTION:
*
*    A FileChooser object provides a dialog box for choosing files.
*
*    INHERITED CLASSES:
*
*    PopUpWindow - Provides the same basic feature of a PopUpWindow.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setType (File_chooser_type new_type) - Specifies the type of
*    file chooser:  FILE_CHOOSER_OPEN, FILE_CHOOSER_SAVE, FILE_CHOOSER_SAVEAS.
*                
*    void setDefaultSaveAsName (const char *name) - Specifies the default
*    name if the file chooser is a "save as" file chooser.
*
*    void setSaveToDirectory (Boolean flag) - Specifies that the file chooser
*    only needs to get a path from the user.
*
*    void setFilterString (const char *string) - Specifies a regular expression
*    that specoifies the files to be displayed by the file chooser.
*
*    void setShowDotFiles (Boolean flag) - Specifies whether or not dot files
*    should be displayed by the file chooser.
*
*    char *getChosenPath () - Get the last chosen path.
*
*    char *getChosenFile () - Get the last chosen file.
*
*    void setNotifyHandler (UICHandlerFunction) - Specifies the notify handler.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual  void createObject (UIObject *parent = NULL) - Creates the
*             XView object.
*
*    static   void notifyProc (File_chooser, char *, char *, Xv_opaque) -
*             Wrapper for the XView file chooser notify proc.
*
*    void     useDefaultNotifyHandler (Boolean flag) - Specifies whther or not the
*    default notify handler should be used.  Call this with flag=TRUE in the
*    constructor of a derived class and then the notify behavior can be written
*    into the virtual member function "defaultNotifyHandler".
*
*    virtual  void defaultNotifyHandler () - Allows encapsulation of notify
*    behavior.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*             void   setFileChooserDefaults () - Initializes the data members.
*
*C-*/
#ifndef _FILE_CHOOSER_H
#define _FILE_CHOOSER_H

#include <xview/xv_version.h>
#if (XV_VERSION_NUMBER >= 3200)

// Includes
#include <xview/file_chsr.h>

#include "PopUpWindow.h"

// Class Definition
class FileChooser : public PopUpWindow {
public:
    		 FileChooser ();
    		 FileChooser (char *);
   		~FileChooser ();

		 void setType (File_chooser_type new_type)
		   {
		     type = new_type;

		     if (owObject)
		       xv_set (owObject, FILE_CHOOSER_TYPE, type, NULL);
		   }

		 void setDefaultSaveAsName (const char *name)
		   {
		     if (defaultSaveAsName) delete defaultSaveAsName;

		     defaultSaveAsName = (name ?
					  strcpy (new char [strlen (name) + 1],
						  name) :
					  (char *) NULL);

		     if (owObject)
		       xv_set (owObject,
			       FILE_CHOOSER_DOC_NAME, defaultSaveAsName,
			       NULL);
		   }

		 void setSaveToDirectory (Boolean flag)
		   {
		     saveToDirectory = flag;
		     saveToDirectorySet = TRUE;

		     if (owObject)
		       xv_set (owObject,
			       FILE_CHOOSER_SAVE_TO_DIR, saveToDirectory,
			       NULL);
		   }

		 void setFilterString (const char *string)
		   {
		     if (filterString) delete filterString;

		     filterString = (string ?
				     strcpy (new char [strlen (string) + 1],
					     string) :
				     (char *) NULL);

		     if (owObject)
		       xv_set (owObject,
			       FILE_CHOOSER_FILTER_STRING, filterString,
			       NULL);
		   }

		 void setShowDotFiles (Boolean flag)
		   {
		     showDotFiles = flag;
		     showDotFilesSet = TRUE;

		     if (owObject)
		       xv_set (owObject,
			       FILE_CHOOSER_SHOW_DOT_FILES, showDotFiles,
			       NULL);
		   }

		 char *getChosenPath ()
		   {
		     return chosenPath;
		   }

		 char *getChosenFile ()
		   {
		     return chosenFile;
		   }

		 void setNotifyHandler (UICHandlerFunction handler)
		   {
		     notifyHandler = handler;

		     if (owObject && !notifyHandlerSet) {
		       notifyHandlerSet = TRUE;

		       xv_set (owObject,
			       FILE_CHOOSER_NOTIFY_FUNC,
				 &FileChooser::notifyProc,
			       NULL);
		     }
		   }

protected:
    File_chooser_type   type;
    char               *defaultSaveAsName;
    Boolean             saveToDirectory;
    Boolean             saveToDirectorySet;
    char               *filterString;
    Boolean             showDotFiles;
    Boolean             showDotFilesSet;
    char               *chosenPath;
    char               *chosenFile;
    UICHandlerFunction  notifyHandler;
    Boolean		notifyHandlerSet;
    Boolean             useDefaultNotifyHandlerFlag;

    void useDefaultNotifyHandler (Boolean flag)
      {
	useDefaultNotifyHandlerFlag = flag;
      }

    virtual void defaultNotifyHandler () {}

    virtual void createObject (UIObject *parent = NULL);
    static  void notifyProc (File_chooser, char *, char *, Xv_opaque);

private:
    void	 setFileChooserDefaults ();
};

#endif

#endif /* _FILE_CHOOSER_H */
