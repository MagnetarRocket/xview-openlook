/*H+ UICTypes.h
*
*    SCCS ID: @(#)UICTypes.h 1.5 93/02/08 SMI 
*
*    Copyright (c) 1990-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Provides include files and type definitions that are used by the UIC.
*
*    AUTHOR: Joe Warzecha and Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       05/10/91   Preparation for code review.
*    MGS       09/11/91   Put "private" definitions into UICPrivateTypes.h.
*    MGS       04/15/92   Add new UICAttribute values.
*
*H-*/
#ifndef _UIC_TYPES_H
#define _UIC_TYPES_H

// Includes

// System Includes
#include <stdio.h>
#include <stdlib.h>

// So we don't include /usr/include/varargs.h
#define _sys_varargs_h
// So we don't include /usr/CC/incl/varargs.h
#define VARARGSH

#include <stdarg.h>

#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/param.h>

// Generic Includes
#include "Boolean.h"

// XView Includes
#include <xview/base.h>
#include <xview/cms.h>
#include <xview/xview.h>
#include <xview/panel.h>
#include <xview/textsw.h>
#include <xview/sel_pkg.h>
#include <xview/dragdrop.h>
#include <xview/cursor.h>
#include <xview/defaults.h>

// X11 Library Include
#include <X11/Xlib.h>

// Internationalization Includes
#ifdef SVR4
#include <locale.h>
#endif

// External Function Declarations
#ifndef SVR4
extern "C" {
char *dgettext (const char *, const char *);
char *bindtextdomain (const char *, const char *);
}
#endif

// Macro Definitions

// UIC Version 3 for public interface compatibility purposes
#define UICV3			"030000"
#define UIC_VERSION_STRING	UICV3
#define UIC_VERSION		030000

// Definition of ScrollChoice for UIT V1 compatibility
#define ScrollChoice ListChoice

// Type Definitions
class UIObject;

typedef Xv_opaque       UICGeneric;

typedef void            (*UICHandlerFunction) (UIObject *);
typedef void            (*UICInputFunction) (UIObject *, int);
typedef void            (*UICTargetFunction) (UIObject *, UIObject *);

typedef Xv_singlecolor  Color;
typedef Destroy_status  DestroyStatus;
typedef Panel_setting   DisplaySetting;
typedef Notify_value    EventResult;
typedef XID             PixelMap;
typedef Textsw_status   TextDisplayStatus;

enum    RowAlignment    { TOP_EDGES, BOTTOM_EDGES, HORIZONTAL_CENTERS };
enum    ColumnAlignment { LEFT_EDGES, RIGHT_EDGES, VERTICAL_CENTERS, LABELS };
enum  	ColorMapType	{ STATIC, DYNAMIC };
enum    CompassPoint    { NORTHWEST, NORTH, NORTHEAST,
                          SOUTHWEST, SOUTH, SOUTHEAST,
			  WEST, CENTER, EAST };
enum	Dimension	{ HEIGHT, WIDTH };
enum	DragStatus	{ DRAG_NO_DRAG,
			  DRAG_START,
			  DRAG_CONVERT,
			  DRAG_DELETE,
			  DRAG_END };
enum	DrawingModel	{ XVIEW, X11, POSTSCRIPT };
enum    Justification   { JUSTIFY_LEFT, JUSTIFY_CENTER, JUSTIFY_RIGHT };
enum    Layout          { NO_LAYOUT, ROW_LAYOUT, COLUMN_LAYOUT,
			  ROW_AND_COLUMN_LAYOUT };
enum	MoveStatus	{ MOVE_NO_MOVE,
			  MOVE_START,
			  MOVE_IN_PROGRESS,
			  MOVE_END };
enum 	Orientation	{ HORIZONTAL, VERTICAL };
enum 	Position	{ LEFT, ABOVE };
enum	SelectionStatus	{ SELECT, DESELECT, VALIDATE, DELETE, UNKNOWN };

enum	UICAttribute	{ UIC_NO_ATTRIBUTE,
			  UIC_OPEN_PS_CONNECTION,
			  UIC_TEXT_DOMAIN,
			  UIC_LOCALE_DIR,
			  UIC_X_ERROR_PROC };

// Constants
const 	int		UIC_NOT_SET = -1;

#endif /* _UIC_TYPES_H */
