include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../server")

file(GLOB_RECURSE server_test_files
    "*.h"
    "*.cpp"
)

add_executable(server_test ${server_test_files})
enable_testing()
add_test(server_test server_test)

target_link_libraries(server_test
	Catch2::Catch2
	${Boost_LIBRARIES}
	server_service
)

include(CTest)
include(Catch)
catch_discover_tests(server_test)