/*
 *	(c) Copyright 1991 Sun Microsystems, Inc.  All rights reserved.
 *	See LEGAL_NOTICE file for terms of the license.
 *
 *	@(#) choose.c 1.6 93/04/06 
 *
 *	This program demonstrates uses of the drawrect and color.
 */

#include <xview/frame.h>
#include <xview/cms.h>
#include <sspkg/canshell.h> 
#include <sspkg/array.h> 
#include <sspkg/drawobj.h> 


Frame create_color_chooser();
void single_click_callback();

main(argc, argv)
	int	argc;
	char	*argv[];
{
	Cms		cms;
	Frame           frame;

	xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);
 
	cms = (Cms) xv_create(XV_NULL, CMS,
			CMS_CONTROL_CMS, TRUE,
			CMS_SIZE, CMS_CONTROL_COLORS + 32,
			CMS_NAMED_COLORS, 
					"MintCream", 
					"moccasin",
					"IndianRed",
					"tomato",
					"HotPink",
					"navy",
					"MistyRose",
					"plum",
					"firebrick",
					"gold1",
					"turquoise",
					"CadetBlue",
					"MediumAquamarine",
					"aquamarine",
					"SeaGreen",
					"MediumSeaGreen",
					"SpringGreen",
					"LawnGreen",
					"MediumSpringGreen",
					"LimeGreen",
					"DarkKhaki",
					"khaki",
					"PaleGoldenrod",
					"LightGoldenrodYellow",
					"yellow",
					"gold",
					"peru",
					"firebrick",
					"salmon",
					"seashell1", 
					"blue", 
					"black", 
					0,
			NULL);

	frame = create_color_chooser(cms, 8, 4, single_click_callback);
	xv_main_loop(frame); 
} 


void
single_click_callback(pw, e, cs, r, state)
	Xv_window pw;
	Event	*e;
	Canvas_shell cs;
	Rectobj	r;
	int	state;
{
	printf("Clicked on color #%d\n", xv_get(r, RECTOBJ_BG));
}


/*
 * This function is written so that you can mostly cut and paste
 * it into your application.
 */
Frame
create_color_chooser(cms, columns, rows, callback_fn)
	Cms cms;
	int columns, rows;
	Proc_ptr callback_fn;
{
	Frame           frame;
	Canvas_shell	shell;
	Array_tile	array_tile;
	int		i;
	Rectobj_ops	*rectobj_ops;
	static Drawrect	proto_drawrect;

	frame = (Frame) xv_create(XV_NULL, FRAME_CMD,
				  FRAME_SHOW_RESIZE_CORNER, FALSE,
				  FRAME_LABEL, "Color Chooser",
				  NULL);

	xv_destroy(xv_get(frame, FRAME_CMD_PANEL));

	shell = (Canvas_shell) xv_create(frame, CANVAS_SHELL,
					 WIN_CMS, cms,
					 NULL);

	array_tile = (Array_tile) xv_create(shell, ARRAY_TILE,
				ARRAY_TILE_N_ROWS, rows,
				ARRAY_TILE_N_COLUMNS, columns,
				ARRAY_TILE_COLUMN_GAP, 2,
				ARRAY_TILE_ROW_GAP, 2,
				ARRAY_TILE_ROW_HEIGHT, 1,
				ARRAY_TILE_COLUMN_WIDTH, 1,
				/*
				 * There is no need for rubber band style
				 * selection, so setting the event proc to
				 * NULL prevents it because the array_tile 
				 * covers all of the space of the canvas
				 * shell.
				 */
				RECTOBJ_EVENT_PROC, NULL,
				NULL);

	if(proto_drawrect == XV_NULL) {
		/* 
		 * Create a DRAWRECT and set up the callbacks:
		 * The advantage is mostly performance, especially
		 * when creating lots of objects.
		 */
		proto_drawrect = (Drawrect) xv_create(XV_NULL, DRAWRECT,
			RECTOBJ_SINGLE_CLICK_PROC, callback_fn,
			RECTOBJ_EVENT_PROC, rectobj_button_event_proc,
			NULL);
	}
	rectobj_ops = (Rectobj_ops*) xv_get(proto_drawrect, RECTOBJ_OPS);

	i = (xv_get(cms, CMS_CONTROL_CMS) ? CMS_CONTROL_COLORS : 0);

	while (i < xv_get(cms, CMS_SIZE)) {
		xv_create(array_tile, DRAWRECT,
			RECTOBJ_OPS, rectobj_ops,
			XV_WIDTH, 20,
			XV_HEIGHT, 20,
			DRAWRECT_BORDER2, 2,
			DRAWOBJ_FILLED, TRUE,
			RECTOBJ_BG2, i,
			RECTOBJ_BG, i,
			NULL);

		i++;
	}

	xv_set(shell,
		/* +2 is for frame's border */
		XV_WIDTH, xv_get(array_tile, XV_WIDTH) + 2,
		XV_HEIGHT, xv_get(array_tile, XV_HEIGHT) + 2,
		NULL);

	window_fit(frame);

	return (frame);
}


