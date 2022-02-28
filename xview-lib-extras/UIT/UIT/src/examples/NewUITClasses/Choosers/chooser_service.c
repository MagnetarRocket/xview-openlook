/*
*    SCCS ID: %Z%%M% %I% %E% SMI
*
*    Copyright (c) 1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <xview/xview.h>
#include <xview/frame.h>
#include <xview/notify.h>
#include <desktop/tt_c.h>

#include "ChooserService_opnums.h"
#include "gcc.h"
#include "gfm.h"
#include "gcc_ui.h"

static int  chooser_service_message_handler ();
static void chooser_service_window_handler ();
static void chooser_service_color_chooser ();
static void chooser_service_file_chooser ();
static void chooser_service_done_proc ();

static void color_chooser_callback ();
static void file_chooser_callback ();

Attr_attribute INSTANCE = 1000;
#define        TT_MSG_KEY 2000

static void add_xview_object ();
static void remove_xview_object ();
static void flush_xview_objects ();

static void      put_message_received ();
static int       get_message_received ();
static Xv_opaque get_xview_object ();

void main (argc, argv)
int    argc;
char **argv;
{
  int            mark;
  Tt_message     msg;
  struct pollfd  poll_fd;
  char          *procid;

  xv_init (XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);

  procid = tt_open ();
 
  if (tt_pointer_error (procid) != TT_OK) {
    fprintf (stderr, "%s: Can't initialize ToolTalk\n", argv[0]);
    exit (1);
  }
 
  if (tt_ptype_declare ("ChooserService") != TT_OK) {
    fprintf (stderr, "The ptype ChooserService is not installed.\n");
    exit (1);
  }

  if (tt_session_join (tt_X_session (getenv ("DISPLAY"))) != TT_OK) {
    fprintf (stderr, "DISPLAY env var is not pointing to a valid X server.\n");
    exit (1);
  }

  poll_fd.fd = tt_fd ();
  poll_fd.events = POLLIN;

  for (;;) {
    if (poll (&poll_fd, 1, -1) > 0) {
      mark = tt_mark ();
      msg = tt_message_receive ();
      if (msg == NULL) continue;

      if (!chooser_service_message_handler (msg))
        chooser_service_window_handler ();
      tt_release (mark);
    }
  }

  (void) tt_close ();
  exit (0);
}

static int chooser_service_message_handler (msg)
Tt_message msg;
{
  int opnum;
 
  if (tt_pointer_error (msg) == TT_ERR_NOMP) {
    fprintf (stderr, "ToolTalk server down.\n");
    exit (0);
  }

  if (tt_message_status (msg) == TT_WRN_START_MESSAGE) {
    tt_message_fail (msg);
    tt_message_destroy (msg);
    return 1;
  }

  switch (tt_message_opnum (msg)) {
  case COLOR_CHOOSER:
    chooser_service_color_chooser (msg);
    break;
  case FILE_CHOOSER:
    chooser_service_file_chooser (msg);
    break;
  default:
    tt_message_reject (msg);
    tt_message_destroy (msg);
    return 1;
  }

  return 0;
}

static void chooser_service_window_handler ()
{
  int        messages_pending = 1;
  Tt_message msg;
  Tt_message new_msg;
  Xv_opaque  xview_object;

  while (messages_pending) {
    put_message_received (FALSE, XV_NULL);

    while (!get_message_received ()) {
      notify_dispatch ();
      flush_xview_objects ();

      if (new_msg = tt_message_receive ()) {
        if (!chooser_service_message_handler (new_msg))
	  messages_pending++;
      }
    }
  
    if (xview_object = get_xview_object ()) {
      msg = (Tt_message) xv_get (xview_object, XV_KEY_DATA, TT_MSG_KEY);

      notify_dispatch ();
      flush_xview_objects ();
      remove_xview_object (xview_object);

      tt_message_reply (msg);
      tt_message_destroy (msg);
      messages_pending--;
    }
  } 
}

static void chooser_service_color_chooser (msg)
Tt_message msg;
{
  char  *title;
  char  *left_footer;
  char  *right_footer;
  char  *current_color;

  gcc_gccwin_objects *gcc;

  title = tt_message_arg_val (msg, 0);
  left_footer = tt_message_arg_val (msg, 1);
  right_footer = tt_message_arg_val (msg, 2);
  current_color = tt_message_arg_val (msg, 3);

  if (title && strlen (title)) 
    gcc = gcc_initialize (NULL, title); 
  else
    gcc = gcc_initialize (NULL, "Color Chooser");
  add_xview_object (gcc->gccwin);

  xv_set (gcc->gccwin, FRAME_DONE_PROC, chooser_service_done_proc, NULL);
  xv_set (gcc->gccwin, XV_KEY_DATA, TT_MSG_KEY, msg, NULL);
  tt_message_user_set (msg, TT_MSG_KEY, gcc->gccwin);
 
  xv_set (gcc->gccwin, FRAME_CMD_PUSHPIN_IN, TRUE, NULL);
  gcc_activate (gcc,
		left_footer,
		right_footer,
		color_chooser_callback,
		msg,
		current_color);
}

static void color_chooser_callback (color, msg)
char       *color;
Tt_message  msg;
{
  Xv_opaque xview_object = (Xv_opaque) tt_message_user (msg, TT_MSG_KEY);
  tt_message_arg_val_set (msg, 4, color);
  put_message_received (TRUE, xview_object);
}

static void chooser_service_file_chooser (msg)
Tt_message msg;
{
  char *title;
  char *left_footer;
  char *right_footer;
  char *button_label;
  char *current_directory;
  char *file_filter_pattern;
  int   show_dot_files;
 
  gfm_popup_objects *gfm;

  title = tt_message_arg_val (msg, 0);
  left_footer = tt_message_arg_val (msg, 1);
  right_footer = tt_message_arg_val (msg, 2);
  button_label = tt_message_arg_val (msg, 3);
  current_directory = tt_message_arg_val (msg, 4);
  file_filter_pattern = tt_message_arg_val (msg, 5);
  (void) tt_message_arg_ival (msg, 6, &show_dot_files);
 
  if (title && strlen (title)) 
    gfm = gfm_initialize (NULL, NULL, title); 
  else
    gfm = gfm_initialize (NULL, NULL, "File Chooser"); 
  add_xview_object (gfm->popup);

  xv_set (gfm->popup, FRAME_DONE_PROC, chooser_service_done_proc, NULL);
  xv_set (gfm->popup, XV_KEY_DATA, TT_MSG_KEY, msg, NULL);
  tt_message_user_set (msg, TT_MSG_KEY, gfm->popup);
 
  xv_set (gfm->popup, FRAME_LEFT_FOOTER, left_footer, NULL); 
  xv_set (gfm->popup, FRAME_RIGHT_FOOTER, right_footer, NULL); 

  if (button_label && strlen (button_label))
    gfm_set_action (gfm, button_label);
  else
    gfm_set_action (gfm, "Load");

  gfm_show_dotfiles (gfm, show_dot_files);

  xv_set (gfm->popup, FRAME_CMD_PUSHPIN_IN, TRUE, NULL);
  gfm_activate (gfm,
		current_directory,
	        file_filter_pattern,
		NULL,
		file_chooser_callback,
		NULL,
		GFM_DEFAULT);
}

static void file_chooser_callback (gfm, dir, file)
gfm_popup_objects *gfm;
char              *dir;
char              *file;
{
  Tt_message msg = (Tt_message) xv_get (gfm->popup, XV_KEY_DATA, TT_MSG_KEY);

  tt_message_arg_val_set (msg, 7, dir);
  tt_message_arg_val_set (msg, 8, file);

  put_message_received (TRUE, gfm->popup);
}

static void chooser_service_done_proc (frame)
Frame frame;
{
  put_message_received (TRUE, frame);
  remove_xview_object (frame);
}

static Xv_opaque *xview_object_list_s;
static int        num_xview_objects_s;

static void add_xview_object (object)
Xv_opaque object;
{
  int        i;
  Xv_opaque *temp_list;

  num_xview_objects_s++;
  temp_list = (Xv_opaque *) calloc (num_xview_objects_s, sizeof (Xv_opaque));

  for (i=0; i < num_xview_objects_s-1; i++)
    temp_list [i] = xview_object_list_s [i];
  temp_list [num_xview_objects_s-1] = object;

  free (xview_object_list_s);
  xview_object_list_s = temp_list;
}

static void remove_xview_object (object)
Xv_opaque object; 
{ 
  int        i, j;
  int        object_found = 0;
  Xv_opaque *temp_list; 
 
  for (i=0, j=0; i < num_xview_objects_s+1; i++) {
    if (xview_object_list_s [i] == object) {
      object_found = 1;
      break;
    }
  }
 
  if (object_found) {
    num_xview_objects_s--; 
    temp_list = (Xv_opaque *) calloc (num_xview_objects_s, sizeof (Xv_opaque)); 
 
    for (i=0, j=0; i < num_xview_objects_s+1; i++) {
      if (xview_object_list_s [i] != object)
        temp_list [j++] = xview_object_list_s [i]; 
    }
 
    free (xview_object_list_s); 
    xview_object_list_s = temp_list; 

    xv_destroy_safe (object);
    notify_dispatch ();
    flush_xview_objects ();
  }
} 

static void flush_xview_objects ()
{
  int i;

  for (i=0; i < num_xview_objects_s; i++)
    XFlush (xv_get (xview_object_list_s [i], XV_DISPLAY));
}
 
static int       message_received_s;
static Xv_opaque xview_object_s;
 
static void put_message_received (mr, object)
int       mr;
Xv_opaque object;
{
  message_received_s = mr;
  xview_object_s = object;
}
 
static int get_message_received ()
{
  return message_received_s;
}

static Xv_opaque get_xview_object ()
{
  return xview_object_s;
}
