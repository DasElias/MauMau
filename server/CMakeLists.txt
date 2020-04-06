
# Boost
set(Boost_USE_STATIC_LIBS ON) 
find_package(Boost 1.71.0 COMPONENTS system regex filesystem log REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories("${CMAKE_SOURCE_DIR}/shared")

include_directories("${CMAKE_SOURCE_DIR}/deps/loguru")
include_directories("${CMAKE_SOURCE_DIR}/deps/json")

enable_testing()

add_subdirectory(server)
add_subdirectory(test)