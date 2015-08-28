set ( SOURCES
	"src/interface.cpp"
        "src/image_converter.cpp"
)

set (HEADERS
        "include/image_converter.h"
	${HEADERS_SHARED}
)

include_directories("include")

add_library ( image_converter MODULE ${SOURCES} ${HEADERS})
target_link_libraries(image_converter PRIVATE lmscore imaging)
