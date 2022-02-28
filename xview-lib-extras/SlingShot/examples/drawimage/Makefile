 
#       @(#) Makefile 1.3 92/10/22 

SSPKGHOME	= ../..

LIBDIR		= $(OPENWINHOME)/lib
LIBS		= -lxview -lolgx -L/usr/X11R6/lib -lX11 -lutil
SSPKGLIB	= $(SSPKGHOME)/lib/libsspkg.a


DBXFLAGS	=  -g 
CFLAGS		= $(DBXFLAGS) -I$(SSPKGHOME)/include -I$(OPENWINHOME)/include 
LDFLAGS		= -L$(LIBDIR) $(LIBS) $(SSPKGLIB) -lm

all: drawimage

drawimage: drawimage.o
	cc -o $@ drawimage.o $(LDFLAGS)

clean:
	-sccs clean
	rm -f *.o drawimage
