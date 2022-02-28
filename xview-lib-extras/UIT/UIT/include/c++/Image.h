/*H+ Image.h
*
*    SCCS ID: @(#)Image.h 1.2 93/04/09 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the Image class.
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

/*C+ Image
*
*    DESCRIPTION:
*
*    The Image class allows creation of an image using the
*    Xview Server Image package.
*
*    INHERITED CLASSES:
*
*    UIObject - The base class of all components in the UIC.
*
*    ORDINARY DATA MEMBERS:
*
*    char      loadIconErrorMsg [IL_ERRORMSG_SIZE];
*              - Error message buffer used by "icon_load_svrim" function
*                (See "xview/load_icon.h").
*    int       depth;
*              - The image depth (the number of color planes).
*    ColorMap *imageColorMap;
*              - The UIC color map to use to render color images.
*    char     *imageFile;
*              - Icon file name.
*    unsigned short *imageShorts;
*              - An array of shorts representing the image.
*    unsigned char  *imageBytes;
*              - An array of bytes representing the image.
*    PixelMap  pixelMap;
*              - An X11 pixmap representing the image.
*    ImageDataType imageDataType;
*              - The type of data that has been supplied.
*
*    CONSTRUCTORS:
*
*    Image (char *image_file)      - Creates an image from a file.
*    Image (int width, int height, unsigned short *image_bits, int depth=1)
*    Image (int width, int height, unsigned char *image_bits, int depth=1)
*    Image (int width, int height, PixelMap pixel_map, int depth=1)
*                                  - Creates an image from the specified bit
*                                    pattern.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setBitMapFile (Boolean)  - If an image file is used, this specifies
*                                    whether or not it is an X11 bit map file
*                                    or an icon file.  If TRUE, it is an X11
*                                    bit map file.  Default: FALSE (icon file).
*
*    void setImageColorMap (ColorMap&)
*				   - Specifies a UIC color map to be used
*				     for rendering color images. 
*                                    Default: Not set.
*
*    int getDepth ()               - Returns the plane depth of the image.
*                                    Black and white images have a depth of 1.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    void createObject (UIObject *parent=NULL)
*				   - Creates the XView image object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setImageDefaults () - Initializes the data members.
*
*C-*/
#ifndef _IMAGE_H
#define _IMAGE_H

// Includes
#include "UIObject.h"

#include <memory.h>

#include <xview/rect.h>
#include <xview/win_input.h>
#include <xview/icon.h>
#include <xview/icon_load.h>
#include <xview/svrimage.h>

// Type Definitions
enum ImageDataType { NO_IMAGE_DATA,
		     ICON_FILE,
		     BIT_MAP_FILE,
		     IMAGE_SHORTS,
		     IMAGE_BYTES,
		     PIXEL_MAP };

// Class Definition
class Image : public UIObject {
public:
  Image (char *image_file)
    {
      setImageDefaults ();

      imageDataType = ICON_FILE;
      imageFile = strcpy (new char [strlen (image_file) + 1], image_file);
    }

  Image (int w, int h, unsigned short *image_bits, int d=1)
    {
      setImageDefaults ();

      width = w;
      height = h;
      depth = d;

      imageDataType = IMAGE_SHORTS;

      int num_bytes = (w * h * d) / 8 + (((w * h * d) % 8) ? 1 : 0);
      num_bytes = num_bytes + (num_bytes % 2);
      imageShorts = new unsigned short [num_bytes / 2]; 
      memcpy (imageShorts, image_bits, num_bytes);
    }

  Image (int w, int h, unsigned char *image_bits, int d=1)
    {
      setImageDefaults ();

      width = w;
      height = h;
      depth = d;

      imageDataType = IMAGE_BYTES;

      int num_bytes = (w * h * d) / 8 + (((w * h * d) % 8) ? 1 : 0);
      imageBytes = new unsigned char [num_bytes]; 
      memcpy (imageBytes, image_bits, num_bytes);
    }

  Image (int w, int h, PixelMap pixel_map, int d=1)
    {
      setImageDefaults ();

      width = w;
      height = h;
      depth = d;

      imageDataType = PIXEL_MAP;
      pixelMap = pixel_map;
    }

  ~Image ()
    {
      if (imageFile) delete imageFile;
      if (imageShorts) delete imageShorts;
      if (imageBytes) delete imageBytes;
    }

  void	     setBitMapFile (Boolean flag)
	     { imageDataType = (flag ? BIT_MAP_FILE : ICON_FILE); }
		 

  void       setImageColorMap (ColorMap &color_map)
             { imageColorMap = &color_map; }

  int        getDepth ()
	     { if (owObject)
		 depth = (int) xv_get (owObject, SERVER_IMAGE_DEPTH);
	       return (depth); }

protected:
  char       loadIconErrorMsg [IL_ERRORMSG_SIZE];

  int             depth;
  ColorMap       *imageColorMap;
  char           *imageFile;
  unsigned short *imageShorts;
  unsigned char  *imageBytes;
  PixelMap        pixelMap;
  
  ImageDataType imageDataType;

  virtual void createObject (UIObject *parent=NULL);

private:
  void setImageDefaults ()
    {
      depth = UIC_NOT_SET;
      imageColorMap = (ColorMap *) NULL;
      imageFile = (char *) NULL;
      imageShorts = (unsigned short *) NULL;
      imageBytes = (unsigned char *) NULL;
      pixelMap = (PixelMap) NULL;

      imageDataType = NO_IMAGE_DATA;
    }
};

#endif /* _IMAGE_H */
