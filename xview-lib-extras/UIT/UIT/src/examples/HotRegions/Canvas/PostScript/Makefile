#
#    SCCS ID: %Z%%M% %I% %E% SMI
#
#    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
#    See LEGAL_NOTICE file for terms of the license.
#

PROGRAM = ps
SOURCES.cc = ps_ui.cc ps_stubs.cc
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

CPPFLAGS += -I$(UITHOME)/include/c++ -I$(OPENWINHOME)/include
LDFLAGS += -L$(UITHOME)/lib -L$(OPENWINHOME)/lib
LDLIBS += -lUIC -lxvps -lxview -lolgx -lX -lcps -lm
LDLIBS += -lwire

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

delete:
	/bin/rm -f $(OBJECTS) $(PROGRAM)
