 
#       @(#) Makefile 1.21 93/11/02 

SSPKGHOME	= ..

LIBDIR		= $(OPENWINHOME)/lib
LIBS		= -lxview -lolgx -lX11
SSPKGLIB	= $(SSPKGHOME)/lib/libsspkg.a


DBXFLAGS	=  -g 
CFLAGS		= $(DBXFLAGS) -I$(SSPKGHOME)/include -I$(OPENWINHOME)/include 
LDFLAGS		= -L$(LIBDIR) $(LIBS) $(SSPKGLIB) -lm

all: icons array_tile clockobj color color2 \
	dnd drawarea drawimage drawline drawtext grip \
	group icons misc selection tacho thermo tree 

array_tile: FORCE
	cd $@; make

clockobj: FORCE
	cd $@; make

dnd: FORCE
	cd $@; make

dnd2: FORCE
	cd $@; make

drawarea: FORCE
	cd $@; make

drawline: FORCE
	cd $@; make

group: FORCE
	cd $@; make

grip: FORCE
	cd $@; make

icons: FORCE
	cd $@; make

misc: FORCE
	cd $@; make

selection: FORCE
	cd $@; make

tree: FORCE
	cd $@; make

drawtext: FORCE
	cd $@; make

drawimage: FORCE
	cd $@; make

tacho: FORCE
	cd $@; make

color: FORCE
	cd $@; make

color2: FORCE
	cd $@; make

thermo: FORCE
	cd $@; make

clean:
	-(cd array_tile; make clean; rm -f .make.state)
	-(cd color; make clean; rm -f .make.state)
	-(cd color2; make clean; rm -f .make.state)
	-(cd clockobj; make clean; rm -f .make.state)
	-(cd dnd; make clean; rm -f .make.state)
	-(cd dnd2; make clean; rm -f .make.state)
	-(cd drawarea; make clean; rm -f .make.state)
	-(cd drawimage; make clean; rm -f .make.state)
	-(cd drawline; make clean; rm -f .make.state)
	-(cd drawtext; make clean; rm -f .make.state)
	-(cd grip; make clean; rm -f .make.state)
	-(cd group; make clean; rm -f .make.state)
	-(cd icons; make clean; rm -f .make.state)
	-(cd misc; make clean; rm -f .make.state)
	-(cd selection; make clean; rm -f .make.state)
	-(cd tree; make clean; rm -f .make.state)
	-(cd tacho; make clean; rm -f .make.state)
	-(cd thermo; make clean; rm -f .make.state)

FORCE:

