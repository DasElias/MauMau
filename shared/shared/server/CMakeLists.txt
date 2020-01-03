file(GLOB_RECURSE shared_server_source_files
    "*.h"
    "*.cpp"
)

add_library(shared_server ${shared_server_source_files})