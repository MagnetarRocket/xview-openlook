/*H+ Image.cc
*
*    SCCS ID: @(#)Image.cc 1.2 93/02/08 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the Image class.
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
#ifndef lint
static char sccsid [] = "@(#)Image.cc 1.2 93/02/08 SMI"; 
#endif

// Includes
#include "ColorMap.h"
#include "Image.h"

/*F+ void Image::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView image.
*
*    PARAMETERS:
*
*    parent - The parent of the Image.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void Image::createObject (UIObject *parent)
{
  if (owObject) return;

  UICGeneric color_map_pointer = (UICGeneric) NULL;
  UICGeneric server_image_cms = (UICGeneric) NULL;
  UICGeneric xview_parent = (UICGeneric) NULL;

  if (parent) {
    Frame xview_frame = (Frame) xv_get (parent->getXViewObject (),
				        XV_KEY_DATA,
					UIC_FRAME_KEY);
    xview_parent = (UICGeneric) xv_get (xview_frame, XV_SCREEN);
  }

  if (imageColorMap) {
    ((UIObject *) imageColorMap)->createObject (parent);
    server_image_cms = (UICGeneric) SERVER_IMAGE_CMS;
    color_map_pointer = imageColorMap->getXViewObject ();
  }

  switch (imageDataType) {
    case ICON_FILE:
      owObject = icon_load_svrim (imageFile, loadIconErrorMsg);
      break;
    case BIT_MAP_FILE:
      owObject = xv_create (xview_parent, SERVER_IMAGE,
			    SERVER_IMAGE_BITMAP_FILE, imageFile,
			    NULL);
      break;
    case IMAGE_SHORTS:
      owObject = xv_create (xview_parent, SERVER_IMAGE,
			    XV_HEIGHT,          height,
			    XV_WIDTH,           width,
			    SERVER_IMAGE_BITS,  imageShorts,
			    SERVER_IMAGE_DEPTH, depth,
			    server_image_cms,   color_map_pointer,
			    NULL);
      break;
    case IMAGE_BYTES:
      owObject = xv_create (xview_parent, SERVER_IMAGE,
			    XV_HEIGHT,           height,
			    XV_WIDTH,            width,
			    SERVER_IMAGE_X_BITS, imageBytes,
			    SERVER_IMAGE_DEPTH,  depth,
			    server_image_cms,    color_map_pointer,
			    NULL);
	  break;
    case PIXEL_MAP:
      owObject = xv_create (xview_parent, SERVER_IMAGE,
			    XV_HEIGHT,           height,
			    XV_WIDTH,            width,
			    SERVER_IMAGE_PIXMAP, pixelMap,
			    SERVER_IMAGE_DEPTH,  depth,
			    server_image_cms,    color_map_pointer,
			    NULL);
      break;
    case NO_IMAGE_DATA:
    default:
      owObject = (UICGeneric) NULL;
      break;
  }

  UIObject::setUIObjectAttributes (parent);
}

#ifdef TEST
#endif /* TEST */
