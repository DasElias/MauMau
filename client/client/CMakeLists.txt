add_subdirectory(utils)
add_subdirectory(system)
add_subdirectory(gui)
add_subdirectory(network)
add_subdirectory(model)
add_subdirectory(renderingModel)
add_subdirectory(rendering)
add_subdirectory(state)

set(card_source_files Main.h Main.cpp)
add_executable(card ${card_source_files})

message("GLFW linked library: \"${GLFW3_LIBRARY}\"")
target_link_libraries(card
	${GLFW3_LIBRARY}
	${Boost_LIBRARIES}
	glad
	nanovg
	egui
	system
	network
	client_model
	state
	rendering
	gui
)