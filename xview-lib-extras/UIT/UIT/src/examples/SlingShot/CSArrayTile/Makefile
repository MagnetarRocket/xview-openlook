PROGRAM = array
SOURCES.cc = array.cc
SOURCES.h = 

SSPKGHOME=/usr/openwin

SOURCES = \
	$(SOURCES.h) \
	$(SOURCES.cc)


OBJECTS = \
	$(SOURCES.cc:%.cc=%.o)

# Compiler flags.

CCFLAGS += -g -DNO_XVPS
CPPFLAGS += -I../../../../include/c++ -I$(OPENWINHOME)/include \
	    -I$(SSPKGHOME)/include
LDFLAGS += -L../../../../UIC/src -L$(OPENWINHOME)/lib -L$(SSPKGHOME)/lib -L/usr/X11R6/lib 
LDLIBS += -lUIC -lUICSS -lsspkg -lxview -lolgx -lX11 -lutil
LDLIBS += -lm

CCC = g++

# Standard targets.

all:	 $(TARGETS.other) $(PROGRAM)
objects: $(SOURCES.cc) $(TARGETS.cc) $(TARGETS.h) $(OBJECTS)
sources: $(SOURCES)
targets: $(SOURCES) $(TARGETS)

$(PROGRAM): $(SOURCES.cc) $(SOURCES.h) $(OBJECTS)
	$(LINK.cc) -o $(PROGRAM) $(OBJECTS) $(LDLIBS)

clean:
	$(RM) $(OBJECTS) $(PROGRAM) core
