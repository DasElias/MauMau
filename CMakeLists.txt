cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(card)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

if(CMAKE_BUILD_TYPE STREQUAL "Release")   
    message("Link runtime statically.")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    set(Boost_USE_STATIC_RUNTIME ON)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if (WIN32)
    macro(get_WIN32_WINNT version)
        if (CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
            string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
            # Check for Windows 10, b/c we'll need to convert to hex 'A'.
            if ("${verMajor}" MATCHES "10")
                set(verMajor "A")
                string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
            endif ("${verMajor}" MATCHES "10")
            # Remove all remaining '.' characters.
            string(REPLACE "." "" ver ${ver})
            # Prepend each digit with a zero.
            string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
            set(${version} "0x${ver}")
        endif(CMAKE_SYSTEM_VERSION)
    endmacro(get_WIN32_WINNT)

    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
endif(WIN32)

add_compile_definitions(_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING)

option(BUILD_CLIENT "Build client" ON)
option(BUILD_SERVER "Build server" ON)
option(BUILD_SERVER_AS_DAEMON "Build server as daemon" OFF)

if(BUILD_SERVER_AS_DAEMON) 
    add_compile_definitions(BUILD_SERVER_AS_DAEMON)
endif(BUILD_SERVER_AS_DAEMON)

add_subdirectory(deps)
add_subdirectory(shared)
if(BUILD_CLIENT)
    add_subdirectory(client)
endif(BUILD_CLIENT)
if(BUILD_SERVER)
    add_subdirectory(server)
endif(BUILD_SERVER)