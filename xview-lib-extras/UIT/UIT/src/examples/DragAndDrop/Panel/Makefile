#
#    SCCS ID: %Z%%M% %I% %E% SMI
#
#    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
#    See LEGAL_NOTICE file for terms of the license.
#

CPPFLAGS += -I../../../../include/c++ -I$(OPENWINHOME)/include 
LDFLAGS += -L../../../../UIC/src -L$(OPENWINHOME)/lib -L/usr/X11R6/lib
LDLIBS += -lUIC -lxview -lolgx -lX11 -lm -lutil 

all: drag_source drop_target

drag_source: drag_source.o
	cc -o drag_source drag_source.o $(LDFLAGS) $(LDLIBS)

drop_target: drop_target.o
	cc -o drop_target drop_target.o $(LDFLAGS) $(LDLIBS)

delete:
	/bin/rm -f *.o drag_source drop_target
