# This Makefile expects TARGET and INSTALL to be set by
# the calling make process.
 
UNAME = $(shell uname)

export PREFIX INSTALL RM MV MOC

#PLUGINS = cube gl_renderer translate rotate stretch binary_op mul_add light color line points triangle rectangle polygon quad blending lighting culling transformation material 

PLUGINS = bordered_rectangle cube gl_renderer translate rotate stretch binary_op mul_add color light line points triangle rectangle polygon quad blending lighting culling transformation material shader_program

#PLUGINS=cube gl_renderer binary_op translate

PLUGIN_SOURCES = $(PLUGINS:%=%.cc)
PLUGIN_HEADERS = $(PLUGINS:%=%.h)
PLUGIN_MOCS = $(PLUGINS:%=%_moc.cc)

PLUGIN_COMPILE_FLAGS =  -I../glew/ -I../oscpack/ -DGLEW_MX

ifeq ($(UNAME),Linux)
  PLUGIN_LIBS = $(PLUGIN_SOURCES:%.cc=%.so)
  LINK_FLAGS = -shared -fpic $(QT_LINK_FLAGS) -export-dynamic -rdynamic -ldl 
endif

ifeq ($(UNAME),Darwin)
  PLUGIN_LIBS = $(PLUGIN_SOURCES:%.cc=%.bundle)
  LINK_FLAGS = -bundle -fPIC -undefined dynamic_lookup 
#  PLUGIN_COMPILE_FLAGS += 
endif


ifeq ($(HAVE_JACK),1)
  ADDITIONAL_COMPILE_FLAGS += `pkg-config jack fftw3f --cflags` -DHAVE_JACK
  PLUGINS += freq_amp
endif

PLUGIN_DIRS = 

TARGET_DIR = $(PREFIX)/lib/scgraph/plugins

.PHONY: all 
all: $(PLUGIN_LIBS) plugin_dirs


ifeq ($(UNAME),Linux)
$(PLUGIN_LIBS): %.so: %.cc %.cc %_moc.cc ../unit.h ../graphics_visitor.h ../graphics.h ../graphics_bus.h ../texture_pool.h ../shader_pool.h ../matrix.h 
	@echo
	@echo --- Building plugin: $< \=\> $@
	$(CXX) $(PLUGIN_COMPILE_FLAGS) $(LINK_FLAGS) -o $@ $*.cc $*_moc.cc $(QT_COMPILE_FLAGS) $(QT_LINK_FLAGS) $(ADDITIONAL_COMPILE_FLAGS) $(ADDITIONAL_LINK_FLAGS) -pthread
endif

ifeq ($(UNAME),Darwin)
$(PLUGIN_LIBS): %.bundle: %.cc %.cc %_moc.cc ../unit.h ../graphics_visitor.h ../graphics.h ../graphics_bus.h ../matrix.h 
	@echo
	@echo --- Building plugin: $< \=\> $@
	$(CXX) $(PLUGIN_COMPILE_FLAGS) $(LINK_FLAGS) -o $@ $*.cc $*_moc.cc $(QT_COMPILE_FLAGS) $(QT_LINK_FLAGS) $(ADDITIONAL_COMPILE_FLAGS) $(ADDITIONAL_LINK_FLAGS) 
endif


$(PLUGIN_MOCS): %_moc.cc: %.h
	@echo
	@echo --- Creating MOC output: $< \=\> $@
	$(MOC) $(MOC_OPTIONS) -o $@ $<

.PHONY: plugin_dirs
plugin_dirs: 
	@echo --- Processing plugin directories: $(PLUGIN_DIRS)
	for n in $(PLUGIN_DIRS); do make -C $$n; done

.PHONY: install
install: all
	@echo
	@echo --- Installing plugins to $(TARGET_DIR)...
	$(INSTALL) -d $(TARGET_DIR)
	for n in $(PLUGIN_LIBS); do $(INSTALL) $$n $(TARGET_DIR); done
	for n in $(PLUGIN_DIRS); do make -C $$n install; done

.PHONY: clean
clean:
	@echo
	@echo --- Making clean in plugins
	$(RM) -f $(PLUGIN_LIBS) $(PLUGIN_MOCS)
	for n in $(PLUGIN_DIRS); do make -C $$n clean; done

.PHONY: reallyclean
reallyclean: clean
	@echo
	@echo --- Making reallyclean in plugins
	$(RM) -f *.so *~
