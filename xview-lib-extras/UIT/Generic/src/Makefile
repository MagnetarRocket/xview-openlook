#
#      SCCS ID: @(#)Makefile.g++ 1.2 93/02/08 SMI
#
#      Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
#      See LEGAL_NOTICE file for terms of the license.
#
CCFLAGS  = -c -pipe -O3 -m486 -fPIC -fno-for-scope
#CCFLAGS  = -c -g

INCLUDE = ../include/
LIB     = ../lib/
TEST    = ../test/
UITDST = /usr/openwin

OBJECTS = Generic.o GenericHash.o GenericList.o

all: libGeneric.a

libGeneric.a : $(OBJECTS)
	ranlib libGeneric.a

install:
	install libGeneric.a $(UITDST)/lib

xs:
	#load $(CCFLAGS) -I$(INCLUDE) $(OBJECTS:.o=.cc)

Generic.o : Generic.cc $(INCLUDE)Generic.h
	g++ $(CCFLAGS) Generic.cc -I$(INCLUDE)
	ar rv libGeneric.a Generic.o

GenericHash.o : GenericHash.cc $(INCLUDE)GenericHash.h
	g++ $(CCFLAGS) GenericHash.cc -I$(INCLUDE)
	ar rv libGeneric.a GenericHash.o

GenericList.o : GenericList.cc $(INCLUDE)GenericList.h
	g++ $(CCFLAGS) GenericList.cc -I$(INCLUDE)
	ar rv libGeneric.a GenericList.o

clean:
	/bin/rm -f *.o *.a *test

test: hashtest listtest

hashtest: GenericHash.cc
	g++ -fno-for-scope -g -o hashtest GenericHash.cc -DTEST -L. -lGeneric -lm \
	       -I$(INCLUDE) 

listtest: GenericList.cc
	g++ -fno-for-scope -fuse-cxa-atexit -g -o listtest GenericList.cc -DTEST -L. -lGeneric -I$(INCLUDE) 
