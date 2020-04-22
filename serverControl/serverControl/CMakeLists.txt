file(GLOB server_control_source_files
    "*.h"
    "*.cpp"
)
add_executable(server_control ${server_control_source_files})
target_link_libraries(server_control
	${Boost_LIBRARIES}
	shared_utils
	shared_server
	packet
)