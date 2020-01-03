file(GLOB_RECURSE network_source_files
    "*.h"
    "*.cpp"
)

add_library(network ${network_source_files})