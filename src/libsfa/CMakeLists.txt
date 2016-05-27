PROJECT (libsfa)
# add libjson lib
SET(libs ../../include)
INCLUDE_DIRECTORIES( ${libs})

SET(libsfa_Files
    ./sfa_parser.cpp
    ./sfa.cpp
	./sfa_sign.cpp
)

file(GLOB_RECURSE HEADERS ${libsfa_SOURCE_DIR}/*.h)

add_library(libsfa
    STATIC
	${libsfa_Files} ${HEADERS}
	)

PROJECT(sfa)

SET(sfa_Files
    ./test.cpp
)

#find our headers
file(GLOB_RECURSE HEADERS ${sfa_SOURCE_DIR}/*.h)

add_executable(sfa
	${sfa_Files} 
	${HEADERS}
)

link_directories(../../library)

TARGET_LINK_LIBRARIES(sfa
                      libsfa 
                      )					 