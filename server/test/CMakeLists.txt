include_directories("${CMAKE_SOURCE_DIR}/deps/catch")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../server")

file(GLOB_RECURSE server_test_files
    "*.h"
    "*.cpp"
)

add_executable(server_test ${server_test_files})

target_link_libraries(server_test
	${Boost_LIBRARIES}
	server_service
)