PROJECT(libconex)

#find xerces package

#add the files to our groups and core
SOURCE_GROUP("control" ./control/*)
SOURCE_GROUP("dal" ./dal/*)
#SOURCE_GROUP("dal-db" ./dal/db/*)
SOURCE_GROUP("dal-xml" ./dal/xml/*)
SOURCE_GROUP("model" ./model/*)
SOURCE_GROUP("ternary_tree" ./ternary_tree/*)
SOURCE_GROUP("util" ./util/*)
#seems to be a bug in cmake, can't put these in analysis\\standard folder :(
#control detail

SET(libconex_Files
	#for example 
	#./control/ControlSynset.cpp	
	./control/VocabularyControl.cc	
	./control/VocabularyXmlControl.cc	
	./control/TermControl.cc
	./control/ClassifierControl.cc
	# for dal
	./dal/VocabularyAccess.cc
	./dal/VocabularyAccessFactory.cc
#	./dal/db/VocabularyDbAccess.cc	
	./dal/xml/VocabularyXmlAccess.cc
	./dal/xml/VocabularySax2Handler.cc
	./dal/xml/XmlHandler.cc
	./dal/xml/VocabularyXmlHandler.cc
	./dal/xml/TermXmlHandler.cc
	./dal/xml/TermContextXmlHandler.cc
	./dal/xml/IdXmlHandler.cc
	./dal/xml/StructureXmlHandler.cc
	./dal/xml/ConceptXmlHandler.cc
	# for model
	./model/Concept.cc	
	./model/Id.cc
	./model/Interpretation.cc
	./model/Relation.cc
	./model/Structure.cc
	./model/Context.cc
	./model/Term.cc
	./model/Vocabulary.cc
	# for util
	./util/SdfEncoder.cc
	# for ternary_tree
	./ternary_tree/examples.cpp
)


#find our headers
file(GLOB_RECURSE HEADERS ${libconex_SOURCE_DIR}/*.h ${libconex_SOURCE_DIR}/*.hpp)

SET(libs ../../include)

#create the libraries
INCLUDE_DIRECTORIES( ${libconex_SOURCE_DIR}
                     ${libs}
					 )

#add_executable(CONEX-App
#	${CONEX-App_Files} ${HEADERS}
#)

add_library(libconex
    STATIC
	 ${libconex_Files} ${HEADERS}
	)

#set properties on the libraries
SET_TARGET_PROPERTIES(libconex PROPERTIES
    VERSION ${stextedit_VERSION}
    SOVERSION ${stextedit_SOVERSION}
    COMPILE_DEFINITIONS_DEBUG _DEBUG
)

#SET_TARGET_PROPERTIES( CONEX-Model PROPERTIES COMPILE_FLAGS "${CONEX_SOURCE_DIR}/src/dal/dal/otlv4.h" )
#SET_TARGET_PROPERTIES( CONEX-Model PROPERTIES LINK_FLAGS "odbc32.lib" )



