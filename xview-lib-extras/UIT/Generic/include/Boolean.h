/*H+ Boolean.h
*
*    SCCS ID: @(#)Boolean.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Declares a boolean type and defines the values of TRUE and FALSE.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       09/10/90   Original creation.
*
*H-*/
#ifndef _BOOLEAN_H
#define _BOOLEAN_H

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE 
#define FALSE 0
#endif

typedef short Boolean;

#endif /* _BOOLEAN_H */
