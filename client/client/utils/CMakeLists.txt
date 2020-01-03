file(GLOB client_utils_source_files
    "*.h"
    "*.cpp"
)

add_library(client_utils ${client_utils_source_files})