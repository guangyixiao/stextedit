PROJECT(xpm-instance)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)

#QT4_WRAP_CPP
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

#xpmschema
#QT4_WRAP_CPP(xpm_MOCS ./xpm.h)

#QT4_WARP_UI

SET(xpm-instance_Files
	#for example 
	#./control/ControlSynset.cpp	
	./main.cc	
	./xpm.cc
)

#find our headers
file(GLOB_RECURSE HEADERS ${xpm-instance_SOURCE_DIR}/*.h)


#create the libraries
SET(libxpm ../libxpm)
SET(libconex ../libconex)
SET(libsfa ../libsfa)
SET(libs ../../include)

INCLUDE_DIRECTORIES( ${libxpm}
                     ${libconex}
                     ${libsfa}
					 ${libjson}
					 ${libs})		 


add_executable(xpm-instance
	           ${xpm-instance_Files} 
	           ${HEADERS})

link_directories(../../library)

TARGET_LINK_LIBRARIES(xpm-instance
                      libsfa 
                      libconex 
                      libxpm	
                      Qt5::Widgets
					  Qt5::PrintSupport)
  
#set properties on the libraries
SET_TARGET_PROPERTIES(xpm-instance PROPERTIES
    VERSION ${XPMDOC_VERSION}
    SOVERSION ${XPMDOC_SOVERSION}
    COMPILE_DEFINITIONS_DEBUG _DEBUG
)


if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    if(DEFINED MINGW)
		SET(CMAKE_EXE_LINKER_FLAGS "-mwindows -fopenmp")
	else(DEFINED MINGW)
#	    SET_TARGET_PROPERTIES( XPMDOC-Gui PROPERTIES COMPILE_FLAGS "${XPMDOC_SOURCE_DIR}/src/dal/dal/db/otlv4.h" )
#        SET_TARGET_PROPERTIES( XPMDOC-Gui PROPERTIES LINK_FLAGS "odbc32.lib" )
#		SET(CMAKE_CXX_FLAGS "/DGLUT_BUILDING_LIB /openmp")
		SET(CMAKE_EXE_LINKER_FLAGS "/subsystem:windows 
		/entry:mainCRTStartup 
		/NODEFAULTLIB:libcmt.lib 
		/level='requireAdministrator'")
	endif(DEFINED MINGW)
else(CMAKE_SYSTEM_NAME MATCHES "Windows")
	SET(CMAKE_EXE_LINKER_FLAGS "-fopenmp")
endif(CMAKE_SYSTEM_NAME MATCHES "Windows")



