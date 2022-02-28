/*H+ ColorMap.h
*
*    SCCS ID: @(#)ColorMap.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the ColorMap class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    JDZ       05/02/91   Code review preparation
*
*H-*/

/*C+ ColorMap
*
*    DESCRIPTION:
*
*    The ColorMap object specifies the color for objects displayed
*    by applications.  One segment from the map defines one color.  
*    All GUI objects normally take their colors from a default map 
*    that OpenWindows provides, and which UIT supports.  However, if 
*    you choose, you can build your own ColorMap with this class. 
*    There is no maximum number of colors.  Once you set the color 
*    on any object in an application, you set it for all objects in 
*    that application (with the exception of the paint color map
*    on a ComponentDisplay object).  Once you use the "start ()"
*    function for the Notifier, the application color map is set.
*
*    INHERITED CLASSES:
*
*    UIObject - In addition to the normal benefits provided by the
*    inheritance mechanism, inheriting the UIObject allows this
*    class to be used as UIObject class inside the Handlers.
*
*    ORDINARY DATA MEMBERS:
*
*    GenericList       colors        - List of UIC color structures in the
*                                      color map.
*    GenericList       colorNames    - List of the names of the colors.
*    ColorMapType      cmsType       - The type of color map: DYNAMIC or
*                                      STATIC.
*    char	      *colorMapName  - The name of the XView color map.
*    long             *xColorIndices - Array of X color map indices
*                                      corresponding to the UIC color map
*                                      indices.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*            void    addColor (char *, int, int, int) - Adds the 
*            named color to the ColorMap.  Arguments are the color 
*            name, and the red, green and blue values of the color. 
*
*            void    addColor (char *, Color) - Adds the named color
*            to the ColorMap.  Arguments are the color name and the 
*            Color specification (rgb values).
* 
*            void    addColor (const char *) -  Finds the named color in 
*            the X11 database and adds it to the ColorMap.
*
*            int     findColor (Color) - Given the Color (rgb values),
*            returns the index of this color into the ColorMap segment.
*            
*            int     findColor (const char *) - Given the Color name,
*            returns the index of this color into the ColorMap segment.
*
*            void    changeColor (char *, int, int, int)*
*            void    changeColor (char *, Color)
*                    - Changes the RGB values of the named color. 
*
*            long    getXColorIndex (Color) - Returns the index of the
*            specified color in the X color map.  The color is specified by a
*            UIC color structure containing the RGB values of the color.
*
*            long    getXColorIndex (char *) - Returns the index of the
*            specified color in the X color map.  The color is specified by
*            name.
*
*            long    getXColorIndex (int) - Returns the index of the
*            specified color in the X color map.  The color is specified by
*            the index into the UIC color map.
*
*            Color  &getColorRGB (char *) - Returns a UIC color structure 
*            containing the RGB values of the named color.
*
*            void    setColorMapName (char *) - Sets the XView name of 
*            this ColorMap.  Default: Name = "UIC Color Map"
*
*            char   *getColorMapName () - Returns the XView name of
*            this ColorMap.
*
*            int     getDefaultBackgroundIndex (UIObject *parent) - Returns
*            the index of the default Background color into the ColorMap
*            segment.  This will return a valid index only after the main base
*	     window of the application has been created.
*
*            int     getDefaultForegroundIndex (UIObject *parent) - Returns
*            the index of the default Foreground color into the ColorMap
*            segment.  This will return a valid index only after the main base
*	     window of the application has been created.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void    createObject (UIObject *parent = NULL) -
*            Create XView object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setColorMapDefaults () - Initializes the data members and
*    creates the minimum UIT color map containing the XView CMS control
*    colors, and Black and White.
*
*C-*/
#ifndef _COLORMAP_H
#define _COLORMAP_H

// Includes
#include "GenericList.h"
#include "UIObject.h"

#include <xview/win_input.h>
#include <xview/rect.h>
#include <pixrect/pixrect.h>
#include <pixrect/pixfont.h>
#include <xview/frame.h>

// Class Definition
class ColorMap : public UIObject {
public:
    			 ColorMap ();
    			 ColorMap (ColorMapType);
			 ColorMap (GenericList, ColorMapType);
			~ColorMap ();

    void		 addColor (char *, int, int, int);
    void		 addColor (char *, Color);
    void		 addColor (const char *);

    int			 findColor (Color);
    int			 findColor (const char *);

    void		 changeColor (char *, int, int, int);
    void		 changeColor (char *, Color);

    long                 getXColorIndex (Color);
    long                 getXColorIndex (char *);
    long                 getXColorIndex (int);

    Color		&getColorRGB (char *);

    void		 setColorMapName (char *);
    char		*getColorMapName ();
    int                  getDefaultBackgroundIndex (UIObject *parent=NULL);
    int                  getDefaultForegroundIndex (UIObject *parent=NULL);

protected:
    GenericList		 colors;
    GenericList		 colorNames;
    ColorMapType	 cmsType;
    char		*colorMapName;
    long                *xColorIndices;

    virtual void	 createObject (UIObject *parent = NULL);

private:
    void		 setColorMapDefaults ();
};

#endif /* _COLORMAP_H */
