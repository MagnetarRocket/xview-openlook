/*H+ CharacterFont.h
*
*    SCCS ID: @(#)CharacterFont.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CharacterFont class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       10/04/90   Original creation.
*
*H-*/

/*C+ CharacterFont
*
*    DESCRIPTION:
*
*    Defines a class that allows creation of a font using the
*    XView Font package.
*
*    INHERITED CLASSES:
*
*    UIObject - The base class of all components in the UIC.
*
*    CONSTRUCTORS:
*
*    CharacterFont (char *name)   - Creates the font with the specified name.
*    CharacterFont (char *family, char *style, int size)
*                                 - Creates a font of the specified family,
*                                   style and size.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    char *getName ()             - Returns the name of the font.
*    char *getFamily ()           - Returns the font family name.
*    char *getStyle ()            - Returns the font style.
*    int   getSize ()             - Returns the size of the font.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *)
*                                 - Creates the XView font object.
*
*C-*/
#ifndef _CHARACTERFONT_H
#define _CHARACTERFONT_H

// Includes
#include "UIObject.h"

#include <xview/font.h>

// Class Definition
class CharacterFont : public UIObject {
public:
  CharacterFont (char *name)
    { 
      fontName = strcpy (new char [strlen (name) + 1], name);
    }
  CharacterFont (char *family, char *style, int size)
    {
      fontName = (char *) NULL;
      fontFamily = strcpy (new char [strlen (family) + 1], family);
      fontStyle = strcpy (new char [strlen (style) + 1], style);
      fontSize = size;
    }
  ~CharacterFont () { if (fontName) delete fontName;
		      if (fontFamily) delete fontFamily;
		      if (fontStyle) delete fontStyle; }

  char         *getName ();
  char         *getFamily ();
  char         *getStyle ();
  int           getSize ();

protected:
  char      *fontName;
  char      *fontFamily;
  char      *fontStyle;
  int        fontSize;

  virtual void  createObject (UIObject *);
};

#endif /* _CHARACTERFONT_H */
