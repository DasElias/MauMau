file(GLOB shared_utils_source_files
    "*.h"
    "*.cpp"
)

add_library(shared_utils ${shared_utils_source_files})