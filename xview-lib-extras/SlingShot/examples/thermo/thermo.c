/*
 *	@(#) thermo.c 1.1 93/04/06 
 */

#include <xview/cms.h>
#include <sspkg/rectobj.h> 
#include <sspkg/canshell.h> 
#include <sspkg/drawobj.h> 
#include "thermo.h"
 

#define TH_LEFT_X -1.0
#define TH_RIGHT_X 1.0
#define TH_UPPER_Y 0.0
#define TH_LOWER_Y 20.0
#define TH_WIDTH (TH_RIGHT_X - TH_LEFT_X)
#define TH_HEIGHT (TH_LOWER_Y - TH_UPPER_Y)

#define INSIDE_LEFT		-0.5
#define INSIDE_SIZE		(TH_WIDTH/2)
#define WELL_SCALE		2.0

#define SCALE_LENGTH		(TH_HEIGHT - WELL_SCALE - INSIDE_SIZE/2)
#define WICK_LEFT		-.25
#define WICK_WIDTH		.5


void
thermo_init(parent, the)
	Rectobj parent;
	Thermo *the;
{
	double left, right, upper, lower;

	left = TH_LEFT_X - 0.1;
	right = TH_RIGHT_X + 1.0;
	upper = TH_UPPER_Y - 0.1;
	lower = TH_LOWER_Y + 0.1;

	the->box = (Cbox) xv_create(parent, CBOX, NULL);

	the->background = (Drawarea) xv_create(the->box, DRAWAREA,
		RECTOBJ_RESIZABLE, TRUE,
		DRAWAREA_LEFT_X, &left,
		DRAWAREA_RIGHT_X, &right,
		DRAWAREA_UPPER_Y, &upper,
		DRAWAREA_LOWER_Y, &lower,
		NULL);

	the->foreground = (Drawarea) xv_create(the->box, DRAWAREA,
		RECTOBJ_RESIZABLE, TRUE,
		DRAWAREA_LEFT_X, &left,
		DRAWAREA_RIGHT_X, &right,
		DRAWAREA_UPPER_Y, &upper,
		DRAWAREA_LOWER_Y, &lower,
		NULL);

	thermo_render_background(the);
}

void
thermo_render_background(the)
	Thermo *the;
{
	VClear( the->background );
	VSetColor( the->background, the->fill);
	DFillArc( the->background,
		INSIDE_LEFT, TH_UPPER_Y,
		INSIDE_SIZE, INSIDE_SIZE,
		0, 360*64);

	VSetColor( the->background, the->outline);
	DDrawArc( the->background,
		INSIDE_LEFT, TH_UPPER_Y,
		INSIDE_SIZE, INSIDE_SIZE,
		0, 360*64);

	VSetColor( the->background, the->fill);
	DFillArc( the->background,
		TH_LEFT_X, TH_HEIGHT - WELL_SCALE,
		WELL_SCALE, WELL_SCALE,
		0, 360*64);

	VSetColor( the->background, the->outline);
	DDrawArc( the->background,
		TH_LEFT_X, TH_HEIGHT - WELL_SCALE,
		WELL_SCALE, WELL_SCALE,
		0, 360*64);

	VSetColor( the->background, the->fill);
	DFillRectangle( the->background,
		INSIDE_LEFT, TH_UPPER_Y + INSIDE_SIZE/2,
		INSIDE_SIZE, TH_HEIGHT - WELL_SCALE);

	VSetColor( the->background, the->outline );
	DDrawLine( the->background, 
		INSIDE_LEFT, TH_UPPER_Y + INSIDE_SIZE/2,
		INSIDE_LEFT, TH_HEIGHT-WELL_SCALE/2-0.880/*dontask*/);
	DDrawLine( the->background, 
		INSIDE_LEFT + INSIDE_SIZE -0.0001, TH_UPPER_Y + INSIDE_SIZE/2,
		INSIDE_LEFT + INSIDE_SIZE - 0.0001, TH_HEIGHT-WELL_SCALE/2-0.880);

	VSetColor( the->background, the->fill2);
	DFillArc( the->background,
		INSIDE_LEFT - .125,
		TH_HEIGHT-WELL_SCALE/2 - INSIDE_SIZE/2 - .125,
		INSIDE_SIZE + .25, INSIDE_SIZE + .25,
		0, 360*64);

	DFillRectangle( the->background,
		WICK_LEFT, SCALE_LENGTH + INSIDE_SIZE/2,
		WICK_WIDTH, WELL_SCALE/2);


}

void
thermo_set_value(the)
	Thermo *the;
{
	Canvas_shell	canvas_shell;
	double          percentage;
	double		y;
	double		height;
	char		string[40];

	if (the->value == the->prev_value &&
	    the->scaletop == the->prev_scaletop &&
	    the->scalebot == the->prev_scalebot)
		return;

	the->prev_value = the->value;
	the->prev_scaletop = the->scaletop;
	the->prev_scalebot = the->scalebot;

	canvas_shell = (Canvas_shell) xv_get(the->box, RECTOBJ_CANVAS);
	if(!canvas_shell)
		return;

	xv_set(canvas_shell, 
		CANVAS_SHELL_DELAY_REPAINT, TRUE,
		NULL);

	percentage = (double) (the->value - the->scalebot) /
		(double) (the->scaletop - the->scalebot);

	height = SCALE_LENGTH * percentage;
	y = SCALE_LENGTH - height + INSIDE_SIZE/2;

	VClear(the->foreground);
	VSetColor(the->foreground, the->fill2);

	DFillRectangle(the->foreground,
		WICK_LEFT, y,
		WICK_WIDTH, height);

	VSetColor( the->foreground, the->outline);
	DDrawLine( the->foreground,
		WICK_LEFT, y,
		WICK_LEFT + WICK_WIDTH + 0.5, y);

	if(the->font)
		VSetFont(the->foreground, the->font);

	sprintf(string, "%d", the->value);
	DDrawString( the->foreground,
		WICK_LEFT + WICK_WIDTH + 0.5, y,
		string, strlen(string));


	xv_set(canvas_shell, 
		CANVAS_SHELL_DELAY_REPAINT, FALSE,
		NULL);
}

