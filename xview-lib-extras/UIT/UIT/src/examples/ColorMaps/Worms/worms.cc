/*
*    SCCS ID: %Z%%M% %I% %E% SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

// Given the number of worms to display...

#include <stdio.h>
#include <math.h>
#include <values.h>

#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "ColorMap.h"
#include "Notifier.h"

static double frand ();
static void   done_handler (UIObject *);

const double PI = 3.14159265359;

#if defined(__STDC__) || defined(SVR4)
const int MAX_RAND = (1 << 15) - 1;
#else
const int MAX_RAND = MAXINT;
#endif

const int INIT_WIDTH = 500;
const int INIT_HEIGHT = 300;
const int INIT_RADIUS = 100;

const int MAX_RADIUS = 500;

main (int argc, char **argv)
{
  BaseWindow bw;
  bw.initUI (argc, argv);
  bw.setWidth (INIT_WIDTH);
  bw.setHeight (INIT_HEIGHT);
  bw.setLabel ("Worms");
  bw.setDoneHandler (done_handler);

  int num_worms = (argc > 1) ? atoi (argv [1]) : 1;

  ComponentDisplay cd;
  cd.setX (0);
  cd.setY (0);
  cd.setDynamic (TRUE);
  cd.setExtendToEdge (HEIGHT);
  cd.setExtendToEdge (WIDTH);
  cd.setPaintHeight (bw.getRootWindowHeight ());
  cd.setPaintWidth (bw.getRootWindowWidth ());
  cd.setAutoExpand (TRUE);
  bw.addDisplay (cd);

  ColorMap cm (DYNAMIC);

  const int NUM_COLORS = 8;
  Color colors [NUM_COLORS];
  colors [0].red =   0; colors [0].green =   0; colors [0].blue = 255;
  colors [1].red =   0; colors [1].green =  20; colors [1].blue = 255;
  colors [2].red =   0; colors [2].green =  40; colors [2].blue = 255;
  colors [3].red =   0; colors [3].green =  60; colors [3].blue = 255;
  colors [4].red =   0; colors [4].green =  80; colors [4].blue = 255;
  colors [5].red =   0; colors [5].green = 100; colors [5].blue = 255;
  colors [6].red =   0; colors [6].green = 120; colors [6].blue = 255;
  colors [7].red =   0; colors [7].green = 120; colors [7].blue = 255;

  char name [20];

  for (int i=0; i < NUM_COLORS; i++) {
    sprintf (name, "color%d", i);
    cm.addColor (name, colors [i]);
  }

  cd.setPaintColorMap (cm);
  cd.setBackgroundColor ("Black");

  Notifier notifier;
  notifier.dispatchEvent ();
  bw.show (TRUE);

  Display   *display = cd.getXDisplay ();
  Drawable   drawable = cd.getXWindow ();
  XGCValues  gcval;

  gcval.function = GXcopy;
  GC gc = XCreateGC (display, drawable, GCFunction, &gcval);

  long seed = clock ();

  double *x = new double [num_worms];
  double *y = new double [num_worms];
  double *r = new double [num_worms];
  double *m = new double [num_worms];
  double *a = new double [num_worms];

  int init_width = bw.getWidth ();
  init_width = (INIT_WIDTH > init_width) ? INIT_WIDTH : init_width;

  int init_height = bw.getHeight ();
  init_height = (INIT_HEIGHT > init_height) ? INIT_HEIGHT : init_height;

  for (i=0; i < num_worms; i++) {
    x [i] = frand () * init_width;
    y [i] = frand () * init_height;
    r [i] = INIT_RADIUS;
    a [i] = frand () * 2.0 * PI;
  }

  int reps = 20 / num_worms + 1;
  int color = 0;

  int constant = ((num_worms % 8 == 0) ? 6 : 7);

  for (;;) {
    for (int i=0; i < reps; i++) {
      for (int j=0; j < num_worms; j++) {
	if (++color > constant) color = 0;

	sprintf (name, "color%d", color);
	XSetForeground (display, gc, cm.getXColorIndex (name));
	XFillArc (display, drawable, gc, (int) (x [j] - r [j]),
		                         (int) (y [j] - r [j]),
		                         (int) (2 * r [j]), (int) (2 * r [j]),
		                         0, 360 * 64);
	XFlush (display);

	m [j] = frand () * r [j] * 0.4 - 0.2 * r [j];
	a [j] = a [j] + frand () * PI / 64.0 - PI / 128.0;
    
	x [j] = m [j] * cos (a [j]) + x [j];
        y [j] = m [j] * sin (a [j]) + y [j];
    
	r [j] = r [j] + 0.04 * r [j] * frand () - 0.02 * r [j];
	if (r [j] > 500) r[j] = 500;

        notifier.dispatchEvent ();
      }
    }

    Color temp_color = cm.getColorRGB ("color0");

    for (i=0; i < NUM_COLORS-1; i++) {
      sprintf (name, "color%d", i+1);
      Color next_color = cm.getColorRGB (name);

      sprintf (name, "color%d", i);
      cm.changeColor (name, next_color);
    }

    sprintf (name, "color%d", i);
    cm.changeColor (name, temp_color);
    
    cd.setPaintColorMap (cm);
    cd.setBackgroundColor ("Black");
  }
}

static double frand ()
{
  static unsigned seed;

  if (!seed)
    (void) srand (seed = (unsigned) time ((time_t *) NULL));
  return ((double) rand () / (double) MAX_RAND);
}

static void done_handler (UIObject *object)
{
  exit (0);
}
