include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../client")

file(GLOB_RECURSE client_test_files
    "*.h"
    "*.cpp"
)

add_executable(client_test ${client_test_files})
enable_testing()
add_test(client_test client_test)

target_link_libraries(client_test
	Catch2::Catch2
	${Boost_LIBRARIES}
	system
	client_model
	state
	rendering
)

include(CTest)
include(Catch)
catch_discover_tests(client_test)