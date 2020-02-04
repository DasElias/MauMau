include_directories("${CMAKE_SOURCE_DIR}/deps/catch")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../client")

file(GLOB_RECURSE client_test_files
    "*.h"
    "*.cpp"
)

add_executable(client_test ${client_test_files})

target_link_libraries(client_test
	${Boost_LIBRARIES}
	system
	client_model
	state
	rendering
)