file(GLOB localServer_source_files
    "*.h"
    "*.cpp"
)

add_library(localServer ${localServer_source_files})
target_link_libraries(localServer shared_server)