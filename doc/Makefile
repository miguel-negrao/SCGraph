.PHONY: all clean reallyclean doc

DOXYGEN=doxygen
INSTALL=install
TARGET=scgraph

include ../Makefile.include

#doc: 
#	$(DOXYGEN)

clean:
	rm -rf doxygen

reallyclean: clean

install:
	$(INSTALL) -d $(PREFIX)/share/$(TARGET)/doc/help
	$(INSTALL) help/*.html $(PREFIX)/share/$(TARGET)/doc/help
	$(INSTALL) -d $(PREFIX)/share/$(TARGET)/doc/help/attachments/scgraph
	$(INSTALL) help/attachments/scgraph/*.png $(PREFIX)/share/$(TARGET)/doc/help/attachments/scgraph
	$(INSTALL) -d $(PREFIX)/share/$(TARGET)/doc/examples
	$(INSTALL) examples/*.scd $(PREFIX)/share/$(TARGET)/doc/examples
	
