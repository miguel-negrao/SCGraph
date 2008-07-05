.PHONY: all clean reallyclean doc

DOXYGEN=doxygen

include ../Makefile.include

#doc: 
#	$(DOXYGEN)

clean:
	rm -rf doxygen

reallyclean: clean

install:
	$(INSTALL) -d $(PREFIX)/share/$(TARGET)/doc/examples
	$(INSTALL) examples/*.scd $(PREFIX)/share/$(TARGET)/doc/examples
	
