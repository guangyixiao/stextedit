PROJECT (libsfa)
# add libjson lib
SET(libjson ../../../libs/libjson-master)
INCLUDE_DIRECTORIES( ${libjson})

SET(libsfa_Files
    ./sfa.cpp
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

TARGET_LINK_LIBRARIES(sfa
                      libsfa 
                      D:/Projects/libs/libjson-master/bin/release/libjson.lib
					  )					 