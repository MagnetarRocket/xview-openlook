all: libguide libguidexv

libguide:
	cd share/src/libguide; make -f makefile.linux

libguidexv:
	cd share/src/libguidexv; make -f makefile.linux

demo:
	cd demo/xview/tree; make -f makefile.linux
install:
	cd share/src/libguide; make -f makefile.linux install
	cd share/src/libguidexv; make -f makefile.linux install

clean:
	cd share/src/libguide; make -f makefile.linux clean
	cd share/src/libguidexv; make -f makefile.linux clean
