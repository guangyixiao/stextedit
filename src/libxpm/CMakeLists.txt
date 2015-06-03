PROJECT(libxpm)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

FIND_PACKAGE(Qt5Widgets REQUIRED)
#FIND_PACKAGE(Qt5Declarative)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
#INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDES})

#add the files to our groups and app
SOURCE_GROUP("mapedit" ./mapedit/*)
#seems to be a bug in cmake, can't put these in analysis\\standard folder :(
#upper detail

#QT4_WRAP_CPP
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

#QT4_WRAP_CPP(XpmTextEdit_MOCS ./XpmTextEdit.h)
#QT4_WRAP_CPP(XpmCompleter_MOCS ./XpmCompleter.h)
#map edit
#QT4_WRAP_CPP(mapeditor_MOCS ./mapedit/mapeditor.h)
#QT4_WRAP_CPP(addmap_MOCS ./mapedit/addmap.h)
#QT4_WRAP_CPP(removemap_MOCS ./mapedit/removemap.h)
#QT4_WRAP_CPP(XpmDocModel_MOCS ./mapedit/XpmDocModel.h)
#QT4_WRAP_CPP(XpmLineEdit_MOCS ./mapedit/XpmLineEdit.h)
QT5_ADD_RESOURCES(libxpm_SRCS 
                      ./mapedit/mapeditor.qrc
					  ./textedit.qrc)
					  
QT5_WRAP_UI(libxpm_UIS ./mapedit/mapeditor.ui
                      ./mapedit/addmap.ui
					  ./mapedit/removemap.ui)
					  
#QT4_WARP_UI

SET(libxpm_Files
	#for example 
	#./control/ControlSynset.cpp	
#	./textedit/main.cc	
#	./textedit/xpmschema.cc
#	./textedit/xpm.cc
	./mapedit/mapeditor.cc
	./mapedit/addmap.cc
	./mapedit/removemap.cc
	./mapedit/XpmDocModel.cc
	./mapedit/TreeItem.cc
	./mapedit/BlockTreeItem.cc
	./mapedit/TermTreeItem.cc
	./mapedit/XpmLineEdit.cc
	./XpmTextEdit.cc
	./XpmCompleter.cc
	${libxpm_UIS}
	${libxpm_SRCS}
)


#find our headers
file(GLOB_RECURSE HEADERS ${libxpm_SOURCE_DIR}/*.h)



#create the libraries
SET(libconex ../libconex)
SET(libsfa ../libsfa)
SET(libjson ../../../libs/libjson-master)

INCLUDE_DIRECTORIES( ${libxpm_SOURCE_DIR} 
                     ${libjson}
					 ${libsfa}
					 ${libconex}
					 )			 


add_library(libxpm
	           ${libxpm_Files} 
	           ${HEADERS})
			   
qt5_use_modules(libxpm Widgets)