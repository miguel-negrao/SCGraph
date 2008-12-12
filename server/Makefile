# COPYRIGHT 2006 Florian Paul Schmidt
 
UNAME = $(shell uname)

# You can override all variables up to the line --- END --- by creating
# a file called Makefile.include in the ../ directory and overwriting
# the values of the variables there. This way if you ever check into SVN
# the main Makefile (this) stays the same.

# The prefix where this program and parts are installed
PREFIX = /usr/local

ADDITIONAL_COMPILE_FLAGS = 
ADDITIONAL_LINK_FLAGS = 

QTDIR ?= /usr/share/qt4

SC3_PREFIX = /usr/
SC3_INCLUDE_PATH = $(SC3_PREFIX)/include/SuperCollider/
SC3_LIBRARY_PATH = $(SC3_PREFIX)/lib

SC3_PLUGIN_PATH = $(SC3_PREFIX)/lib/SuperCollider/plugins

OSCPACK_COMPILE_FLAGS = 

ifeq ($(UNAME),Linux)
  QT_LINK_FLAGS = -L$(QTDIR)/lib -lQtGui -lQtCore -lQtOpenGL -lQtUiTool
  OSCPACK_COMPILE_FLAGS += -DOSC_HOST_LITTLE_ENDIAN
endif

ifeq ($(UNAME),Darwin)
  QT_LINK_FLAGS = -F$(QTDIR)/lib -weak_framework QtGui -weak_framework QtCore -weak_framework QtOpenGL -weak_framework OpenGL
  OSCPACK_COMPILE_FLAGS += -DOSC_HOST_LITTLE_ENDIAN
endif

QT_COMPILE_FLAGS =  -I$(QTDIR)/include -DQT_THREAD_SUPPORT

HAVE_JACK = 0

CP      ?= cp
MV      ?= mv
RM      ?= rm
INSTALL ?= install
DOXYGEN ?= doxygen
MOC     ?= $(QTDIR)/bin/moc

TARGET = scgraph

# --- END ----------------------------------------------------------------------------------

-include ../Makefile.include

export PREFIX INSTALL CP MV RM QT_COMPILE_FLAGS QT_LINK_FLAGS MOC SC3_HEADERS ADDITIONAL_COMPILE_FLAGS ADDITIONAL_LINK_FLAGS HAVE_JACK TARGET 


.PHONY: all
all:  $(TARGET) plugins test_tree 


COMPILE_FLAGS = -pedantic -Wall -Wno-long-long -ansi -Werror $(ADDITIONAL_COMPILE_FLAGS) -DPREFIX=\"$(PREFIX)\" -DSC3_INCLUDE_PATH=\"$(SC3_INCLUDE_PATH)\" $(QT_COMPILE_FLAGS) -I$(SC3_INCLUDE_PATH)/plugin_interface  -I$(SC3_INCLUDE_PATH)/server -I$(SC3_INCLUDE_PATH)/common `pkg-config sndfile GraphicsMagick++ --cflags`  -DSC3_PLUGIN_PATH=\"$(SC3_PLUGIN_PATH)\"  -I./glew/ -DGLEW_MX -I./oscpack/

ifeq ($(HAVE_JACK),1)
  COMPILE_FLAGS += `pkg-config jack fftw3f --cflags` -DHAVE_JACK
endif


ifeq ($(UNAME),Linux)
  COMPILE_FLAGS += -pthread 
endif

ifeq ($(UNAME),Darwin)
  COMPILE_FLAGS += -DSC_DARWIN
endif

LINK_FLAGS = -lpthread $(ADDITIONAL_LINK_FLAGS) $(QT_LINK_FLAGS) -lm `pkg-config sndfile GraphicsMagick++ --cflags --libs` 

ifeq ($(HAVE_JACK),1)
  LINK_FLAGS += `pkg-config jack --libs fftw3f` -DHAVE_JACK
endif


ifeq ($(UNAME),Linux)
  LINK_FLAGS += -lrt -lm -Wl,--export-dynamic -L$(SC3_LIBRARY_PATH) -lscsynth -lGLU -rdynamic -ldl
endif

ifeq ($(UNAME),Darwin)
  LINK_FLAGS += $(SC3_LIBRARY_PATH)/libscsynth.a  -Wl,-bind_at_load -lstdc++  
endif

STUFF     = main cow_ptr transformation_command transformation_command_visitor graphics_visitor obj_loader loader_3d buffer_pool matrix node_tree synth node group options scgraph synthdef control_loop graphic_loop synthdef_file util synthdef_pool plugin_pool plugin unit  osc_handler graphics tree in_out control graphics_bus material face color_rgba vector_3d hvector_3d vector_2d sc_unit_wrapper sc__unit texture_pool ui_pool

GLEWSTUFF = glew/glew

OSCPACKSOURCES = $(wildcard oscpack/osc/*.cpp) $(wildcard oscpack/ip/*.cpp) $(wildcard oscpack/ip/posix/*.cpp)
OSCPACKSTUFF = $(OSCPACKSOURCES:%.cpp=%) 

QTSTUFF   = graphic_loop osc_handler texture_pool 

STUFF += shader_pool
QTSTUFF += shader_pool

ifeq ($(HAVE_JACK),1)
  STUFF += jack_client
endif


OBJECTS        = $(STUFF:%=%.o)
GLEWOBJECTS    = $(GLEWSTUFF:%=%.o)
OSCPACKOBJECTS = $(OSCPACKSTUFF:%=%.o)
QTOBJECTS      = $(QTSTUFF:%=%_moc.o)
SOURCES        = $(STUFF:%=%.cc)
QTSOURCES      = $(QTSTUFF:%=%_moc.cc)
HEADERS        = $(STUFF:%=%.h)
DEPS           = $(STUFF:%=deps/%.dep)

# only create deps file if really making
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),reallyclean)
ifneq ($(MAKECMDGOALS),doc)
  -include $(DEPS)
endif
endif
endif

test_tree: tree.h test_tree.cc
	 g++ test_tree.cc tree.cc -Wall -pedantic -o test_tree -g $(ADDITIONAL_COMPILE_FLAGS)

test_obj_loader: obj_loader.h test_obj_loader.cc obj_loader.cc graphics.cc matrix.cc graphics.h matrix.h
	 g++ test_obj_loader.cc obj_loader.cc loader_3d.cc graphics.cc matrix.cc vector_3d.cc face.cc color_rgba.cc material.cc -Wall -pedantic -o test_obj_loader -g $(ADDITIONAL_COMPILE_FLAGS)

.PHONY: plugins
plugins:
	make -C plugins

$(TARGET): $(OBJECTS) $(QTOBJECTS) $(GLEWOBJECTS) $(OSCPACKOBJECTS)
	@echo --- Building target: $(TARGET)
	$(CXX) -o $(TARGET) $(OBJECTS) $(QTOBJECTS) $(GLEWOBJECTS) $(OSCPACKOBJECTS) $(LINK_FLAGS)

$(QTOBJECTS): %.o: %.cc 
	@echo --- Building Qt object: $< \=\> $@
	$(CXX) -c $< $(COMPILE_FLAGS)
	@echo

$(GLEWOBJECTS): %.o: %.c $(GLEWSOURCES)
	@echo Building GLEW support
	$(CC) -c $< -o $@ -I ./glew/  -DGLEW_MX -O2

$(OSCPACKOBJECTS): %.o: %.cpp $(OSCPACKSOURCES)
	@echo Building OSCPack support
	$(CXX) -c $< -o $@ -I ./oscpack/ $(ADDITIONAL_COMPILE_FLAGS) $(OSCPACK_COMPILE_FLAGS)

$(QTSOURCES): %_moc.cc: %.h
	@echo --- Building moc file: $< \=\> $@
	$(MOC) $(MOC_OPTIONS) -o $@ $<
	@echo

$(OBJECTS): %.o: %.cc %.h deps/%.dep 
	@echo --- Building object: $< \=\> $@
	$(CXX) -c $< $(COMPILE_FLAGS)
	@echo

$(DEPS): deps/%.dep: %.cc %.h 
	@echo --- Building dependency: $< \=\> $@
	$(CXX) -M -o $@ $< $(COMPILE_FLAGS)
	@echo

.PHONY: clean reallyclean doc install

doc:
	$(DOXYGEN)

clean:
	$(RM) -f $(TARGET) $(OBJECTS) $(QTOBJECTS) $(GLEWOBJECTS) $(OSCPACKOBJECTS)  deps/*.dep *_moc.cc test_obj_loader test_tree
	$(RM) -rf doc/html/
	$(RM) -rf doc/latex/
	$(RM) -f test_tree
	make -C plugins clean
	make -C sclang clean

reallyclean: clean
	$(RM) -f *~ core* svn-commit*
	make -C plugins reallyclean
	make -C sclang reallyclean

install: all
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) $(TARGET) $(PREFIX)/bin/
	$(INSTALL) -d $(PREFIX)/share/$(TARGET)
	make -C sclang install
	make -C plugins install
#	make -C doc install

uninstall:
	$(RM) -rf $(PREFIX)/bin/$(TARGET)
	$(RM) -rf $(PREFIX)/lib/$(TARGET)
	$(RM) -rf $(PREFIX)/share/$(TARGET)

