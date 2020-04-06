include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../shared")

file(GLOB_RECURSE shared_test_files
    "*.h"
    "*.cpp"
)

add_executable(shared_test ${shared_test_files})
enable_testing()
add_test(shared_test shared_test)

target_link_libraries(shared_test
	Catch2::Catch2
	${Boost_LIBRARIES}
	system
	client_model
	state
	rendering
)

include(CTest)
include(Catch)
catch_discover_tests(shared_test)