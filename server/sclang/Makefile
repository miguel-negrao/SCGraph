.PHONY: clean reallyclean install

clean:
	rm -f *.o

reallyclean: clean
	rm -f *~

install:
	@echo Install ScLang classes...
	install -d $(PREFIX)/share/scgraph/sclang
	install *.sc $(PREFIX)/share/scgraph/sclang
	for n in $(PREFIX)/share/scgraph/sclang/*.sc; do sed -ibak -e "s,PREFIX,$(PREFIX)/,g" "$$n" ;  done


