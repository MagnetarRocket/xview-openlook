/*
 * copyright 1993 Brian Warkentine
 * This program is functional, but unfinished.  It probably has bugs.
 *
 *	@(#) editcolor.c 1.8 93/10/25 
 */

#include <xview/frame.h>
#include <xview/panel.h>
#include <xview/cms.h>
#include <xview/font.h>
#include <sspkg/canshell.h> 
#include <sspkg/array.h> 
#include <sspkg/drawobj.h> 
#include <sspkg/box.h> 


/*
 * Copied from...
 *#ident "@(#)olgx_impl.h	1.17 92/07/17 SMI"
 */

#define	XRGB	0xffff
#define	MAXRGB	0xff
#define	MAXH	360
#define	MAXSV	1000

typedef struct {
    int         r, g, b;
} RGB;

typedef struct {
    int         h, s, v;
} HSV;

extern void hsv_to_rgb();
extern void rgb_to_hsv();
extern void rgb_to_xcolor();
extern void hsv_to_xcolor();
extern void xcolor_to_hsv();




typedef struct {
	Display		*display;
	Colormap	colormap;
	Frame           frame;
	Cms		cms;
	Panel           panel;
	Panel_item      red, green, blue;
	Panel_item      hue, shade, bright;

	Xv_font		font;

	Canvas_shell    shell;
	Cbox		cbox;
	Box		hbox, vbox;
	Array_tile      array_tile;

	Drawrect	a_colorchip;
	Drawrect	b_colorchip;

	Drawtext	colora;
	Drawrect	buttona;
	Drawtext	colorb;
	Drawrect	buttonb;
	Drawtext	floatera;
	Drawtext	floaterb;
	int		editing_a;

	int		color_selected;
	int		a_index;
	int		b_index;
	RGB		rgb;
	HSV		hsv;
	XColor		xcolor;
} Palette;
Palette p;

void create_palette();

main(argc, argv)
	int	argc;
	char	*argv[];
{
	Cms		cms;

	xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);

	cms = (Cms) xv_create(XV_NULL, CMS,
			CMS_TYPE, XV_DYNAMIC_CMS,
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

	create_palette(cms, 8, 4);
	xv_main_loop(p.frame); 
} 

void
set_hsv_sliders()
{
	xv_set(p.hue, PANEL_VALUE, p.hsv.h, NULL);
	xv_set(p.shade, PANEL_VALUE, p.hsv.s, NULL);
	xv_set(p.bright, PANEL_VALUE, p.hsv.v, NULL);
}

void
set_rgb_sliders()
{
	xv_set(p.red, PANEL_VALUE, p.rgb.r, NULL);

	xv_set(p.green, PANEL_VALUE, p.rgb.g, NULL);
	xv_set(p.blue, PANEL_VALUE, p.rgb.b, NULL);
}

void
set_hsv(item, value, event)
        Panel_item      item;
        int             value;
        Event           *event;
{
	switch(*(char *) xv_get(item, PANEL_LABEL_STRING)) {
	case 'H':
		p.hsv.h = value;
		break;
	case 'S':
		p.hsv.s = value;
		break;
	case 'V':
		p.hsv.v = value;
		break;
	}
	hsv_to_xcolor(&p.hsv, &p.xcolor);
	hsv_to_rgb(&p.hsv, &p.rgb);
	set_rgb_sliders();

	xv_set(p.cms,
		CMS_X_COLORS, &p.xcolor,
		CMS_INDEX, p.color_selected,
		CMS_COLOR_COUNT, 1,
		NULL);
}


void
set_rgb(item, value, event)
        Panel_item      item;
        int             value;
        Event           *event;
{
	switch(*(char *) xv_get(item, PANEL_LABEL_STRING)) {
	case 'R':
		p.rgb.r = value;
		break;
	case 'G':
		p.rgb.g = value;
		break;
	case 'B':
		p.rgb.b = value;
		break;
	}
	rgb_to_hsv(&p.rgb, &p.hsv);
	rgb_to_xcolor(&p.rgb, &p.xcolor);
	set_hsv_sliders();

	xv_set(p.cms,
		CMS_X_COLORS, &p.xcolor,
		CMS_INDEX, p.color_selected,
		CMS_COLOR_COUNT, 1,
		NULL);
}

void
set_all_sliders()
{
	p.xcolor.pixel = (unsigned long) 
			xv_get(p.cms, CMS_PIXEL, p.color_selected);
	XQueryColor(p.display, p.colormap, &p.xcolor);
	xcolor_to_hsv(&p.xcolor, &p.hsv);
	set_hsv_sliders();
	hsv_to_rgb(&p.hsv, &p.rgb);
	set_rgb_sliders();
}

void
position_floater(r)
	Rectobj r;
{
	Drawtext	floater;
	Drawtext	colorswab;

	if(p.editing_a) {
		floater = p.floatera;
		colorswab= p.colora;
		p.a_colorchip = r;
	} else {
		floater = p.floaterb;
		colorswab = p.colorb;
		p.b_colorchip = r;
	}

	xv_set(colorswab,
		RECTOBJ_BG, p.color_selected,
		NULL);

	xv_set(floater,
		RECTOBJ_BG, p.color_selected,
		RECTOBJ_X, xv_get(r, RECTOBJ_X) + 
		      xv_get(r, XV_WIDTH)/2 - xv_get(p.floatera, XV_WIDTH)/2,
		RECTOBJ_Y, xv_get(r, RECTOBJ_Y) + 
		      xv_get(r, XV_HEIGHT)/2 - xv_get(p.floatera, XV_HEIGHT)/2,
		RECTOBJ_STACKING_POSITION, 1000,
		NULL);
}

void
color_clicked(pw, e, cs, r)
	Xv_window pw;
	Event	*e;
	Canvas_shell cs;
	Rectobj	r;
{
	p.color_selected = xv_get(r, RECTOBJ_BG);

	set_all_sliders();
	position_floater(r);

	if(p.editing_a) {
		p.a_index = p.color_selected;
	} else {
		p.b_index = p.color_selected;
	}

	/* flush it nicely... */
	xv_set(p.shell,
		CANVAS_SHELL_BATCH_REPAINT, TRUE,
		NULL);
	rectobj_flush_repaint(TRUE);
	xv_set(p.shell,
		CANVAS_SHELL_BATCH_REPAINT, FALSE,
		NULL);

}


void 
ab_chosen(pw, e, cs, r)
	Xv_window pw;
	Event	*e;
	Canvas_shell cs;
	Rectobj	r;
{
	if (r == p.colora) {
		if(p.editing_a)
			return;
		p.editing_a = TRUE;

		xv_set(p.buttona,
			RECTOBJ_HIGHLIGHT,
			NULL);
		xv_set(p.buttonb,
			RECTOBJ_NORMAL,
			NULL);
		p.color_selected = xv_get(p.colora, RECTOBJ_BG);
		xv_set(p.a_colorchip, RECTOBJ_SELECTED, TRUE, NULL);
	} else {
		if(!p.editing_a)
			return;
		p.editing_a = FALSE;

		xv_set(p.buttona,
			RECTOBJ_NORMAL,
			NULL);
		xv_set(p.buttonb,
			RECTOBJ_HIGHLIGHT,
			NULL);
		p.color_selected = xv_get(p.colorb, RECTOBJ_BG);
		xv_set(p.b_colorchip, RECTOBJ_SELECTED, TRUE, NULL);
	}
	set_all_sliders();
}

void
blend_rgb()
{
	int i, start, end;
	XColor xcolor, xcolora, xcolorb;

	if (p.a_index < p.b_index) {
		start = p.a_index;
		end = p.b_index;
	} else {
		start = p.b_index;
		end = p.a_index;
	}

	xcolora.pixel = (unsigned long) xv_get(p.cms, CMS_PIXEL, p.a_index);
	xcolorb.pixel = (unsigned long) xv_get(p.cms, CMS_PIXEL, p.b_index);
	XQueryColor(p.display, p.colormap, &xcolora);
	XQueryColor(p.display, p.colormap, &xcolorb);

	xcolor.flags = DoRed | DoGreen | DoBlue;
	for (i = start + 1; i < end; i++) {

		if (xcolora.red < xcolorb.red) {
			xcolor.red = xcolora.red +
				(xcolorb.red - xcolora.red) * (i - start) /
				(unsigned long) (end - start);
		} else {
			xcolor.red = xcolorb.red +
				(xcolora.red - xcolorb.red) * (end - i) /
				(unsigned long) (end - start);
		}

		if (xcolora.green < xcolorb.green) {
			xcolor.green = xcolora.green +
				(xcolorb.green - xcolora.green) * (i - start) /
				(unsigned long) (end - start);
		} else {
			xcolor.green = xcolorb.green +
				(xcolora.green - xcolorb.green) * (end - i) /
				(unsigned long) (end - start);
		}

		if (xcolora.blue < xcolorb.blue) {
			xcolor.blue = xcolora.blue +
				(xcolorb.blue - xcolora.blue) * (i - start) /
				(unsigned long) (end - start);
		} else {
			xcolor.blue = xcolorb.blue +
				(xcolora.blue - xcolorb.blue) * (end - i) /
				(unsigned long) (end - start);
		}

		xcolor.pixel = (unsigned long) xv_get(p.cms, CMS_PIXEL, i);

		xv_set(p.cms,
		       CMS_X_COLORS, &xcolor,
		       CMS_INDEX, i,
		       CMS_COLOR_COUNT, 1,
		       NULL);
	}
}

void
panel_button_callback(item, event)
	Panel_item	item;
	Event		*event;
{

#define C_SWAP		1
#define C_COPY_A_TO_B	2
#define C_COPY_B_TO_A	3
#define	C_BLEND_H	4
#define	C_BLEND_RGB	5
#define C_UNDO		6


	XColor xcolor, xcolora, xcolorb;

	switch(xv_get(item, PANEL_CLIENT_DATA)) {

		case C_SWAP:
			xcolora.pixel = (unsigned long) xv_get(p.cms,
				CMS_PIXEL, p.a_index);
			XQueryColor(p.display, p.colormap, &xcolora);
			xcolorb.pixel = (unsigned long) xv_get(p.cms,
				CMS_PIXEL, p.b_index);
			XQueryColor(p.display, p.colormap, &xcolorb);

			xv_set(p.cms,
				CMS_X_COLORS, &xcolorb,
				CMS_INDEX, p.a_index,
				CMS_COLOR_COUNT, 1,
				NULL);

			xv_set(p.cms,
				CMS_X_COLORS, &xcolora,
				CMS_INDEX, p.b_index,
				CMS_COLOR_COUNT, 1,
				NULL);

			set_all_sliders();
			break;

		case C_COPY_A_TO_B:
			xcolor.pixel = (unsigned long) xv_get(p.cms,
				CMS_PIXEL, p.a_index);
			XQueryColor(p.display, p.colormap, &xcolor);
			xv_set(p.cms,
				CMS_X_COLORS, &xcolor,
				CMS_INDEX, p.b_index,
				CMS_COLOR_COUNT, 1,
				NULL);
			if(!p.editing_a)
				set_all_sliders();
			break;

		case C_COPY_B_TO_A:
			xcolor.pixel = (unsigned long) xv_get(p.cms,
				CMS_PIXEL, p.b_index);
			XQueryColor(p.display, p.colormap, &xcolor);
			xv_set(p.cms,
				CMS_X_COLORS, &xcolor,
				CMS_INDEX, p.a_index,
				CMS_COLOR_COUNT, 1,
				NULL);
			if(p.editing_a)
				set_all_sliders();
			break;

		case C_BLEND_H:
			xcolora.pixel = (unsigned long) xv_get(p.cms,
				CMS_PIXEL, p.a_index);
			XQueryColor(p.display, p.colormap, &xcolora);
			xcolorb.pixel = (unsigned long) xv_get(p.cms,
				CMS_PIXEL, p.b_index);
			XQueryColor(p.display, p.colormap, &xcolorb);
			break;

		case C_BLEND_RGB:
			blend_rgb();
			break;

		case C_UNDO:
			break;
	}
}

void
setup_panel()
{
	p.panel = xv_get(p.frame, FRAME_CMD_PANEL);

	xv_create(p.panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Copy A to B",
		PANEL_NOTIFY_PROC, panel_button_callback,
		PANEL_CLIENT_DATA, C_COPY_A_TO_B,
		NULL);

	xv_create(p.panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Copy B to A",
		PANEL_NOTIFY_PROC, panel_button_callback,
		PANEL_CLIENT_DATA, C_COPY_B_TO_A,
		NULL);

	xv_create(p.panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Swap",
		PANEL_NOTIFY_PROC, panel_button_callback,
		PANEL_CLIENT_DATA, C_SWAP,
		NULL);

	xv_create(p.panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Blend",
		PANEL_NOTIFY_PROC, panel_button_callback,
		PANEL_CLIENT_DATA, C_BLEND_RGB,
		NULL);

	/*
	xv_create(p.panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Undo",
		PANEL_NOTIFY_PROC, panel_button_callback,
		PANEL_CLIENT_DATA, C_UNDO,
		NULL);
	*/

	xv_set(p.panel, 
		PANEL_LAYOUT, PANEL_VERTICAL,
		NULL);

	p.red = xv_create(p.panel, PANEL_SLIDER,
			XV_X, 20,
			PANEL_VALUE_X, 40,
			PANEL_LABEL_STRING, "R:",
			PANEL_NOTIFY_PROC, set_rgb,
			PANEL_NOTIFY_LEVEL, PANEL_ALL,
			PANEL_MAX_VALUE, MAXRGB,
			PANEL_SHOW_RANGE, FALSE,
			PANEL_SHOW_VALUE, FALSE,
			NULL);

	p.green = xv_create(p.panel, PANEL_SLIDER,
			XV_X, 20,
			PANEL_VALUE_X, 40,
			PANEL_LABEL_STRING, "G:",
			PANEL_NOTIFY_PROC, set_rgb,
			PANEL_NOTIFY_LEVEL, PANEL_ALL,
			PANEL_MAX_VALUE, MAXRGB,
			PANEL_SHOW_RANGE, FALSE,
			PANEL_SHOW_VALUE, FALSE,
			NULL);

	p.blue = xv_create(p.panel, PANEL_SLIDER,
			XV_X, 20,
			PANEL_VALUE_X, 40,
			PANEL_LABEL_STRING, "B:",
			PANEL_NOTIFY_PROC, set_rgb,
			PANEL_NOTIFY_LEVEL, PANEL_ALL,
			PANEL_MAX_VALUE, MAXRGB,
			PANEL_SHOW_RANGE, FALSE,
			PANEL_SHOW_VALUE, FALSE,
			NULL);

	p.hue = xv_create(p.panel, PANEL_SLIDER,
			XV_X, 20,
			PANEL_VALUE_X, 40,
			PANEL_LABEL_STRING, "H:",
			PANEL_NOTIFY_PROC, set_hsv,
			PANEL_NOTIFY_LEVEL, PANEL_ALL,
			PANEL_MAX_VALUE, MAXH,
			PANEL_SHOW_RANGE, FALSE,
			PANEL_SHOW_VALUE, FALSE,
			NULL);

	p.shade = xv_create(p.panel, PANEL_SLIDER,
			XV_X, 20,
			PANEL_VALUE_X, 40,
			PANEL_LABEL_STRING, "S:",
			PANEL_NOTIFY_PROC, set_hsv,
			PANEL_NOTIFY_LEVEL, PANEL_ALL,
			PANEL_MAX_VALUE, MAXSV,
			PANEL_SHOW_RANGE, FALSE,
			PANEL_SHOW_VALUE, FALSE,
			NULL);
	
	p.bright = xv_create(p.panel, PANEL_SLIDER,
			XV_X, 20,
			PANEL_VALUE_X, 40,
			PANEL_LABEL_STRING, "V:",
			PANEL_NOTIFY_PROC, set_hsv,
			PANEL_NOTIFY_LEVEL, PANEL_ALL,
			PANEL_MAX_VALUE, MAXSV,
			PANEL_SHOW_RANGE, FALSE,
			PANEL_SHOW_VALUE, FALSE,
			NULL);

	window_fit(p.panel);
}

void
floater_event_proc(paint_window, event, canvas_shell, rectobj)
	Xv_window       paint_window;
	Event          *event;
	Canvas_shell    canvas_shell;
	Rectobj         rectobj;
{
	/* pass the event on to the object that the floater is on top of */
	rectobj_selection_event_proc(paint_window, event, canvas_shell,
		(rectobj == p.colora ? p.a_colorchip : p.b_colorchip));
}

void
setup_shell(columns, rows)
	int columns, rows;
{
	int		i;
	Rectobj_ops	*rectobj_ops;
	static Drawrect	proto_drawrect;

	p.font = xv_find(p.frame, FONT,
			FONT_NAME, "helvetica-bold-14",
			NULL);

	p.shell = (Canvas_shell) xv_create(p.frame, CANVAS_SHELL,
					XV_X, 0,
					XV_Y, 0,
					NULL);

	p.cbox = (Box) xv_create(p.shell, CBOX,
			RECTOBJ_EVENT_PROC, NULL,
			NULL);

	p.vbox = (Box) xv_create(p.cbox, BOX,
			RECTOBJ_EVENT_PROC, NULL,
			RECTOBJ_BORDER, 4,
			BOX_GAP, 4,
			BOX_LAYOUT, BOX_LAYOUT_VERTICAL,
			NULL);

	p.array_tile = (Array_tile) xv_create(p.vbox, ARRAY_TILE,
				ARRAY_TILE_N_ROWS, rows,
				ARRAY_TILE_N_COLUMNS, columns,
				ARRAY_TILE_COLUMN_GAP, 1,
				ARRAY_TILE_ROW_GAP, 1,
				ARRAY_TILE_ROW_HEIGHT, 1,
				ARRAY_TILE_COLUMN_WIDTH, 1,
				RECTOBJ_EVENT_PROC, NULL,
				NULL);

	if(proto_drawrect == XV_NULL) {
		/* 
		 * Create a DRAWRECT and set up the callbacks:
		 * The advantage is mostly performance, especially
		 * when creating lots of objects.
		 */
		proto_drawrect = (Drawrect) xv_create(XV_NULL, DRAWRECT,
			RECTOBJ_SINGLE_CLICK_PROC, color_clicked,
			RECTOBJ_EVENT_PROC, rectobj_selection_event_proc,
			NULL);
	}
	rectobj_ops = (Rectobj_ops*) xv_get(proto_drawrect, RECTOBJ_OPS);

	i = (xv_get(p.cms, CMS_CONTROL_CMS) ? CMS_CONTROL_COLORS : 0);


	while (i < xv_get(p.cms, CMS_SIZE)) {
		xv_create(p.array_tile, DRAWRECT,
			RECTOBJ_OPS, rectobj_ops,
			RECTOBJ_EXCLUSIVE_SELECT, TRUE,
			RECTOBJ_SELECTABLE, TRUE,
			XV_WIDTH, 20,
			XV_HEIGHT, 20,
			DRAWRECT_BORDER2, 2,
			DRAWOBJ_FILLED, TRUE,
			RECTOBJ_BG2, i,
			RECTOBJ_BG, i,
			NULL);

		i++;
	}

	p.hbox = xv_create(p.vbox, BOX,
		BOX_GAP, 20,
		RECTOBJ_EVENT_PROC, NULL,
		NULL);

	p.buttona = xv_create(p.hbox, DRAWRECT,
		DRAWRECT_BORDER1, 2,
		DRAWRECT_BORDER2, 3,
		DRAWOBJ_OPAQUE, FALSE,
		RECTOBJ_HIGHLIGHT,
		RECTOBJ_EVENT_PROC, NULL,
		NULL);

	p.colora = xv_create(p.buttona, DRAWTEXT,
		RECTOBJ_SINGLE_CLICK_PROC, ab_chosen,
		RECTOBJ_EVENT_PROC, rectobj_button_event_proc,
		DRAWTEXT_STRING, "Color A",
		DRAWTEXT_FONT, p.font,
		DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_CENTER,
		RECTOBJ_FG, 0,
		XV_WIDTH, 96,
		NULL);

	p.buttonb = xv_create(p.hbox, DRAWRECT,
		DRAWRECT_BORDER1, 2,
		DRAWRECT_BORDER2, 3,
		DRAWOBJ_OPAQUE, FALSE,
		RECTOBJ_NORMAL,
		RECTOBJ_EVENT_PROC, NULL,
		NULL);

	p.colorb = xv_create(p.buttonb, DRAWTEXT,
		RECTOBJ_SINGLE_CLICK_PROC, ab_chosen,
		RECTOBJ_EVENT_PROC, rectobj_button_event_proc,
		DRAWTEXT_STRING, "Color B",
		DRAWTEXT_FONT, p.font,
		DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_CENTER,
		RECTOBJ_FG, 0,
		XV_WIDTH, 96,
		NULL);

	p.floatera = xv_create(p.shell, DRAWTEXT,
		DRAWTEXT_STRING, "A",
		DRAWTEXT_FONT, p.font,
		DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_CENTER,
		RECTOBJ_EVENT_PROC, floater_event_proc,
		RECTOBJ_FG, 0,
		XV_X, -100,
		NULL);

	p.floaterb = xv_create(p.shell, DRAWTEXT,
		DRAWTEXT_STRING, "B",
		DRAWTEXT_FONT, p.font,
		DRAWTEXT_JUSTIFY, DRAWTEXT_JUSTIFY_CENTER,
		RECTOBJ_EVENT_PROC, floater_event_proc,
		RECTOBJ_FG, 0,
		XV_X, -100,
		NULL);
}


void
create_palette(cms, columns, rows)
	Cms cms;
	int columns, rows;
{
	Rectobj	tmp;

	p.cms = cms;
	p.frame = (Frame) xv_create(XV_NULL, FRAME_CMD,
				  FRAME_SHOW_RESIZE_CORNER, FALSE,
				  FRAME_LABEL, "Color Chooser",
				  WIN_CMS, cms,
				  FRAME_CMD_DEFAULT_PIN_STATE, FRAME_CMD_PIN_IN,
				  NULL);

	p.display = (Display *) xv_get(p.frame, XV_DISPLAY);
	p.colormap= (Colormap) xv_get(p.cms, XV_XID);

	setup_panel();
	setup_shell(columns, rows);

	xv_set(p.shell,
		/* assume the panel is wider */
		XV_WIDTH, xv_get(p.panel, XV_WIDTH),
		XV_HEIGHT, xv_get(p.cbox, XV_HEIGHT) + 2,
		NULL);

	xv_set(p.cbox,
		XV_WIDTH, xv_get(p.panel, XV_WIDTH),
		NULL);

	tmp = xv_get(p.array_tile, RECTOBJ_NTH_CHILD, 1);
	p.color_selected = xv_get(tmp, RECTOBJ_BG);
	p.b_index = p.color_selected;
	p.editing_a = FALSE;
	position_floater(tmp);

	tmp = xv_get(p.array_tile, RECTOBJ_NTH_CHILD, 0);
	p.color_selected = xv_get(tmp, RECTOBJ_BG);
	p.a_index = p.color_selected;
	p.editing_a = TRUE;
	position_floater(tmp);
	xv_set(tmp, RECTOBJ_SELECTED, TRUE, NULL);
	set_all_sliders();

	xv_set(p.panel,
		XV_Y, xv_get(p.shell, XV_HEIGHT),
		NULL);

	window_fit(p.frame);

	return;
}

