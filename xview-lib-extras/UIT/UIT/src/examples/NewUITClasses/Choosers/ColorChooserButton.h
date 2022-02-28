/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include <desktop/tt_c.h>

#include "Button.h"

class ColorChooserButton : public Button {
public:
        ColorChooserButton ();
        ColorChooserButton (char *label);
        ColorChooserButton (Image &image);
  void  setColorChooserButtonDefaults ();

       ~ColorChooserButton ();

  void  setColorChooserTitle (char *);
  void  setColorChooserLeftFooter (char *);
  void  setColorChooserRightFooter (char *);
  void  setColorChooserColor (char *);

  void  setSelectionHandler (UICHandlerFunction);

  Boolean isSelectionReceived ();
  char *getSelectedColor ();

protected:
  char    *ccTitle;
  char    *ccLeftFooter;
  char    *ccRightFooter;
  char    *ccColor;
  Boolean  selectionReceived;
  char    *selectedColor;

  UICHandlerFunction selectionHandler;

  void defaultNotifyHandler ();

private:
  void clearSelections ();
};
