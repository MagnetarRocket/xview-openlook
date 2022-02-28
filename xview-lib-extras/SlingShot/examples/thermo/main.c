/*
 * @(#) main.c 1.3 93/04/06 
 *
 * The thermo is a prototype object.  It is not a full xview object
 * in the sence that the interface does not use xv_get, xv_set, etc.
 * but it is encapulated.  To use it, set values in the Thermo structure
 * and call thermo_set_value.  A thermo is created by thermo_init and
 * destroyed by calling xv_destroy() on therm->box.
 *
 * The thermo is implemented using the SlingShot drawarea object.
 * 
 * The code in this file is largely for demonstrations purposes,
 * if you wish to link in the thermo in your application, all you
 * need to copy are the thermo.c and thermo.h files.
 *
 * brian.warkentine@eng.sun.com
 */

#include <sys/types.h>
#include <time.h>
#include <xview/frame.h>
#include <xview/cms.h>
#include <sspkg/canshell.h>
#include <sspkg/rectobj.h>
#include <sspkg/drawobj.h>
#include "thermo.h"

void            resize();

Frame           frame;
Canvas_shell    shell;
Thermo          a_thermo;
Cms             cms;

#define		WHITE	0
#define		COLOR1	1
#define		COLOR2	2
#define		COLOR3	3
#define		COLOR4	4
#define		COLOR5	5
#define 	COLOR6	6
#define 	BLACK	7

main(argc, argv)
	int             argc;
	char           *argv[];
{

	xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);

	cms = (Cms) xv_create(XV_NULL, CMS,
			      CMS_SIZE, 8,
			      CMS_NAMED_COLORS,
			      "white", "gray90", "snow", "peru",
			      "orange", "firebrick", "gold", "black", NULL,
			      NULL);

	frame = (Frame) xv_create(XV_NULL, FRAME,
				  FRAME_LABEL, argv[0],
				  XV_WIDTH, 100,
				  XV_HEIGHT, 500,
				  NULL);

	shell = (Canvas_shell) xv_create(frame, CANVAS_SHELL,
					 CANVAS_SHELL_BATCH_REPAINT, TRUE,
					 WIN_CMS, cms,
					 NULL);

	a_thermo.outline = BLACK;
	a_thermo.fill = COLOR1;
	a_thermo.fill2 = COLOR5;
	a_thermo.label = COLOR4;
	thermo_init(shell, &a_thermo);
	a_thermo.scaletop = 100;
	a_thermo.scalebot = 0;
	a_thermo.value = 40;
	a_thermo.font = (Xv_font) xv_create(frame, FONT,
			FONT_NAME, "helvetica-22",
			NULL);
	thermo_set_value(&a_thermo);

	xv_set(a_thermo.box,
	       XV_WIDTH, xv_get(shell, XV_WIDTH),
	       XV_HEIGHT, xv_get(shell, XV_HEIGHT),
	       NULL);

	xv_set(shell,
	       CANVAS_RESIZE_PROC, resize,
	       NULL);

        init_timer();

	xv_main_loop(frame);
}

void
resize(shell, width, height)
	Canvas_shell    shell;
	int             width;
	int             height;
{
	xv_set(a_thermo.box,
	       XV_WIDTH, width,
	       XV_HEIGHT, height,
	       NULL);
}


Notify_value
alarmhandler(client, which)
        Notify_client   client;
        int             which;
{
        a_thermo.value = rand() % 100;
        thermo_set_value(&a_thermo);
         
        return NOTIFY_DONE;
}
       

init_timer()
{
	static struct itimerval timer;

	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;

	notify_set_itimer_func(shell,
			       alarmhandler,
			       ITIMER_REAL,
			       &timer,
			       NULL);
}

