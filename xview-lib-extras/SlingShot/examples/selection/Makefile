 
#       @(#) Makefile 1.1 92/06/22 

SSPKGHOME	= ../..

LIBDIR		= $(OPENWINHOME)/lib
LIBS		= -lxview -lolgx -L/usr/X11R6/lib -lX11 -lutil
SSPKGLIB	= $(SSPKGHOME)/lib/libsspkg.a

.KEEP_STATE:

DBXFLAGS	=  -g 
CFLAGS		= $(DBXFLAGS) -I$(SSPKGHOME)/include -I$(OPENWINHOME)/include 
LDFLAGS		= -L$(LIBDIR) $(LIBS) $(SSPKGLIB) -lm

all: select

select: select.o
	cc -o $@ select.o $(LDFLAGS)

clean:
	-sccs clean
	rm -f *.o select

