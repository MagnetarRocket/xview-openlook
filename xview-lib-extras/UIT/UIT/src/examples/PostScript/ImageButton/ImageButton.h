/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#ifndef _IMAGE_BUTTON_H
#define _IMAGE_BUTTON_H

#include "ColorMap.h"
#include "ComponentDisplay.h"
#include "UIComponent.h"

enum Mode {NOT_PRESSED, PRESSED};

class ImageButton : public UIComponent {
public:
  ImageButton (Image &image)
    {
      buttonImage = &image;
      isDrawableOnCanvas = TRUE;
      noDragDrop = TRUE;

      // At startup, the left mouse button is not pressed
      mode = NOT_PRESSED;
      leftButton = FALSE;
    }

  ~ImageButton () { cd->removePSProc (imageProcName);
	            cd->removePSProc (boxProcName);
		    cd->removePSProc (ubProcName);
		    cd->removePSProc (lbProcName); }

  void show (Boolean flag)
    {
      showFlag = flag;

      if (objectCreated) {
        if (showFlag) {
          repaintComponent (cd);
        }
        else {
          Display *display = cd->getXDisplay ();
          Window   window = cd->getXWindow ();

          // Clear the old location of the object
          XClearArea (display, window,
                      xLoc,    yLoc,
                      width,   height,
                      False);
        }
      }
    }

protected:
  Image		   *buttonImage;
  char              imageProcName [22];
  char		    boxProcName [20];
  char		    ubProcName [20];
  char		    lbProcName [20];
  char		    drawImage [80];
  char		    drawBox [80];
  char		    drawUB [80];
  char		    drawLB [80];

  ComponentDisplay *cd;
  HotRegion         hotRegion;
  ColorMap          componentColorMap;
  Color             BG1;
  Color             BG2;
  Color             BG3;
  Color             HL;
  Color		    FG;

  char              temp_string [128];
  Boolean	    leftButton;
  Mode		    mode;

  void repaintComponent (ComponentDisplay *parent)
    {
      if (showFlag) { 
        // Make sure we paint on the correct ComponentDisplay
        parent->setPSFocus ();

        if (mode == NOT_PRESSED)
          cd->setPSColor (BG1);
        else
          cd->setPSColor (BG2);
	POSTSCRIPT_STRING (drawBox);

	if (mode == NOT_PRESSED)
	  cd->setPSColor (HL);
	else
	  cd->setPSColor (BG3);
	POSTSCRIPT_STRING (drawUB);

	if (mode == NOT_PRESSED)
	  cd->setPSColor (BG3);
	else
	  cd->setPSColor (HL);
	POSTSCRIPT_STRING (drawLB);
	
	cd->setPSColor (FG);
	POSTSCRIPT_STRING (drawImage);
	
	ps_flush_PostScript ();
      }
    }

  void showFeedback (ComponentDisplay *parent)
    {
      if (showFlag) {
	// Make sure we paint on the correct ComponentDisplay
	  parent->setPSFocus ();
 
	if (mode == NOT_PRESSED)
	  cd->setPSColor (BG1);
	else
	  cd->setPSColor (BG2);
	POSTSCRIPT_STRING (drawBox);
	
	if (mode == NOT_PRESSED)
	  cd->setPSColor (HL);
	else
	  cd->setPSColor (BG3);
	POSTSCRIPT_STRING (drawUB);
	
	if (mode == NOT_PRESSED)
	  cd->setPSColor (BG3);
	else
	  cd->setPSColor (HL);
	POSTSCRIPT_STRING (drawLB);
	
	ps_flush_PostScript ();
      }
    }
 
  void componentHotRegionEventProc (ComponentDisplay *parent)
    {
      if (showFlag) {
	InputEvent *event = cd->getEvent ();

	if (event->isButtonDown () &&
	    ((event->getAction () == ACTION_SELECT) || leftButton)) {
	  if (mode == NOT_PRESSED) {
	    mode = PRESSED;
	    leftButton = TRUE;
	    showFeedback (parent);
	  }
	}
	else if (event->isButtonUp () && (mode == PRESSED)) {
	  mode = NOT_PRESSED;
	  leftButton = FALSE;
	  showFeedback (parent);
	  
	  if (notifyHandler)
	    (*notifyHandler) (this);
	}
      }
    }

  void componentBackgroundEventProc (ComponentDisplay *parent)
    {
      if (!showFlag) {
	InputEvent *event = cd->getEvent ();
	
	if (mode == PRESSED) {
	  mode = NOT_PRESSED;
	  showFeedback (parent);
	}
	
	if (event->isButtonDown () && (event->getAction () == ACTION_SELECT))
	  leftButton = TRUE;
	else if (event->getID () != LOC_DRAG)
	  leftButton = FALSE;
      }
    }

  void createObject (UIObject *parent)
    {
      cd = (ComponentDisplay *) parent;

      // Create the XView object of the button's image
      ((UIObject *) buttonImage)->createObject (parent);

      // Use the size of the button's image as the size of the button
      width = buttonImage->getWidth () + 1;
      height = buttonImage->getWidth () + 1;

      // Specify the button's location and size with a HotRegion object
      hotRegion.setDefaultRectangle (xLoc, yLoc, width, height);

      // Create an XView ColorMap object to obtain color information
      ((UIObject *) &componentColorMap)->createObject (parent);

      /* Get the RGB values of the colors needed to provide a
	 3D look and feel for the button */
      BG1 = componentColorMap.getColorRGB ("BG1");
      BG2 = componentColorMap.getColorRGB ("BG2");
      BG3 = componentColorMap.getColorRGB ("BG3");
      HL  = componentColorMap.getColorRGB ("Highlight");
      FG  = componentColorMap.getColorRGB ("Foreground");

      // Create the PostScript procedure for drawing the button image
      sprintf (imageProcName, "SIB_image_%d", uiObjectID);
      cd->storePSDrawImageProc (imageProcName, *buttonImage, TRUE);
      sprintf (drawImage, " %d %d %s ", xLoc, yLoc, imageProcName);

      /* Create the PostScript procedure for drawing the background
	 of the button */
      sprintf (boxProcName, "SIB_box_%d", uiObjectID);
      cd->storePSDrawImageProc (boxProcName, *buttonImage, TRUE, TRUE);
      sprintf (drawBox, " %d %d %s ", xLoc, yLoc, boxProcName);

      /* Create a PostScript procedure for drawing the upper borders of
         of the square around the button */
      sprintf (ubProcName, "SIB_ub_%d", uiObjectID);
      POSTSCRIPT_STRING ("/");
      POSTSCRIPT_STRING (ubProcName);
      POSTSCRIPT_STRING (" { ");
      POSTSCRIPT_STRING ("/h exch def ");
      POSTSCRIPT_STRING ("/w exch def ");
      POSTSCRIPT_STRING ("/y exch def ");
      POSTSCRIPT_STRING ("/x exch def ");
      POSTSCRIPT_STRING ("newpath ");
      POSTSCRIPT_STRING ("x y h add moveto ");
      POSTSCRIPT_STRING ("0 h neg rlineto ");
      POSTSCRIPT_STRING ("w 0 rlineto ");
      POSTSCRIPT_STRING ("stroke ");
      POSTSCRIPT_STRING (" } def ");
      ps_flush_PostScript ();

      sprintf (drawUB, " %d %d %d %d %s ",
	       xLoc, yLoc, width-1, height-1, ubProcName);

      /* Create a PostScript procedure for drawing the lower borders of
         of the square around the button */
      sprintf (lbProcName, "SIB_lb_%d", uiObjectID);
      POSTSCRIPT_STRING ("/");
      POSTSCRIPT_STRING (lbProcName);
      POSTSCRIPT_STRING (" { ");
      POSTSCRIPT_STRING ("/h exch def ");
      POSTSCRIPT_STRING ("/w exch def ");
      POSTSCRIPT_STRING ("/y exch def ");
      POSTSCRIPT_STRING ("/x exch def ");
      POSTSCRIPT_STRING ("newpath ");
      POSTSCRIPT_STRING ("x w add y moveto ");
      POSTSCRIPT_STRING ("0 h rlineto ");
      POSTSCRIPT_STRING ("w neg 0 rlineto ");
      POSTSCRIPT_STRING ("stroke ");
      POSTSCRIPT_STRING (" } def ");
      ps_flush_PostScript ();

      sprintf (drawLB, " %d %d %d %d %s ",
	       xLoc, yLoc, width-1, height-1, lbProcName);

      // Register the button's HotRegion with its parent ComponentDisplay
      cd->addComponentHotRegion (*this, hotRegion, TRUE);

      // Register the button to be repainted by its parent ComponentDisplay
      cd->addRepaintComponent (*this);

      setUIObjectAttributes (parent);
    }
};

#endif
