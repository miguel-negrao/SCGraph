# see the respective subdir Makefiles for adjustments
  
.PHONY: all doc server clean reallyclean install uninstall

all: Makefile.include server

Makefile.include: Makefile.include.template
	@echo --- Creating a copy of the Makefile.include.template. Please edit Makefile.include
	@if test -e Makefile.include;  then \
		echo Oh it already exists...; \
		touch Makefile.include ; \
	else \
		cp Makefile.include.template Makefile.include; \
	fi
	@echo
	@echo --- Exiting now to let you edit that file.
	@echo     If no changes are needed just rerun make
	exit 42
	

server:
	make -C server

doc:
	make -C doc
	make -C server doc

install:
	make -C server install
	make -C doc install

uninstall:
	make -C server uninstall

clean:
	make -C server clean

reallyclean: clean
	make -C server reallyclean
	make -C doc reallyclean
	rm -f *~ core*
	rm -f svn-commit*
