include_directories("${CMAKE_SOURCE_DIR}/deps/catch")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../shared")

file(GLOB_RECURSE shared_test_files
    "*.h"
    "*.cpp"
)

add_executable(shared_test ${shared_test_files})

target_link_libraries(shared_test
	${Boost_LIBRARIES}
	system
	client_model
	state
	rendering
)