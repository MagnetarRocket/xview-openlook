/*H+ TTYDisplay.h
*
*    SCCS ID: @(#)TTYDisplay.h 1.1 92/11/06 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the TTYDisplay class.
*
*    AUTHOR: Terry Glanfield and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ TTYDisplay
*
*    DESCRIPTION:
*
*    The class provides such terminal emulation capability as a TTY 
*    subwindow, which is like having a shell tool available. An application 
*    can be run or an interactive shell can be started.  This class uses
*    the XView TTY package (as opposed to using the TERMSW package).
*
*    INHERITED CLASSES:
*
*    UIDisplay - Supplies basic display capabilities.
*
*    ORDINARY DATA MEMBERS:
*
*    Boolean fdToSTDIO - Specifies whether or not the TTY file descriptor
*                        should be set to STDIN and STDOUT.
*    char **ttyArgs - Pointer to the parameters that define the terminal.
*
*    ORDINARY MEMBER FUNCTIONS:
*
*    void setFDToSTDIO - Sets the TTY FD to STDIN and STDOUT.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void        createDragSite () - Creates a drag site for this
*            object.
*
*    virtual void        createDropSite () - Creates a drop site for this
*            object.
*
*    virtual void createObject (UIObject *) - Creates the XView object.
*
*C-*/

#ifndef _TTY_DISPLAY_H
#define _TTY_DISPLAY_H

#include "UIDisplay.h"

#include <unistd.h>
#include <xview/win_input.h>
#include <xview/termsw.h>
#include <xview/tty.h>

class TTYDisplay : public UIDisplay
{
  public:
    TTYDisplay ();
    TTYDisplay (char *, ...);
    ~TTYDisplay ();

    void setFDToSTDIO ()
      {
	fdToSTDIO = TRUE;
	if (owObject) {
	  dup2 ((int) xv_get (owObject, TTY_TTY_FD), 0);
	  dup2 ((int) xv_get (owObject, TTY_TTY_FD), 1);
	}
      }

  protected:
    Boolean        fdToSTDIO;
    char         **ttyArgs;
 
    virtual void   createDragSite ();
    virtual void   createDropSite ();
    virtual void   createObject (UIObject *);

  private:
    void setTTYDisplayDefaults ();
};
 
#endif // _TTY_DISPLAY_H
