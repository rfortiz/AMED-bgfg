cmake_minimum_required(VERSION 3.9)
project(bgfg_amed VERSION 1.0.0 DESCRIPTION "Implementation of adaptive median background with openCV interface")

find_package(OpenCV REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")


if( CMAKE_COMPILER_IS_GNUCXX AND NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "-std=c++11 -pedantic -Wall -Wextra -Wconversion -Wsign-conversion" CACHE STRING "Flags used by the compiler during all build types" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG" CACHE STRING "Flags used by the compiler during release builds" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb" CACHE STRING "Flags used by the compiler during debug builds" FORCE)
endif()

include_directories(include)
add_library(bgfgamed SHARED src/bgfg_amed.cpp)

install (TARGETS bgfgamed DESTINATION lib)
install (FILES include/bgfg_amed.hpp DESTINATION include)

option( BUILD_DEMO "Build demo" ON )
if( BUILD_DEMO )
    add_subdirectory(example)
endif()

option( BUILD_TEST "Build test" ON )
if( BUILD_TEST )
    add_subdirectory(tests)
endif()

set(CONFIGURED_ONCE TRUE CACHE INTERNAL  "A flag showing that CMake has configured at least once.")
