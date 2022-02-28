
#	@(#) Makefile 1.21 93/10/25 

SSPKGHOME = 	.
INSTALL_DIR =	/usr/openwin
TARFILE =	/tmp/sspkgs.tar

SRCDIR =	$(SSPKGHOME)/src
LIBDIR =	$(SSPKGHOME)/lib
INCLUDEDIR = 	$(SSPKGHOME)/include/sspkg

.PRECIOUS: $(SRCDIR) $(INCLUDEDIR) examples

all: subdirs

subdirs: $(INCLUDEDIR) $(SRCDIR) examples

$(INCLUDEDIR): FORCE
	cd $@; make

$(SRCDIR): FORCE Make.config
	cd $@; make


examples: FORCE
	cd $@; make

#
# Make.config has things that are needed to build on various platforms.
# Here /vmunix existance is tested to determine if we're running svr4 
# or sunos4.x.  This is a hack, but is works.
#
Make.config: config.svr4 config.sunos4x
	rm -f Make.config
	@ if [ -s /vmunix ]; then  \
		echo cp config.sunos4x Make.config ; \
		cp config.sunos4x Make.config ; \
	else  \
		echo cp config.svr4 Make.config ; \
		cp config.svr4 Make.config ; \
	fi


FORCE:

clean:
	-sccs clean
	(cd $(INCLUDEDIR); make clean)
	(cd $(SRCDIR); make clean)
	(cd examples; make clean)
	rm -f Make.config

install: $(INSTALL_DIR) FORCE
	(cd $(INCLUDEDIR); make install INSTALL_DIR=$(INSTALL_DIR))
	(cp $(LIBDIR)/* $(INSTALL_DIR)/lib)
	-ranlib $(INSTALL_DIR)/lib/libsspkg.a
	-ranlib $(INSTALL_DIR)/lib/libsspkg.sa.*
	(cd $(INSTALL_DIR)/lib; rm -f libsspkg.so; ln -s libsspkg.so.* libsspkg.so)

$(INSTALL_DIR): FORCE
	-mkdir -p $(INSTALL_DIR)
	-mkdir -p $(INSTALL_DIR)/include/sspkg
	-mkdir $(INSTALL_DIR)/lib

#tarfile: FORCE examples/README examples/things

tarfile: FORCE LEGAL_NOTICE README config.svr4 config.sunos4x
	$(RM) $(TARFILE)
	(find . -name SCCS -print > /tmp/excludelist)
	(find . -name lex.yy.c -print >> /tmp/excludelist)
	(find . -name libsspkg.a -print >> /tmp/excludelist)
	(find . -name .make.state -print >> /tmp/excludelist)
	(find . -name .nse_depinfo -print >> /tmp/excludelist)
	(find . -type f -perm -700 -print >> /tmp/excludelist)
	(echo ./Make.config >> /tmp/excludelist)
	(echo ./test >> /tmp/excludelist)
	(echo ./book >> /tmp/excludelist)
	(echo ./lib >> /tmp/excludelist)
	(cd ..; tar zcvf $(TARFILE) -X /tmp/excludelist ./SlingShot)
	$(RM) /tmp/excludelist

#	(cd `dirname $(TARFILE)`; tar xf $(TARFILE); makekit `find sspkg2.0 -print`)
# note: copy doc files by hand.

