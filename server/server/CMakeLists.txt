file(GLOB server_appl_source_files
    "*.h"
    "*.cpp"
)
add_executable(server_appl ${server_appl_source_files})
target_link_libraries(server_appl
	${Boost_LIBRARIES}
	shared_utils
	packet
	shared_model
	shared_server
	
)