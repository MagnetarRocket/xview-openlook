#
#    SCCS ID: %Z%%M% %I% %E% SMI
#
#    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
#    See LEGAL_NOTICE file for terms of the license.
#

PROGRAM = xlib
SOURCES.cc = xlib_ui.cc xlib_stubs.cc
SOURCES.h = 

# Derived parameters.

SOURCES = \
	$(SOURCES.h) \
	$(SOURCES.l) \
	$(SOURCES.y) \
	$(SOURCES.cc)

TARGETS.cc = $(SOURCES.cc)

TARGETS.h = \
	$(SOURCES.l:%.l=%.h) \
	$(SOURCES.y:%.y=%.h)

TARGETS = \
	$(TARGETS.h) \
	$(TARGETS.cc)

OBJECTS = \
	$(TARGETS.cc:%.cc=%.o)

# Compiler flags.

CCFLAGS += -DNO_XVPS 
CPPFLAGS += -I../../../../../include/c++ -I$(OPENWINHOME)/include -fno-for-scope
LDFLAGS += -L../../../../../UIC/src -L$(OPENWINHOME)/lib -L/usr/X11R6/lib
LDLIBS += -lUIC -lxview -lolgx -lX11 -lm -lutil

CCC = g++ 

# Standard targets.

all:	 $(TARGETS.other) $(PROGRAM)
objects: $(SOURCES.cc) $(TARGETS.cc) $(TARGETS.h) $(OBJECTS)
sources: $(SOURCES)
targets: $(SOURCES) $(TARGETS)

$(PROGRAM): $(SOURCES.cc) $(TARGETS.cc) $(TARGETS.h) $(OBJECTS)
	$(LINK.cc) -o $(PROGRAM) $(OBJECTS) $(LDLIBS)

# Targets to be used by a certain CASE tool.

stest: $(SOURCES.cc) $(TARGETS.cc) $(TARGETS.h) $(OBJECTS)
	#load $(CCFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH) \
		-o $(PROGRAM) $(TARGETS.cc) $(LDLIBS)
	#link

clean:
	/bin/rm -f $(OBJECTS) $(PROGRAM)
