/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "AlphanumericInput.h"
#include "BaseWindow.h"
#include "ColorChooserButton.h"

/*
 * Notify callback function for `ccb'.
 */
void color_chooser_handler (UIObject *object)
{
  ColorChooserButton *ccb = (ColorChooserButton *) object;

  AlphanumericInput *ct = (AlphanumericInput *) object->getObjectData ("COLOR");
  BaseWindow        *bw = (BaseWindow *) object->getObjectData ("BW");

  if (ccb->isSelectionReceived ()) {
    ct->setValue (ccb->getSelectedColor ());
    ct->setForegroundColor (ccb->getSelectedColor ());
    bw->setLeftFooter ("");
  }
  else {
    ct->setValue ("");
    ct->setForegroundColor ("Foreground");
    bw->setLeftFooter ("No Selection Specified"); 
  }

  return;
}
