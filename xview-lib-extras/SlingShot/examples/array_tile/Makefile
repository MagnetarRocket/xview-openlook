 
#       @(#) Makefile 1.14 92/10/15 

SSPKGHOME	= ../..

LIBDIR		= $(OPENWINHOME)/lib
LIBS		= -lxview -lolgx -L/usr/X11R6/lib -lX11 -lutil
SSPKGLIB	= $(SSPKGHOME)/lib/libsspkg.a

.KEEP_STATE:

DBXFLAGS	=  -pipe -O3 -m486
CFLAGS		= $(DBXFLAGS) -I$(SSPKGHOME)/include -I$(OPENWINHOME)/include 
LDFLAGS		= -L$(LIBDIR) $(LIBS) $(SSPKGLIB) -lm

all: array_ex

array_ex: array_ex.o
	cc -o $@ array_ex.o $(LDFLAGS)

clean:
	-sccs clean
	rm -f *.o array_ex

