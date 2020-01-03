include_directories("${CMAKE_SOURCE_DIR}/deps/json")

#Boost
set(Boost_USE_STATIC_LIBS ON) 
find_package(Boost COMPONENTS filesystem log REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(shared)
add_subdirectory(test)