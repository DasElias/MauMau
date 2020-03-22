add_compile_definitions(WIN32_LEAN_AND_MEAN)

# GLFW
find_package(glfw3 3.3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})

# Boost
set(Boost_USE_STATIC_LIBS ON) 
find_package(Boost COMPONENTS log regex filesystem date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/res/include")

include_directories("${CMAKE_SOURCE_DIR}/shared")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/glad")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/glm")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/nanovg")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/egui")

include_directories("${CMAKE_SOURCE_DIR}/deps/loguru")
include_directories("${CMAKE_SOURCE_DIR}/deps/simpleIni")
include_directories("${CMAKE_SOURCE_DIR}/deps/json")

enable_testing()

add_subdirectory(res)
add_subdirectory(deps)
add_subdirectory(client)
add_subdirectory(test)

