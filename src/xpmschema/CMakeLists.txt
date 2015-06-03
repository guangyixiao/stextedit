PROJECT(xpm-schema)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)

#QT4_WRAP_CPP
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

#xpmschema
#QT4_WRAP_CPP(xpmschema_MOCS ./xpmschema.h)

#QT4_WARP_UI

SET(xpm-schema_Files
	#for example 
	#./control/ControlSynset.cpp	
	./main.cc	
	./xpmschema.cc
	${xpmschema_MOCS}	
)


#find our headers
file(GLOB_RECURSE HEADERS ${xpm-schema_SOURCE_DIR}/*.h)


#create the libraries
SET(libxpm ../libxpm)
SET(libconex ../libconex)
SET(libsfa ../libsfa)
SET(libjson ../../../libs/libjson-master)

INCLUDE_DIRECTORIES( ${libxpm}
                     ${libconex}
                     ${libsfa}
					 ${libjson}
					 )			 


add_executable(xpm-schema
	           ${xpm-schema_Files} 
	           ${HEADERS})

FIND_PACKAGE(XERCESC REQUIRED)

TARGET_LINK_LIBRARIES(xpm-schema
                      libsfa 
                      libconex 
                      libxpm	
                      D:/Projects/libs/libjson-master/bin/release/libjson.lib					  
					  ${XERCESC_LIBRARY}
					  Qt5::Widgets
					  Qt5::PrintSupport)
					  

#set properties on the libraries
SET_TARGET_PROPERTIES(xpm-schema PROPERTIES
    VERSION ${stextedit_VERSION}
    SOVERSION ${stextedit_SOVERSION}
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



