file(GLOB_RECURSE client_model_source_files
    "*.h"
    "*.cpp"
)

add_library(client_model ${client_model_source_files})
target_link_libraries(client_model
shared_model
shared_server)