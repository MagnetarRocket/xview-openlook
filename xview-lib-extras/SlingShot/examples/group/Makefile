 
#       @(#) Makefile 1.1 93/05/24 

SSPKGHOME	= ../..

LIBDIR		= $(OPENWINHOME)/lib
LIBS		= -lxview -lolgx -L/usr/X11R6/lib -lX11 -lutil
SSPKGLIB	= $(SSPKGHOME)/lib/libsspkg.a


DBXFLAGS	=  -g 
CFLAGS		= $(DBXFLAGS) -I$(SSPKGHOME)/include -I$(OPENWINHOME)/include 
LDFLAGS		= -L$(LIBDIR) $(LIBS) $(SSPKGLIB) -lm

all: group1

group1: group1.o
	cc -o $@ group1.o $(LDFLAGS)

clean:
	-sccs clean
	rm -f *.o group1

