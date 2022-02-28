/*H+ ColorMap.cc
*
*    SCCS ID: @(#)ColorMap.cc 1.2 93/02/08 SMI
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines member functions of the ColorMap class.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       05/10/91   Preparation for code review. 
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)ColorMap.cc 1.2 93/02/08 SMI"; 
#endif

// Includes
#include "ColorMap.h"

// Global Variables
static char *default_xview_cmap_name = "UIC Color Map";
static char *xview_control_color_names [CMS_CONTROL_COLORS] =
            { "BG1", "BG2", "BG3", "Highlight" };

/*F+ ColorMap::ColorMap ()
*
*    PURPOSE:
*
*    ColorMap constructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ColorMap::ColorMap ()
{
  setColorMapDefaults ();
}

/*F+ ColorMap::ColorMap (ColorMapType cmap_type = STATIC)
*
*    PURPOSE:
*
*    ColorMap constructor.
*
*    PARAMETERS:
*
*    cmap_type - The color map type, STATIC or DYNAMIC.  The default is STATIC.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ColorMap::ColorMap (ColorMapType cmap_type = STATIC)
{
  setColorMapDefaults ();
  cmsType = cmap_type;
}

/*F+ ColorMap::ColorMap (GenericList cmap_colors, ColorMapType cmap_type)
*
*    PURPOSE:
*
*    ColorMap constructor.
*
*    PARAMETERS:
*
*    cmap_colors - A list (GenericList) of colors to put in the map.
*    cmap_type   - The color map type, STATIC or DYNAMIC.  The default is
*                  STATIC.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ColorMap::ColorMap (GenericList cmap_colors, ColorMapType cmap_type = STATIC)
{
  setColorMapDefaults ();

  cmsType = cmap_type;
  colors = cmap_colors; 
}

/*F+ void ColorMap::setColorMapDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members and creates the minimum UIT color map
*    containing the XView CMS control colors, and Black and White.
*
*    PARAMETERS:
*
*    cmap_name - The XView color map name.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ColorMap::setColorMapDefaults ()
{
  for (int i = 0; i < CMS_CONTROL_COLORS; i++) {
    colors.addItem ((Generic *) NULL); 
    colorNames.addItem ((Generic *) xview_control_color_names [i]);
  }
  
  addColor ("White", 255, 255, 255);
  addColor ("Black", 0, 0, 0);

  cmsType = STATIC;
  colorMapName = new char [strlen (default_xview_cmap_name) + 1];
  strcpy (colorMapName, default_xview_cmap_name);

  xColorIndices = (long *) NULL;
}

/*F+ ColorMap::~ColorMap ()
*
*    PURPOSE:
*
*    ColorMap destructor.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
ColorMap::~ColorMap ()
{
  int num_entries = colors.getSize ();

  // Delete every item in the color list
  for (int i = num_entries-1; i >= 0; i--) {
    delete ((char *) colorNames.removeItem (i)); 
    delete ((Color *) colors.removeItem (i));
  }

  if (colorMapName) delete colorMapName;
  if (xColorIndices) delete xColorIndices;
}

/*F+ void ColorMap::setColorMapName (char *cmap_name)
*
*    PURPOSE:
*
*    Sets the XView color map name.
*
*    PARAMETERS:
*
*    cmap_name - The XView color map name.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ColorMap::setColorMapName (char *cmap_name)
{
  if (colorMapName)
    delete colorMapName;
  
  colorMapName = new char [strlen (cmap_name) + 1];
  strcpy (colorMapName, cmap_name);
}
       
/*F+ char *ColorMap::getColorMapName ()
*
*    PURPOSE:
*
*    Gets the XView color map name.
*
*    PARAMETERS:
*
*    cmap_name - The XView color map name.
*
*    RETURN VALUES:
*
*    Returns the XView color map name.  If no color map name was
*    specified, "UIC Color Map" is the default returned.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
char *ColorMap::getColorMapName ()
{
  char *cmap_name = strcpy (new char [strlen (colorMapName) + 1],
			    colorMapName);
  return (cmap_name);
}

/*F+ void ColorMap::addColor (char *name, int red, int green, int blue)
*
*    PURPOSE:
*
*    Adds a color to the color map.
*
*    PARAMETERS:
*
*    name  - The name of the color.
*    red   - The red component of the color.
*    blue  - The blue component of the color.
*    green - The green component of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ColorMap::addColor (char *name, int red, int green, int blue)
{
  if (findColor (name) != UIC_NOT_SET)
    return;

  Color *temp_color = new Color;
  temp_color->red = red;
  temp_color->green = green;
  temp_color->blue = blue;

  char *temp_name = strcpy (new char [strlen (name) + 1], name);
 
  colors.addItem ((Generic *) temp_color); 
  colorNames.addItem ((Generic *) temp_name);
}

/*F+ void ColorMap::addColor (char *name, Color color)
*
*    PURPOSE:
*
*    Adds a color to the color map.
*
*    PARAMETERS:
*
*    name  - The name of the color.
*    color - A Color structure containing the RGB of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ColorMap::addColor (char *name, Color color)
{
  if (findColor (name) != UIC_NOT_SET)
    return;

  Color *temp_color = new Color;
  temp_color->red = color.red;
  temp_color->green = color.green;
  temp_color->blue = color.blue;

  char *temp_name = strcpy (new char [strlen (name) + 1], name);
  
  colors.addItem ((Generic *) temp_color);
  colorNames.addItem ((Generic *) temp_name);
}

/*F+ void ColorMap::addColor (const char *name)
*
*    PURPOSE:
*
*    Adds a color to the color map.
*
*    PARAMETERS:
*
*    name  - The X color database color name.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ColorMap::addColor (const char *name)
{
  if (findColor (name) != UIC_NOT_SET)
    return;

  initUI (noInitUIArgs); // Make sure the graphics server is initialized

  XColor db_value; // Database value
  XColor hw_value; // Hardware value

  // Get the default color map for the root window
  Colormap  cmap = DefaultColormap (rootDisplay, 0);

  // Look the color up in the X color database
  XLookupColor (rootDisplay, cmap, name, &db_value, &hw_value);

  /* Convert the X color database hardware color value to 0-255 range
     and store in the UIC Color structure */
  Color *temp_color = new Color ;
  temp_color->red  = hw_value.red >> 8;
  temp_color->green = hw_value.green >> 8;
  temp_color->blue = hw_value.blue >> 8;
   
  char *temp_name = strcpy (new char [strlen (name) + 1], name);
 
  colors.addItem ((Generic *) temp_color);
  colorNames.addItem ((Generic *) temp_name);
}

/*F+ int ColorMap::findColor (Color color)
*
*    PURPOSE:
*
*    Return the index of the specified color.
*
*    PARAMETERS:
*
*    color - A Color structure containing the RGB of the color to find.
*
*    RETURN VALUES:
*
*    Returns the index of the specified color.  UIC_NOT_SET is returned
*    if the color does not exist.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ColorMap::findColor (Color color)
{
  int num_colors = colors.getSize ();

  for (int i=0; i < num_colors; i++) {
    Color *temp_color = (Color *) colors.getItem (i);

    if (temp_color)
      if ((temp_color->red == color.red) &&
          (temp_color->green == color.green) &&
          (temp_color->blue == color.blue))
        return (i);
  }
 
  return (UIC_NOT_SET);
}
          
/*F+ int ColorMap::findColor (const char *name)
*
*    PURPOSE:
*
*    Return the index of the specified color.
*
*    PARAMETERS:
*
*    name - The name of the color to find.
*
*    RETURN VALUES:
*
*    Returns the index of the specified color.  UIC_NOT_SET is returned
*    if the color does not exist.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ColorMap::findColor (const char *name)
{
  if (name) {
    int num_colors = colorNames.getSize ();

    for (int i=0; i < num_colors; i++) {
      char *temp_name = (char *) colorNames.getItem (i);
  
      if (temp_name)
        if (!strcmp (temp_name, name))
          return (i);
    }

    if (!strcmp ("Background", name))
      return (colors.getSize ());
  
    if (!strcmp ("Foreground", name))
      return (colors.getSize ()+1);
  }

  return (UIC_NOT_SET);
}

/*F+ void ColorMap::changeColor (char *name, int red, int green, int blue)
*
*    PURPOSE:
*
*    Change the RGB value of the named color.
*
*    PARAMETERS:
*
*    name  - The name of the color to change.
*    red   - The new red component.
*    green - The new green component.
*    blue  - The new blue component.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ColorMap::changeColor (char *name, int red, int green, int blue)
{
  Color *temp_color = new Color;
  temp_color->red = red;
  temp_color->green = green;
  temp_color->blue = blue;

  int size = colorNames.getSize ();
  for (int i=0; i < size; i++) {
    if (!strcmp ((char *) colorNames [i], name)) {
      if (colors [i]) delete ((Color *) colors [i]);
      colors [i] = (Generic *) temp_color;
      break;
    }
  }
}

/*F+ void ColorMap::changeColor (char *name, Color color)
*
*    PURPOSE:
*
*    Change the RGB value of the named color.
*
*    PARAMETERS:
*
*    name  - The name of the color to change.
*    color - A Color structure containing the new RGB of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ColorMap::changeColor (char *name, Color color)
{
  Color *temp_color = new Color;
  temp_color->red = color.red;
  temp_color->green = color.green;
  temp_color->blue = color.blue;

  int size = colorNames.getSize ();
  for (int i=0; i < size; i++) {
    if (!strcmp ((char *) colorNames [i], name)) {
      if (colors [i]) delete ((Color *) colors [i]);
      colors [i] = (Generic *) temp_color;
      break;
    }
  }
}

/*F+ long ColorMap::getXColorIndex (char *color)
*
*    PURPOSE:
*
*    Get the index of the specified color in the X color map.
*
*    PARAMETERS:
*
*    color - The name of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
long ColorMap::getXColorIndex (char *color)
{
  return (getXColorIndex (findColor (color)));
}

/*F+ long ColorMap::getXColorIndex (Color color)
*
*    PURPOSE:
*
*    Get the index of the specified color in the X color map.
*
*    PARAMETERS:
*
*    color - A UIC color structure containing the RGB components of the color.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
long ColorMap::getXColorIndex (Color color)
{
  return (getXColorIndex (findColor (color)));
}

/*F+ long ColorMap::getXColorIndex (int color)
*
*    PURPOSE:
*
*    Get the index of the specified color in the X color map.
*
*    PARAMETERS:
*
*    color - The index of the color into the UIC color map.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
long ColorMap::getXColorIndex (int color)
{
  long x_color_index = UIC_NOT_SET;

  if (owObject)
    x_color_index = xColorIndices [color];

  return (x_color_index);
}

/*F+ Color &ColorMap::getRGBColor (char *name)
*
*    PURPOSE:
*
*    Return the RGB value of the named color.
*
*    PARAMETERS:
*
*    name - The name of the color.
*
*    RETURN VALUES:
*
*    Returns a Color structure with the RGB value of the specified color.
*    Returns a Color structure with red=0, green=0 and blue=0 if the color
*    does not exist.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
Color &ColorMap::getColorRGB (char *name)
{
  int index = 0;
  char *temp_name = (char *) colorNames.traverse (TRUE);

  while (temp_name) {
    if (!strcmp (temp_name, name))
      return *((Color *) colors [index]);

    index++;
    temp_name = (char *) colorNames.traverse (FALSE);
  }

  static Color color;
  color.red = 0;
  color.green = 0;
  color.blue = 0;

  return color;
}

/*F+ void ColorMap::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView color map.
*
*    PARAMETERS:
*
*    parent - The ColorMap parent.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void ColorMap::createObject (UIObject *parent)
{
  Xv_cmsdata *old_cms_data;
  Color      *temp_color;
  UICGeneric  xview_object;
    
  /* If a parent is specified, get an XView frame from it.  Otherwise,
     use the main base window frame. */
  xview_object = (parent ? parent->getXViewObject () :
			   baseWindow->getXViewObject ());

  // Get the XView and the CMS data from the XView frame
  Frame xview_frame = (Frame) xv_get (xview_object, XV_KEY_DATA, UIC_FRAME_KEY);
  old_cms_data = (Xv_cmsdata *) xv_get (xview_frame, WIN_CMS_DATA);

  // Save the size of the color list
  int current_size = colors.getSize ();
  
  // If the XView object already exists...
  if (owObject) {
    /* Remove the old foreground and background colors from the color list:
       They will be replaced by the latest defaults. */

    // Remove the foreground color
    int index = findColor ("Foreground");
    if (index != UIC_NOT_SET) {
      delete ((Color *) colors.removeItem (index));
      delete ((char *) colorNames.removeItem (index));
    }

    // Remove the background color
    index = findColor ("Background");
    if (index != UIC_NOT_SET) {
      delete ((Color *) colors.removeItem (index));
      delete ((char *) colorNames.removeItem (index));
    }

    // Recalculate the current size of the color map
    current_size = colors.getSize ();
  }
  
  /* Allocate space for XView color data (allow for "Foreground" and
     "Background" to be added */
  Color *color_data = new Color [current_size + 2];
  
  // Copy the UIC color list to the XView color data array
  for (int i = 0 ; i < current_size - CMS_CONTROL_COLORS; i++) {
    temp_color = (Color *) colors [i + CMS_CONTROL_COLORS];
    color_data [i].red = temp_color->red;
    color_data [i].green = temp_color->green;
    color_data [i].blue = temp_color->blue;
  }

  int xview_bg_index = (int) xv_get (xview_frame, WIN_BACKGROUND_COLOR);
  
  /* Store the current Background color as the second to last color in the
     XView color data array */
  color_data [current_size - CMS_CONTROL_COLORS].red =
    old_cms_data->red [xview_bg_index];
  color_data [current_size - CMS_CONTROL_COLORS].green =
    old_cms_data->green [xview_bg_index];
  color_data [current_size - CMS_CONTROL_COLORS].blue =
    old_cms_data->blue [xview_bg_index];
  
  int xview_fg_index = (int) xv_get (xview_frame, WIN_FOREGROUND_COLOR);
  
  /* Store the current Foreground color as the last color in the XView color
     data array */
  color_data [current_size - CMS_CONTROL_COLORS + 1].red = 
    old_cms_data->red [xview_fg_index];
  color_data [current_size - CMS_CONTROL_COLORS + 1].green = 
    old_cms_data->green [xview_fg_index];
  color_data [current_size - CMS_CONTROL_COLORS + 1].blue =
    old_cms_data->blue [xview_fg_index];
  
  // If the XView object exists...
  if (owObject)
    // Load new colors into the color map
    xv_set (owObject,
	    CMS_COLORS,	color_data,
	    NULL);
  // Else, the XView object does not exist...
  else {
    Xv_Screen screen = (Xv_Screen) xv_get (xview_frame, XV_SCREEN);

    // If the color map is to be static...
    if (cmsType == STATIC)
      owObject = xv_create (screen, CMS,
			    CMS_NAME,	     colorMapName,
			    CMS_TYPE,	     XV_STATIC_CMS,
			    CMS_CONTROL_CMS, TRUE,
			    CMS_SIZE,	     current_size + 2,
			    CMS_COLORS,	     color_data,
			    NULL);
    // Else, create a dynamic color map...
    else
      owObject = xv_create (screen, CMS,
			    CMS_NAME,	     colorMapName,
			    CMS_TYPE,	     XV_DYNAMIC_CMS,
			    CMS_CONTROL_CMS, TRUE,
			    CMS_SIZE,	     current_size + 2,
			    CMS_COLORS,	     color_data,
			    NULL);
  }
  
  // Get the color data so we can retrieve the CMS_CONTROL_COLORS colors.
  xv_get (owObject, CMS_COLORS, color_data);
  
  // Store the CMS Control Colors in the UIC color list
  for (i=0; i < CMS_CONTROL_COLORS; i++) {
    Color *temp_color = new Color;
    temp_color->red = color_data [i].red;
    temp_color->green = color_data [i].green;
    temp_color->blue = color_data [i].blue;

    if (colors [i]) delete ((Color *) colors [i]);
    colors [i] = (Generic *) temp_color;
  }
  
  // Add the Background color to the UIC color list
  addColor ("Background", color_data [current_size]);

  // Add the Foreground color to the UIC color list
  addColor ("Foreground", color_data [current_size+1]);
  
  // Delete the XView color data array
  delete color_data;

  // If old X color map indices exist...
  if (xColorIndices) delete xColorIndices;

  // Get the X color map indices
  long *temp_array = (long *) xv_get (owObject, CMS_INDEX_TABLE);

  // Save the indices
  int num_colors = colors.getSize ();
  xColorIndices = new long [num_colors];
  for (i=0; i < num_colors; i++)
    xColorIndices [i] = temp_array [i];

  UIObject::setUIObjectAttributes (parent);
}

/*F+ int ColorMap::getDefaultBackgroundIndex (UIObject *parent)
*
*    PURPOSE:
*
*    Returns the index of the default background color after the main base
*    window has been created.
*
*    PARAMETERS:
*
*    parent - The parent object of the XView color map.
*
*    RETURN VALUES:
*
*    Returns the index of the default background color.  Returns UIC_NOT_SET
*    if the notifier has not yet been started.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ColorMap::getDefaultBackgroundIndex (UIObject *parent)
{
  int         bg_index = colors.getSize () - 2;
  int         index = UIC_NOT_SET;
  UICGeneric  xview_object;
  Xv_cmsdata *xview_cms_data;

  if (xViewObjectsCreated) {
    xview_object = (parent ? parent->getXViewObject () :
			     baseWindow->getXViewObject ());

    // Get the default XView color map
    xview_cms_data = (Xv_cmsdata *) xv_get (xview_object, WIN_CMS_DATA);

    /* XView stores its default background at index 0 or index size-2
       depending on the CMS Control Colors */
    Color *bg1_color = (Color *) colors [0];
    Color *bg_color = (Color *) colors [bg_index];

    // Get the XView CMS background color index
    int xview_bg_index = (int) xv_get (xview_object, WIN_BACKGROUND_COLOR);

    // If the XView default is equal to the UIC color at index 0...
    if ((xview_cms_data->red [xview_bg_index] == bg1_color->red) &&
        (xview_cms_data->green [xview_bg_index] == bg1_color->green) &&
        (xview_cms_data->blue [xview_bg_index] == bg1_color->blue))
      // Return 0 as the default BG index
      index = 0;
    // Else if the XView default is equal to the UIC color at index size-2
    else if ((xview_cms_data->red [xview_bg_index] == bg_color->red) &&
	     (xview_cms_data->green [xview_bg_index] == bg_color->green) &&
	     (xview_cms_data->blue [xview_bg_index] == bg_color->blue))
      // Return size-2 as the default BG index
      index = bg_index;
    // Else, none of the defaults match...
    else
      // Return the index of "White" as the default BG index
      index = findColor ("White");
  }

  return index;
}

/*F+ int ColorMap::getDefaultForegroundIndex (UIObject *parent)
*
*    PURPOSE:
*
*    Returns the index of the default foreground color after the notifier	
*    has been started.
*
*    PARAMETERS:
*
*    parent - The parent object of the XView color map.
*
*    RETURN VALUES:
*
*    Returns the index of the default foreground color.  Returns UIC_NOT_SET
*    if the notifier has not yet been started.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int ColorMap::getDefaultForegroundIndex (UIObject *parent)
{
  int         fg_index = colors.getSize () - 1;
  int         index = UIC_NOT_SET;
  UICGeneric  xview_object;
  Xv_cmsdata *xview_cms_data;

  if (xViewObjectsCreated) {
    xview_object = (parent ? parent->getXViewObject () :
			     baseWindow->getXViewObject ());

    // Get the default XView color map
    xview_cms_data = (Xv_cmsdata *) xv_get (xview_object, WIN_CMS_DATA);

    // XView and UIC both store their default foregrounds at index size-1
    Color *fg_color = (Color *) colors [fg_index];

    // Get the XView CMS foreground color index
    int xview_fg_index = (int) xv_get (xview_object, WIN_FOREGROUND_COLOR);

    // If the XView default is equal to the UIC default...
    if ((xview_cms_data->red [xview_fg_index] == fg_color->red) &&
        (xview_cms_data->green [xview_fg_index] == fg_color->green) &&
        (xview_cms_data->blue [xview_fg_index] == fg_color->blue))
      // Return size-1 as the default FG index
      index = fg_index;
    // Else, the defaults don't match...
    else
      // Return the index of "Black" as the default BG index
      index = findColor ("Black");
  }

  return index;
}

#ifdef TEST
#endif /* TEST */
