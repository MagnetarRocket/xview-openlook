#
#    SCCS ID: %Z%%M% %I% %E% SMI
#
#    Copyright (c) 1992 Sun Microsystems, Inc.  All rights reserved.
#    See LEGAL_NOTICE file for terms of the license.
#

INCLUDES = -I$(OPENWINHOME)/include \
	   -I$(GUIDEHOME)/include \
	   -I$(GUIDEHOME)/share/src/libguidexv

chooser_service: chooser_service.o gcc.o ChooserService.types.deps
	cp $(GUIDEHOME)/lib/libguidexv.a .
	chmod +w libguidexv.a
	ar r libguidexv.a gcc.o
	ranlib libguidexv.a
	cc -o chooser_service chooser_service.o \
	      ./libguidexv.a -lxview -lolgx -lX -ltt -lce -lI18N

chooser_service.o: chooser_service.c ChooserService_opnums.h
	cc -c $(CFLAGS) chooser_service.c $(INCLUDES)

gcc.o: gcc.c gcc.h
	cc -c $(CFLAGS) gcc.c $(INCLUDES)

ChooserService.types.deps: ChooserService.types
	tt_type_comp ChooserService.types
	@echo ToolTalk types installed for chooser_service.
	@echo Remember to \"kill -USR2\" the ttsession process so the new
	@echo types will be active.  Also, the directory containing
	@echo "chooser_service" must be in the execution path for the
	@echo process that started the ttsession.

clean:
	/bin/rm -f *.o *.a *.deps chooser_service core
