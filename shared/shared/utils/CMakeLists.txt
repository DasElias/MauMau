file(GLOB shared_utils_source_files
    "*.h"
    "*.cpp"
)

add_library(shared_utils ${shared_utils_source_files})
target_link_libraries(shared_utils
    ${Boost_LIBRARIES}
)