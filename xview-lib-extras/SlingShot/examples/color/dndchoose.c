/*
 *	(c) Copyright 1991 Sun Microsystems, Inc.  All rights reserved.
 *	See LEGAL_NOTICE file for terms of the license.
 *
 *	@(#) dndchoose.c 1.11 93/04/19 
 *
 *	This program demonstrates further uses of dnd and color.
 *	You can drag colors from the array of colored objects
 *	and drop them in the drop "bullseye" target.  
 *	Both the drop site and the draggable objects are in this one 
 *	executable, but could have been seperate processes.  For
 *	example, you can select the name of a color (e.g. "orange") in a
 *	cmdtool and drag it into the bullseye.  You can also drag the
 *	color into a cmdtool or textedit window.
 */

#include <xview/frame.h>
#include <xview/cms.h>
#include <xview/cursor.h>
#include <xview/svrimage.h>
#include <xview/dragdrop.h>
#include <sspkg/canshell.h> 
#include <sspkg/array.h> 
#include <sspkg/drawobj.h> 


Frame create_color_chooser();
void single_click_callback();

#define COLOR_NAME_FROM_OBJECT(_object_) (colors[xv_get(_object_, RECTOBJ_BG)])

static char * colors[] = {
			/* using a control cms, so these will be ignored */
			"color0", "color1", "color2", "color3",

			"#000000", "#000000", "#4c4c4c", "#666666",
			"#999999", "#b2b2b2", "#cccccc", "#ffffff",
			"#bf9898", "#bf7272", "#bf4c4c", "#bf2626",
			"#e5b7b7", "#e58989", "#e55b5b", "#e52d2d",
			"#bfb498", "#bfaa72", "#bf9e4c", "#bf9426",
			"#e5d8b7", "#e5cb89", "#e5be5b", "#e5b12d",
			"#bdbf98", "#bbbf72", "#b9bf4c", "#b7bf26",
			"#e3e5b7", "#e0e589", "#dee55b", "#dce52d",
			"#98bfa2", "#72bf86", "#4cbf69", "#26bf4c",
			"#b7e5c2", "#89e5a0", "#5be57e", "#2de55b",
			"#98bfbf", "#72bfbf", "#4cbfbf", "#26bfbf",
			"#b7e5e5", "#89e5e5", "#5be5e5", "#2de5e5",
			"#98a2bf", "#7286bf", "#4c69bf", "#264cbf",
			"#b7c2e5", "#89a0e5", "#5b7ee5", "#2d5be5",
			"#b298bf", "#a572bf", "#984cbf", "#8c26bf",
			"#d5b7e5", "#c689e5", "#b75be5", "#a72de5",
			"#bf98b6", "#bf72ac", "#bf4ca2", "#bf2698",
			"#e5b7da", "#e589ce", "#e55bc2", "#e52db7",
	};

main(argc, argv)
	int	argc;
	char	*argv[];
{
	Frame           frame;
	Cms		cms;

	xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);
 
	cms = (Cms) xv_create(XV_NULL, CMS,
			CMS_CONTROL_CMS, TRUE,
			CMS_SIZE, CMS_CONTROL_COLORS + 72,
			CMS_NAMED_COLORS,
				colors[4], colors[5], colors[6], colors[7],
				colors[8], colors[9], colors[10], colors[11],
				colors[12], colors[13], colors[14], colors[15],
				colors[16], colors[17], colors[18], colors[19],
				colors[20], colors[21], colors[22], colors[23],
				colors[24], colors[25], colors[26], colors[27],
				colors[28], colors[29], colors[30], colors[31],
				colors[32], colors[33], colors[34], colors[35],
				colors[36], colors[37], colors[38], colors[39],
				colors[40], colors[41], colors[42], colors[43],
				colors[44], colors[45], colors[46], colors[47],
				colors[48], colors[49], colors[50], colors[51],
				colors[52], colors[53], colors[54], colors[55],
				colors[56], colors[57], colors[58], colors[59],
				colors[60], colors[61], colors[62], colors[63],
				colors[64], colors[65], colors[66], colors[67],
				colors[68], colors[69], colors[70], colors[71],
				colors[72], colors[73], colors[74], colors[75],
				NULL,
			NULL);

	frame = create_color_chooser(cms, 8, 9, single_click_callback);

	create_target();

	xv_main_loop(frame); 
} 

/* drop site code */

Cms		dropsite_cms;

#define DROPSITE_BG	"PeachPuff1"

void
set_dropsite_color(colorname)
	char *colorname;
{
	xv_set(dropsite_cms,
		CMS_NAMED_COLORS, DROPSITE_BG, colorname, NULL,
		NULL);
}

void
drop_callback_fn(paint_window, event, shell, drawtext)
        Xv_window       paint_window;
        Event           *event;
        Canvas_shell    shell;
        Drawtext        drawtext;
{
	Selection_requestor sel_req;

	switch (event_action(event)) {
	case ACTION_DRAG_PREVIEW:
		if(event_id(event) == LOC_WINENTER) {
			/* show something */
		} else
		if(event_id(event) == LOC_WINEXIT) {
			/* hide it */
		} else
		if(event_id(event) == LOC_DRAG) {
			/* ignore */
		}
		return;

	case ACTION_DRAG_COPY:
	case ACTION_DRAG_MOVE:
	case ACTION_DRAG_LOAD:{
			int             length, format;
			char           *string;
			char            str[60];

			sel_req = xv_create(shell, SELECTION_REQUESTOR, NULL);

			if (dnd_decode_drop(sel_req, event) == XV_ERROR)
				break;

			xv_set(sel_req,
			       SEL_TYPE, XA_STRING,
			       NULL);

			string = (char *) xv_get(sel_req,
						 SEL_DATA, &length, &format);

			if (length != SEL_ERROR) {
				set_dropsite_color(string);
				free(string);
			}

			if (event_action(event) == ACTION_DRAG_MOVE) {
				int             length, format;

				xv_set(sel_req, SEL_TYPE_NAME, "DELETE", 0);
				(void) xv_get(sel_req,
					      SEL_DATA, &length, &format);
			}

			dnd_done(sel_req);
		}
		break;
	}
	xv_destroy(sel_req);
}


create_target()
{
	Frame           frame2;
	Drawarea        dropsite_drawarea;
	Canvas_shell    dropsite_shell;

	frame2 = (Frame) xv_create(XV_NULL, FRAME_CMD,
			XV_WIDTH, 300,
			XV_HEIGHT, 60,
			FRAME_SHOW_RESIZE_CORNER, FALSE,
			FRAME_LABEL, "Drop here...",
			XV_SHOW, TRUE,
			NULL);

	xv_destroy(xv_get(frame2, FRAME_CMD_PANEL));

	dropsite_shell = (Canvas_shell) xv_create(frame2, CANVAS_SHELL,
			CANVAS_SHELL_AUTO_DROP_SITE, TRUE,
			NULL);

	dropsite_drawarea = (Drawarea) xv_create(dropsite_shell, DRAWAREA,
			RECTOBJ_ACCEPTS_DROP, TRUE,
			RECTOBJ_DROP_PROC, drop_callback_fn,
			XV_WIDTH, 298,
			XV_HEIGHT, 58,
			NULL);

	dropsite_cms = (Cms) xv_create(XV_NULL, CMS,
		CMS_TYPE, XV_DYNAMIC_CMS,
		CMS_SIZE, 2,
		CMS_NAMED_COLORS, DROPSITE_BG, "BlanchedAlmond", NULL,
		NULL);

	set_dropsite_color("BlanchedAlmond");

	xv_set(dropsite_shell,
		WIN_CMS, dropsite_cms,
		NULL);

	VSetColor(dropsite_drawarea, 1);
	VFillArc(dropsite_drawarea, 500, 500, 9000, 9000, 0, 360 * 64);
	VSetColor(dropsite_drawarea, 0);
	VFillArc(dropsite_drawarea, 1000, 1000, 8000, 8000, 0, 360 * 64);

	VSetColor(dropsite_drawarea, 1);
	VFillArc(dropsite_drawarea, 2000, 2000, 6000, 6000, 0, 360 * 64);
	VSetColor(dropsite_drawarea, 0);
	VFillArc(dropsite_drawarea, 2600, 2600, 4900, 4900, 0, 360 * 64);

	VSetColor(dropsite_drawarea, 1);
	VFillArc(dropsite_drawarea, 3500, 3500, 3000, 3000, 0, 360 * 64);
}



void
single_click_callback(pw, e, cs, r, state)
	Xv_window pw;
	Event	*e;
	Canvas_shell cs;
	Rectobj	r;
	int	state;
{
	set_dropsite_color(COLOR_NAME_FROM_OBJECT(r));
}


/* drag code */

static Dnd      drag_dnd;
static Selection_item drag_sel_item;
Xv_Cursor cursor1, cursor2;

static unsigned short drag_cursor1_bits[] = {
#include "../icons/paintbrush.icon"
};

static unsigned short drag_cursor2_bits[] = {
#include "../icons/nopaintbrush.icon"
};


Xv_Cursor
create_cursor(bits)
	unsigned short bits[];
{
	Xv_Cursor cursor;

	cursor = (Xv_Cursor) xv_create(XV_NULL, CURSOR,
		CURSOR_IMAGE, xv_create(XV_NULL, SERVER_IMAGE,
				SERVER_IMAGE_BITS, bits,
				SERVER_IMAGE_DEPTH, 1,
				XV_WIDTH, 16,
				XV_HEIGHT, 16,
				NULL),
		CURSOR_OP, 	PIX_SRC^PIX_DST,
		NULL);
	
	return cursor;
}

Xv_singlecolor *
cursor_color(shell, rectobj)
	Canvas_shell shell;
	Rectobj rectobj;
{
	/*
	 * This goes through the hoops to get to the color info.
	 * You could probably get this information with CMS_COLORS
	 * attribute.  But that winds up doing a XQueryColors call
	 * so this actually performs better.
	 */
	static Xv_singlecolor xvsc;
	XColor xc;
	Cms cms;

	cms = (Cms) xv_get(shell, WIN_CMS);

	xc.pixel = (unsigned long) xv_get(cms, 
			CMS_PIXEL, xv_get(rectobj, RECTOBJ_BG));
	XQueryColor(
		(Display *) xv_get(shell, XV_DISPLAY),
		(Colormap) xv_get(cms, XV_XID),
		&xc);

	xvsc.red = xc.red;
	xvsc.blue = xc.blue;
	xvsc.green = xc.green;

	return &xvsc;
}


void
drag_start(paint_window, event, canvas_shell, rectobj, x, y, adjust)
	Xv_window       paint_window;
	Event          *event;
	Canvas_shell    canvas_shell;
	Rectobj         rectobj;
	int             x, y, adjust;
{
	Xv_singlecolor xvsc;

	if(cursor1 == NULL) {
		cursor1 = create_cursor(drag_cursor1_bits);
		cursor2 = create_cursor(drag_cursor2_bits);
	}

	xv_set(cursor1,
		CURSOR_BACKGROUND_COLOR, cursor_color(canvas_shell, rectobj),
		NULL);

	xv_set(cursor2,
		CURSOR_BACKGROUND_COLOR, cursor_color(canvas_shell, rectobj),
		NULL);

	xv_set(drag_dnd,
		DND_ACCEPT_CURSOR, cursor1,
		DND_CURSOR, cursor2,
		NULL);

	xv_set(drag_sel_item,
	       SEL_TYPE, (Atom) XA_STRING,
	       SEL_DATA, (Xv_opaque) COLOR_NAME_FROM_OBJECT(rectobj),
	       0);

	switch (dnd_send_drop(drag_dnd)) {
		/* ... */
		case XV_OK:
		case DND_ABORTED:
		case DND_TIMEOUT:
		case DND_ROOT:
		case DND_ILLEGAL_TARGET:
		case DND_SELECTION:
		case XV_ERROR:
			/* reset the object, because that looks nicer. */
			xv_set(rectobj, RECTOBJ_NORMAL, NULL);
			break;
	}

}


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
				  FRAME_LABEL, "Drag these...",
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
			RECTOBJ_EVENT_PROC, rectobj_selection_event_proc,
			RECTOBJ_START_DRAG_PROC, drag_start,
			NULL);
	}
	rectobj_ops = (Rectobj_ops*) xv_get(proto_drawrect, RECTOBJ_OPS);

	i = (xv_get(cms, CMS_CONTROL_CMS) ? CMS_CONTROL_COLORS : 0);

	while (i < xv_get(cms, CMS_SIZE)) {
		xv_create(array_tile, DRAWRECT,
			RECTOBJ_OPS, rectobj_ops,
			RECTOBJ_DRAGGABLE, TRUE,
			XV_WIDTH, 20,
			XV_HEIGHT, 20,
			DRAWRECT_BORDER2, 2,
			DRAWOBJ_FILLED, TRUE,
			RECTOBJ_BG2, i,
			RECTOBJ_BG, i,
			NULL);

		i++;
	}

	drag_dnd = xv_create(canvas_paint_window(shell), DRAGDROP,
		DND_TYPE, DND_COPY,
		NULL);
 
	drag_sel_item = xv_create(drag_dnd, SELECTION_ITEM, NULL);
 
	xv_set(shell,
		XV_WIDTH, xv_get(array_tile, XV_WIDTH) + 2,
		XV_HEIGHT, xv_get(array_tile, XV_HEIGHT) + 2,
		NULL);

	window_fit(frame);

	return (frame);
}

