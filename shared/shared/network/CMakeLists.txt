file(GLOB_RECURSE shared_network_source_files
    "*.h"
    "*.cpp"
)

add_library(shared_network ${shared_network_source_files})
target_link_libraries(shared_network
    packet
)