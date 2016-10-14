if(USE_CONAN)
message(STATUS "Using CONAN")
cmake_minimum_required(VERSION 2.8.12)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
else()
message(STATUS "NOT Using CONAN")

endif(USE_CONAN)

set ( SOURCES
    "src/interface.cpp"
    "src/image_converter.cpp"
)

set (HEADERS
    "include/image_converter.h"
)

include_directories("include")

#set compiler flags
if((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wreturn-type -Wpedantic ")
endif()

if(USE_CONAN)
add_library (lms_image_converter MODULE ${SOURCES} ${HEADERS})
target_link_libraries(lms_image_converter PRIVATE ${CONAN_LIBS}) #TODO remove imaging_detection wegen warp gebraucht
else()
add_library ( image_converter MODULE ${SOURCES} ${HEADERS})
target_link_libraries(image_converter PRIVATE lmscore imaging lms_imaging_detection) #TODO remove imaging_detection wegen warp gebraucht
endif(USE_CONAN)
