#
# makefile.linux
# to build libguide; please look at the original Makefile from
# SUN for copyright!
#

SOURCES.c = \
	guide.c \
	gio.c \
	gio_path.c \
	guide_actions.c

SOURCES.h = \
	guide.h \
	gio.h \
	guide_actions.h \
	c_varieties.h

SOURCES = \
	$(SOURCES.h) \
	$(SOURCES.c)

OBJECTS = \
	guide.o \
        gio.o \
        gio_path.o \
        guide_actions.o

LIBRARY = libguide.a

PROJECTDIR = $(GUIDEHOME)
DESTDIR = $(OPENWINHOME)

# Compiler flags.

CPPFLAGS += -I. -I$(OPENWINHOME)/include

# Standard targets.

all:	 $(LIBRARY)

$(OBJECTS): $(SOURCES)
   
$(LIBRARY): $(OBJECTS)
	ar rv $@ $?
	ranlib $@
	rm -f $?

install: all
	install -d $(DESTDIR)/etc
	install -d $(DESTDIR)/lib
	install -m 664 $(LIBRARY) $(DESTDIR)/lib
	install -d $(DESTDIR)/include
	install -m 644 $(SOURCES.h) $(DESTDIR)/include
	(cd $(DESTDIR)/lib ; ranlib -t $(LIBRARY) ; chmod 444 $(LIBRARY))

clean:
	$(RM) $(OBJECTS) core
