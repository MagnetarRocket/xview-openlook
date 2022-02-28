/*H+ UICPrivateTypes.h
*
*    SCCS ID: @(#)UICPrivateTypes.h 1.2 93/04/09 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines types and constants used internally by the UIC.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/
#ifndef _UIC_PRIVATE_TYPES_H
#define _UIC_PRIVATE_TYPES_H

// Type Definitions
typedef struct {
        XID    xid;
        int    x;
        int    y;
        Window window;
        Atom   property;
} DragMessage;

// Constants
const int UIC_OBJECT_KEY = 100;
const int UIC_FRAME_KEY  = 101;
const int UIC_NOTICE_KEY = 102;
const int UIC_CHILD_KEY  = 103;

#endif /* _UIC_PRIVATE_TYPES_H */
