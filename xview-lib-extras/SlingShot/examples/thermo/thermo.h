/*
 *	@(#) thermo.h 1.1 93/04/06 
 */

 
#include <xview/font.h>
#include <sspkg/drawobj.h>
#include <sspkg/box.h>

typedef struct {
	/* colors */
	int             outline;
	int             fill;
	int             fill2;
	int		label;
	/* values */
	int		scaletop;
	int		scalebot;
	int		value;

	Xv_font		font;
	/*
	 * Following fields are initialized in thermo_init, you can deref 'box'
	 * if you want to attach key data or 'xv_destroy' the thermo.
	 */
	Cbox            box;
	Drawarea        background;
	Drawarea        foreground;
	int		prev_scaletop;
	int		prev_scalebot;
	int		prev_value;
} Thermo;

/*
 * call thermo_init to create;
 * call thermo_render_background if you set colors;
 * call thermo_set when scaletop, scalebot, or value change
 */
EXTERN_FUNCTION(void thermo_init, ( Rectobj parent, Thermo* ));
EXTERN_FUNCTION(void thermo_render_background, ( Thermo* ));
EXTERN_FUNCTION(void thermo_set_value, ( Thermo* ));


