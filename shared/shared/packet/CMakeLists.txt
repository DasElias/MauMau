file(GLOB_RECURSE packet_source_files
    "*.h"
    "*.cpp"
)

add_library(packet ${packet_source_files})