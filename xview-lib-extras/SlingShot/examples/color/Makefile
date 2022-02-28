 
#        @(#) Makefile 1.3 93/10/25 

SSPKGHOME	= ../..

LIBDIR		= $(OPENWINHOME)/lib
LIBS		= -lxview -lolgx -L/usr/X11R6/lib -lX11 -lutil
SSPKGLIB	= $(SSPKGHOME)/lib/libsspkg.a

.KEEP_STATE:

DBXFLAGS	=  -g 
CFLAGS		= $(DBXFLAGS) -I$(SSPKGHOME)/include -I$(OPENWINHOME)/include 
LDFLAGS		= -L$(LIBDIR) $(LIBS) $(SSPKGLIB) -lm

all: choose dndchoose

choose: choose.o
	cc -o $@ choose.o $(LDFLAGS)

dndchoose: dndchoose.o
	cc -o $@ dndchoose.o $(LDFLAGS)

clean:
	-sccs clean
	rm -f *.o choose dndchoose

