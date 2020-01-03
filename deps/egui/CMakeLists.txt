file(GLOB_RECURSE egui_source_files FOLLOW_SYMLINKS
    "*.h"
    "*.cpp"
)

# GLFW
find_package(glfw3 3.3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})

add_library(egui ${egui_source_files})
target_include_directories(egui PRIVATE "${CMAKE_SOURCE_DIR}/deps/nanovg")
target_include_directories(egui PRIVATE "${CMAKE_SOURCE_DIR}/deps/glad")
