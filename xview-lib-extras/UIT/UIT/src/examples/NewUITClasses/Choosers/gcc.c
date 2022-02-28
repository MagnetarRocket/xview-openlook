/*
 * This file is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify this file without charge, but are not authorized to
 * license or distribute it to anyone else except as part of a product
 * or program developed by the user.
 * 
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 * 
 * This file is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 * 
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS FILE
 * OR ANY PART THEREOF.
 * 
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even
 * if Sun has been advised of the possibility of such damages.
 * 
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

#ifndef lint
static char	sccsid[] = "@(#)gcc.c	2.9 91/10/15 Copyright 1989 Sun Microsystems";
#endif

#include	<stdio.h>
#include	<sys/param.h>
#include	<sys/types.h>
#include	<xview/xview.h>
#include	<xview/panel.h>
#include	<xview/cms.h>
#include	<xview/svrimage.h>
#include	"gcm.h"
#include	"gcc_ui.h"

#define CC_COLOR_KEY INSTANCE+1
#define CC_DATA_KEY  INSTANCE+2

void			(*Callback)();
static void		set_color_field();
static void		fill_color_list();

/*
 * Initialize the gcc popup window with a title.
 */
gcc_gccwin_objects *
#ifdef __STDC__
gcc_initialize(Xv_opaque owner, char *title)
#else
gcc_initialize(owner, title)
	Xv_opaque	owner;
	char		*title;
#endif
{
	gcc_gccwin_objects *gcc_win = gcc_gccwin_objects_initialize(NULL, owner);
	xv_set(gcc_win->gccwin, XV_LABEL, title, 0);

	fill_color_list(gcc_win);

	set_color_field(gcc_win, Gcm_colornames[0]);
	return gcc_win;
}

/*
 * Activate the color popup.  Set footer messages, current color, and
 * register a callback function to be called when the apply button is
 * pressed.  Store some client data for the caller.
 */
void
#ifdef __STDC__
gcc_activate(gcc_gccwin_objects *gcc_win, char *left, char *right, void (*func)(),
	     caddr_t client_data, char *color)
#else
gcc_activate(gcc_win, left, right, func, client_data, color)
	gcc_gccwin_objects *gcc_win;
	char	*left;
	char	*right;
	void	(*func)();
	caddr_t	client_data;
	char	*color;
#endif
{
char   *current_color = (char *) NULL;
int	index;

	xv_set(gcc_win->gccwin, FRAME_LEFT_FOOTER, left, 0);
	xv_set(gcc_win->gccwin, FRAME_RIGHT_FOOTER, right, 0);
	xv_set(gcc_win->gccwin, WIN_SHOW, TRUE, WIN_FRONT, 0);

	xv_set(gcc_win->apply, PANEL_INACTIVE, FALSE, 0);
	xv_set(gcc_win->reset, PANEL_INACTIVE, FALSE, 0);

	if((index = gcm_color_index(color)) == -1)
		index = gcm_color_index("black");

	xv_set(gcc_win->color_list, PANEL_LIST_SELECT, index, TRUE, 0);

	if (color) current_color = strdup (color);
	xv_set(gcc_win->apply, XV_KEY_DATA, CC_COLOR_KEY, current_color, 0);
	xv_set(gcc_win->reset, XV_KEY_DATA, CC_COLOR_KEY, current_color, 0);
	set_color_field(gcc_win, current_color);

	xv_set(gcc_win->apply, XV_KEY_DATA, CC_DATA_KEY, client_data, 0);
	xv_set(gcc_win->reset, XV_KEY_DATA, CC_DATA_KEY, client_data, 0);

	Callback = func;
}

/*
 * Notify callback function for `color_list'.
 */
/* ARGSUSED */
int
#ifdef __STDC__
gcc_list_proc(Panel_item item, char *string, Xv_opaque client_data,
	      Panel_list_op op, Event *event)
#else
gcc_list_proc(item, string, client_data, op, event)
	Panel_item	item;
	char		*string;
	Xv_opaque	client_data;
	Panel_list_op	op;
	Event		*event;
#endif
{
	gcc_gccwin_objects *gcc_win;

	switch(op)
	{
	case PANEL_LIST_OP_DESELECT:
		break;

	case PANEL_LIST_OP_SELECT:
		gcc_win = (gcc_gccwin_objects *)
			  xv_get (item, XV_KEY_DATA, INSTANCE);
		set_color_field(gcc_win, string);
		break;

	case PANEL_LIST_OP_VALIDATE:
		break;

	case PANEL_LIST_OP_DELETE:
		break;
	}
	return XV_OK;
}

/*
 * Notify callback function for `apply'.
 */
/*ARGSUSED*/
void
#ifdef __STDC__
gcc_apply(Panel_item item, Event *event)
#else
gcc_apply(item, event)
	Panel_item	item;
	Event		*event;
#endif
{
	caddr_t		    client_data;
	char               *current_color;
	gcc_gccwin_objects *gcc_win;

	gcc_win = (gcc_gccwin_objects *)
		  xv_get (item, XV_KEY_DATA, INSTANCE);

	current_color = (char *) xv_get (item, XV_KEY_DATA, CC_COLOR_KEY);
	if (current_color) free (current_color);

	current_color = strdup((char *)xv_get(gcc_win->color_name,
					      PANEL_VALUE));
	set_color_field(gcc_win, current_color);

	client_data = (caddr_t) xv_get (item, XV_KEY_DATA, CC_DATA_KEY);
	if(Callback)
		(Callback)(current_color, client_data);
	free (current_color);
}

/*
 * Notify callback function for `reset'.
 */
/*ARGSUSED*/
void
#ifdef __STDC__
gcc_reset(Panel_item item, Event *event)
#else
gcc_reset(item, event)
	Panel_item	item;
	Event		*event;
#endif
{
	char		   *current_color;
	gcc_gccwin_objects *gcc_win;

	gcc_win = (gcc_gccwin_objects *)
		  xv_get (item, XV_KEY_DATA, INSTANCE);
	current_color = (char *) xv_get (item, XV_KEY_DATA, CC_COLOR_KEY);
	set_color_field(gcc_win, current_color);

	xv_set(gcc_win->reset, PANEL_NOTIFY_STATUS, XV_ERROR, 0);
}

/*
 * Set a color name into the text field and change the color on the blot.
 */
static void
#ifdef __STDC__
set_color_field(gcc_gccwin_objects *gcc_win, char *color)
#else
set_color_field(gcc_win, color)
	gcc_gccwin_objects *gcc_win;
	char	           *color;
#endif
{
int	index;

	if((index = gcm_color_index(color)) == -1)
		index = gcm_color_index("black");

	xv_set(gcc_win->color_name, PANEL_VALUE, color, 0);
	xv_set(gcc_win->color_blot, PANEL_ITEM_COLOR, index, 0);
}

/*
 * Fill the scrolling list with all of the available colors.  Only
 * fill it with the names if color isn't available.
 */
static void
#ifdef __STDC__
fill_color_list(gcc_gccwin_objects *gcc_win)
#else
fill_color_list(gcc_win)
	gcc_gccwin_objects *gcc_win;
#endif
{
	Display		*display = (Display *)xv_get(gcc_win->gccwin, XV_DISPLAY);
	Drawable	xid = (Drawable)xv_get(gcc_win->gccwin, XV_XID);
	int		depth = (unsigned int)xv_get(gcc_win->gccwin, XV_DEPTH);
	int		visual_class;
	int		row = 0;
	int		size;
	int		color_available = FALSE;
	int		color_index;
	char		**color;
	unsigned long	*pixel_table;
	GC		gc;
	Pixmap		pixmap;
	Xv_opaque	color_glyph;

	visual_class = (int)xv_get(gcc_win->gccwin, XV_VISUAL_CLASS);

	if ((depth > 7) &&
	    ((visual_class == StaticColor) || (visual_class == PseudoColor) ||
	     (visual_class == TrueColor) || (visual_class == DirectColor)))
		color_available = TRUE;
	/* MOOSE: SHould we worry about .Xdefaults entries here? Like OpenWindows.3DLook.Color */

	if (color_available)
	{
		gcm_initialize_colors(gcc_win->controls, NULL, NULL);
		pixel_table = (unsigned long *)xv_get(xv_get(gcc_win->controls,
							     WIN_CMS),
						      CMS_INDEX_TABLE);
		gc = XCreateGC(display, xid, 0, NULL);
		size = (int)xv_get(gcc_win->color_list,
				   PANEL_LIST_ROW_HEIGHT) - 2;
	}

	for(color = Gcm_colornames; *color; row++, color++)
	{
		if (color_available)
		{
			pixmap = XCreatePixmap(display, xid, size,
					       size, depth);
			color_index = pixel_table[gcm_color_index(*color)];
			XSetForeground(display, gc, color_index);
			XFillRectangle(display, pixmap, gc, 0, 0, size, size);

			color_glyph =
				xv_create(0, SERVER_IMAGE,
					  SERVER_IMAGE_COLORMAP, gcm_colormap_name(),
					  SERVER_IMAGE_PIXMAP,	pixmap,
						0);

			xv_set(gcc_win->color_list,
			       PANEL_LIST_INSERT, row,
			       PANEL_LIST_GLYPH, row, color_glyph,
			       PANEL_LIST_STRING, row, *color,
			       0);
		}
		else
		{
			xv_set(gcc_win->color_list,
			       PANEL_LIST_INSERT, row,
			       PANEL_LIST_STRING, row, *color,
			       0);
		}
	}

	if (color_available)
		XFreeGC(display, gc);
}
