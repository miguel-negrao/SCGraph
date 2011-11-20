################################################################################

cmake_minimum_required (VERSION 2.8)
project (scgraph)

# The version number.
set (scgraph_VERSION_MAJOR 0)
set (scgraph_VERSION_MINOR 14)

################################################################################

if (APPLE OR WIN32)
  set(CMAKE_SHARED_MODULE_SUFFIX ".bundle")
  set(PLUGIN_EXTENSION ".bundle")
endif()

################################################################################

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules
                      ${CMAKE_MODULE_PATH})

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

set(SC3_LIBRARY_PATH "/Applications/SuperCollider")

add_subdirectory(src)

################################################################################

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

################################################################################

find_package(SuperCollider3)
if (NOT SC_FOUND)
	message(SEND_ERROR "cannot find SuperCollider3 headers. Set the variable SC_PATH.")
else()
	message(STATUS "Using SC source located at ${SC_PATH}")
endif()

################################################################################

# this doesn't seem to be used anyways
#option(GLEW "If you have libGLEW installed and want scgraph to check for shader extensions before using them" ON)

option(SHADERS "Build with experimental vertex and fragment shader support" ON)
option(JACK "Build with jack support for the FFT UGen")

# option(QUARKS "Install plugins as quarks")
# option(OSX_PACKAGE "Package dmg for Apple")
# option(IN_PLACE_BUILD "Build and install in cmake build folder" ON)

# # osx `make install' defaults into cmake_build/SC3-plugins directory
# if (APPLE AND IN_PLACE_BUILD)
#   set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")
# endif()

# # when packaging OSX dmg, install in 'cmake_build/build_osx' so we can include
# # directly the created SC3plugins subfolder in the root of the dmg (instead of
# # all plugins directories in the dmg root)
# if (APPLE AND OSX_PACKAGE)
#   set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/build_osx")
# endif()

# if (QUARKS OR OSX_PACKAGE) # add quarks DIRECTORY in osx dmg
#   add_subdirectory(quarks)
# endif()

# if (OSX_PACKAGE)
#   add_subdirectory(osx_package)
# endif()


set(plugins "")

GET_GCC_VERSION(gcc_version)

set(INSTALL_DESTINATION_PLUGINS "lib/scgraph/plugins")

if (LINUX)
    set(INSTALL_DESTINATION "lib/SuperCollider/plugins/scgraph")
    if (QUARKS)
        set(INSTALL_DESTINATION_DISTRO
                "share/SuperCollider/scgraph")
    else()
        set(INSTALL_DESTINATION_DISTRO
                "share/SuperCollider/Extensions/scgraph")
    endif(QUARKS)
elseif(APPLE)
    set(INSTALL_DESTINATION "scgraph")
    set(INSTALL_DESTINATION_DISTRO "SCGraph")
endif(LINUX)

################################################################################
### install docs, sc classes and examples

if(APPLE)
  install(DIRECTORY src/sc/ DESTINATION
    ${CMAKE_BINARY_DIR}/${INSTALL_DESTINATION_DISTRO})
else()
  install(DIRECTORY src/sc/ DESTINATION ${INSTALL_DESTINATION_DISTRO})
endif(APPLE)

install(DIRECTORY doc/examples DESTINATION share/doc/examples)