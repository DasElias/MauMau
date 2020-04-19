add_subdirectory(utils)
add_subdirectory(system)
add_subdirectory(gui)
add_subdirectory(network)
add_subdirectory(model)
add_subdirectory(renderingModel)
add_subdirectory(rendering)
add_subdirectory(state)

set(card_source_files Main.h Main.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Release")   
	if(MSVC)
		set(CMAKE_CREATE_WIN32_EXE "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
	else()
	endif()
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
	add_executable(card WIN32 ${card_source_files})
else()
	add_executable(card ${card_source_files})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "MauMau3D")



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
	res_font
)

install(TARGETS card DESTINATION .)

set(CPACK_GENERATOR WIX)
set(CPACK_PACKAGE_NAME "MauMau 3D")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MauMau")
set(CPACK_PACKAGE_VENDOR "Elias Horner")
set_property(INSTALL "$<TARGET_FILE_NAME:card>"
  PROPERTY CPACK_START_MENU_SHORTCUTS "MauMau 3D"
)
set(CPACK_WIX_UPGRADE_GUID "798DCAC0-0B6D-4432-A603-27464F1E821F")
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
include(CPack)
include(InstallRequiredSystemLibraries)