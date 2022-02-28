/*
 *	(c) Copyright 1993 Brian Warkentine.  All rights reserved.
 *
 * 	@(#) group1.c 1.3 93/11/02 
 */
#include <xview/xview.h>
#include <sspkg/canshell.h>
#include <sspkg/rectobj.h>
#include <sspkg/drawobj.h>
#include <sspkg/box.h>

static unsigned short smiley_bits[] = {
#include "../icons/smiley.icon"
};

static unsigned short frown_bits[] = {
#include "../icons/frown.icon"
};


Server_image    smiley, frown;


void
make_some_group_members(parent, string)
	Rectobj parent;
	char *string;
{

	(void) xv_create(parent, DRAWTEXT,
		DRAWTEXT_STRING, string,
		NULL);

	(void) xv_create(parent, DRAWICON,
		DRAWIMAGE_SVRIMAGE, smiley,
		DRAWIMAGE_HIGHLIGHT_IMAGE, frown,
		DRAWTEXT_STRING, "Click Me!",
		NULL);

	(void) xv_create(parent, DRAWICON,
		DRAWIMAGE_SVRIMAGE, smiley,
		DRAWIMAGE_HIGHLIGHT_IMAGE, frown,
		DRAWTEXT_STRING, "Click Me!",
		NULL);

	(void) xv_create(parent, DRAWICON,
		DRAWIMAGE_SVRIMAGE, smiley,
		DRAWIMAGE_HIGHLIGHT_IMAGE, frown,
		DRAWTEXT_STRING, "Click Me!",
		NULL);

}

make_group_div(parent)
	Xv_object parent;
{
/* purely for esthetic looks */
	xv_create(parent, DRAWRECT,
		XV_WIDTH, 1,
		XV_HEIGHT, 150,
		NULL);
}




main(argc, argv)
	int             argc;
	char           *argv[];

{
	Frame		frame;
	Canvas_shell	shell;
	Bag		topbox, box1, box2, box3, box4;
	

	xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);

	frame = (Frame) xv_create(XV_NULL, FRAME,
			FRAME_LABEL, argv[0],
			NULL);

	smiley = (Server_image) xv_create(XV_NULL, SERVER_IMAGE,
			XV_WIDTH, 32,
			XV_HEIGHT, 20,
			SERVER_IMAGE_BITS, smiley_bits,
			NULL);

	frown = (Server_image) xv_create(XV_NULL, SERVER_IMAGE,
			XV_WIDTH, 32,
			XV_HEIGHT, 20,
			SERVER_IMAGE_BITS, frown_bits,
			NULL);

	shell = (Canvas_shell) xv_create(frame, CANVAS_SHELL,
			NULL);
	
	topbox = xv_create(shell, BOX,
		BOX_LAYOUT, BOX_LAYOUT_HORIZONTAL,
		BOX_GAP, 30,
		RECTOBJ_BORDER, 20,
		NULL);

	box1 = xv_create(topbox, BOX,
		BOX_LAYOUT, BOX_LAYOUT_VERTICAL,
		BOX_GAP, 10,
		NULL);

	make_some_group_members(box1, "Global Selection");
	make_group_div(topbox);

	box2 = xv_create(topbox, BOX,
		RECTOBJ_HIGHLIGHT_GROUP, TRUE,
		BOX_LAYOUT, BOX_LAYOUT_VERTICAL,
		BOX_GAP, 10,
		NULL);

	make_some_group_members(box2, "Selection Group 1");
	make_group_div(topbox);

	box3 = xv_create(topbox, BOX,
		RECTOBJ_HIGHLIGHT_GROUP, TRUE,
		BOX_LAYOUT, BOX_LAYOUT_VERTICAL,
		BOX_GAP, 10,
		NULL);

	make_some_group_members(box3, "Selection Group 2");
	make_group_div(topbox);

	box4 = xv_create(topbox, BOX,
		BOX_LAYOUT, BOX_LAYOUT_VERTICAL,
		BOX_GAP, 10,
		NULL);

	make_some_group_members(box4, "Global Selection");

	xv_set(shell, 
		XV_WIDTH, xv_get(topbox, XV_WIDTH) + 10,
		XV_HEIGHT, xv_get(topbox, XV_HEIGHT) + 10,
		NULL);

	window_fit(frame);

	xv_main_loop(frame);

	xv_destroy( smiley );
	xv_destroy( frown );
}


