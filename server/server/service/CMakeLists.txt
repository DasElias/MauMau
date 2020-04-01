file(GLOB server_service_source_files
    "*.h"
    "*.cpp"
)

add_library(server_service ${server_service_source_files})
target_link_libraries(server_service
    shared_model
    packet
    shared_server
    shared_utils
)