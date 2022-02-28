/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include <desktop/tt_c.h>

#include "Button.h"

class FileChooserButton : public Button {
public:
        FileChooserButton ();
        FileChooserButton (char *label);
        FileChooserButton (Image &image);
  void  setFileChooserButtonDefaults ();

       ~FileChooserButton ();

  void  setFileChooserTitle (char *);
  void  setFileChooserLeftFooter (char *);
  void  setFileChooserRightFooter (char *);
  void  setFileChooserButtonLabel (char *);
  void  setFileChooserDirectory (char *);
  void  setFileChooserFilePattern (char *);
  void  setFileChooserShowDotFiles (Boolean);

  void  setSelectionHandler (UICHandlerFunction);

  Boolean isSelectionReceived ();
  char *getSelectedDirectory ();
  char *getSelectedFile ();

protected:
  char    *fcTitle;
  char    *fcLeftFooter;
  char    *fcRightFooter;
  char    *fcButtonLabel;
  char    *fcDirectory;
  char    *fcFilePattern;
  Boolean  fcShowDotFiles;
  Boolean  selectionReceived;
  char    *selectedFile;
  char    *selectedDirectory;

  UICHandlerFunction selectionHandler;

  void defaultNotifyHandler ();

private:
  void clearSelections ();
};
