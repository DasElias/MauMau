file(GLOB state_source_files
    "*.h"
    "*.cpp"
)

add_library(state ${state_source_files})
target_link_libraries(state
	client_utils
	shared_utils
	packet
	rendering
)