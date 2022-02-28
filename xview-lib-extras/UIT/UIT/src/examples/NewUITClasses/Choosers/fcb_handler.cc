/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include "AlphanumericInput.h"
#include "BaseWindow.h"
#include "FileChooserButton.h"

/*
 * Notify callback function for `fcb'.
 */
void file_chooser_handler (UIObject *object)
{
  FileChooserButton *fcb = (FileChooserButton *) object;

  AlphanumericInput *dt = (AlphanumericInput *) object->getObjectData ("DIR");
  AlphanumericInput *ft = (AlphanumericInput *) object->getObjectData ("FILE");
  BaseWindow        *bw = (BaseWindow *) object->getObjectData ("BW");

  if (fcb->isSelectionReceived ()) {
    dt->setValue (fcb->getSelectedDirectory ());
    ft->setValue (fcb->getSelectedFile ());
    bw->setLeftFooter ("");
  }
  else {
    dt->setValue ("");
    ft->setValue ("");
    bw->setLeftFooter ("No Selection Specified"); 
  }

  return;
}
