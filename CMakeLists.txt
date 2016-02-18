PROJECT (stextedit)

cmake_minimum_required(VERSION 2.8)

#Single output directory for building all executables and libraries.
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH "Executable Output Directory" FORCE)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH "Library Output Directory" FORCE)

# include specific modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#set characters set as unicode
add_definitions(-DUNICODE -D_UNICODE)
ADD_DEFINITIONS(-D_HAS_ITERATOR_DEBUGGING=0)

#include the projects
ADD_SUBDIRECTORY (src/libsfa)
ADD_SUBDIRECTORY (src/libconex)
ADD_SUBDIRECTORY (src/libxpm)
ADD_SUBDIRECTORY (src/conex)
ADD_SUBDIRECTORY (src/xpminstance)
ADD_SUBDIRECTORY (src/xpmschema)	  