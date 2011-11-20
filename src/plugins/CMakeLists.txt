################################################################################
### shaders

if(SHADERS)
  add_definitions(-DHAVE_SHADERS)
endif()

################################################################################
### sources

set(plugins_srcs bordered_rectangle cube gl_renderer translate rotate stretch binary_op mul_add color light line points triangle rectangle polygon quad blending lighting culling transformation material shader_program)

################################################################################
### qt

set(plugins_moc_hdrs bordered_rectangle.h cube.h gl_renderer.h translate.h rotate.h stretch.h binary_op.h mul_add.h color.h light.h line.h points.h triangle.h rectangle.h polygon.h quad.h blending.h lighting.h culling.h transformation.h material.h shader_program.h)

# add some useful macros and variables
INCLUDE( ${QT_USE_FILE} )

################################################################################
### want jack?

if (JACK)
  include_directories(${FFTW3F_INCLUDE_DIR})
  include_directories(${JACK_INCLUDE_DIRS})

  add_definitions(-DHAVE_JACK)

  list(APPEND plugins_srcs freq_amp)
  list(APPEND plugins_moc_srcs freq_amp.h) ##???
endif()


################################################################################
### includes

include_directories(${GraphicsMagick++_INCLUDE_DIRS})

include_directories(${CMAKE_BINARY_DIR})

include_directories(${SC_PATH}/include/plugin_interface)
include_directories(${SC_PATH}/include/common)
include_directories(${SC_PATH}/include/server) # for SC_Group.h

include_directories(${SC_PATH}/external_libraries/libsndfile)
#include_directories(${SC_PATH}/external_libraries/oscpack/)

include_directories(../glew)
include_directories(../oscpack)

################################################################################

add_definitions(-DGLEW_MX)

add_definitions(-DPREFIX="${CMAKE_INSTALL_PREFIX}")

set(CMAKE_SHARED_MODULE_PREFIX "")
set(CMAKE_SHARED_MODULE_SUFFIX ".so")

if(APPLE)
  add_definitions(-DSC_DARWIN)
  set(CMAKE_SHARED_MODULE_SUFFIX ".bundle")
endif(APPLE)

################################################################################
### link qt, build, link and install

link_directories(${QT_LIBRARIES})

foreach(plugin ${plugins_srcs})
  QT4_WRAP_CPP(${plugin}_moc_src ${plugin}.h)
  add_library(${plugin} MODULE ${plugin} ${${plugin}_moc_src})
  if(APPLE)
    target_link_libraries(${plugin}
      -bundle 
      -fPIC 
      "-undefined dynamic_lookup")
  elseif(LINUX)
    target_link_libraries(${plugin} 
      -shared 
      -fpic 
      -export-dynamic 
      -rdynamic 
      dl
      "")
  else(APPLE)
    target_link_libraries(${plugin} 
      -shared 
      -fpic 
      -export-dynamic 
      -rdynamic 
      dl
      "")
  endif(APPLE)
  install(FILES ${CMAKE_BINARY_DIR}/src/plugins/${plugin}${CMAKE_SHARED_MODULE_SUFFIX}
    DESTINATION "lib/scgraph/plugins/")
endforeach(plugin)
