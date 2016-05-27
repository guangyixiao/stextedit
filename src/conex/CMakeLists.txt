PROJECT (conex)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

FIND_PACKAGE(Qt5Widgets REQUIRED)

#QT4_WRAP_CPP
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

#QT4_WRAP_CPP(conexide_MOCS ./conexide.h)
QT5_WRAP_UI(conex_UIS ./conexide.ui
                      ./FindDialog.ui)
QT5_ADD_RESOURCES(conex_SRCS ./conexmain.qrc)
#QT4_WARP_UI

SET(conex_Files
	#for example 
	#./control/ControlSynset.cpp	
	./main.cc	
	./conexide.cc
	./FindDialog.cc
	./VocabularyTableModel.cc
#	./viewmodel/ModifiedTableModel.cc
#	./viewmodel/HistoryTableModel.cc
	${conex_UIS}
	${conex_SRCS}	
)


#find our headers
file(GLOB_RECURSE HEADERS ${conex_SOURCE_DIR}/*.h)



#create the libraries
SET(libxpm ../libxpm)
SET(libconex ../libconex)
SET(libsfa ../libsfa)
SET(libs ../../include)

INCLUDE_DIRECTORIES( ${libxpm}
                     ${libconex}
                     ${libsfa}
					 ${libs}			
					 )	

add_executable(conex
	           ${conex_Files} 
	           ${HEADERS})

FIND_PACKAGE(XERCESC REQUIRED)

link_directories(../../library)

TARGET_LINK_LIBRARIES(conex
                      libsfa 
                      libconex 
                      libxpm	
                      Qt5::Widgets )
			   
					  
#set properties on the libraries
SET_TARGET_PROPERTIES(conex PROPERTIES
    VERSION ${conexide_VERSION}
    SOVERSION ${conexide_SOVERSION}
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