#
# Указать тип сборки (Release|Debug):
#
#   cmake -DCMAKE_BUILD_TYPE=Release ../ -G "MinGW Makefiles"
#
# Потом:
#   cmake --build .
#
#
# Под Linux можно собрать так:
#   cmake --clean . && cmake -DCMAKE_BUILD_TYPE=Debug ../ && cmake --build . && ./app_dbg
#   cmake --clean . && cmake -DCMAKE_BUILD_TYPE=Release ../ && cmake --build . && ./app_rel
#
# Сборка с использование Clang:
#   CC="clang" CXX="clang++" cmake ../ && cmake --build .
#
# https://cmake.org/Wiki/CMake_Useful_Variables
#

cmake_minimum_required( VERSION "3.15" )
SET( CMAKE_VERBOSE_MAKEFILE OFF )

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "\nPREVENTED IN-TREE BUILD: please, create the build directory outside of the source code and call cmake from there!")
endif()

# Сборка с использование Clang
SET( CLANG TRUE )
SET( CMAKE_C_COMPILER clang CACHE STRING "clang compiler" FORCE )
SET( CMAKE_CXX_COMPILER clang++ CACHE STRING "clang++ compiler" FORCE )
SET( CMAKE_AR      llvm-ar )
SET( CMAKE_LINKER  llvm-ld )
SET( CMAKE_NM      llvm-nm )
SET( CMAKE_OBJDUMP llvm-objdump )
SET( CMAKE_RANLIB  llvm-ranlib )

# Для сборки релиза следует указать: "-D CMAKE_BUILD_TYPE=Release",
if( "${CMAKE_BUILD_TYPE}" MATCHES "^Release$" )
  OPTION( WITH_DEBUG_MODE "Option debug mode" OFF )
else()
  SET( CMAKE_BUILD_TYPE "Debug" )
  OPTION( WITH_DEBUG_MODE "Option debug mode" ON )
endif()

project( DIG
  VERSION "0.1.0.2"
  DESCRIPTION "Digital 3D framework"
  HOMEPAGE_URL "https://bigov.github.io"
)
SET( PROJECT_AUTHOR "Igor Balezin aka bigov" )
SET( PROJECT_AUTHOR_EMAIL "bigov@yandex.com" )

message( STATUS "Building for ${CMAKE_SYSTEM_NAME} target system: ${CMAKE_SYSTEM}" )
message( STATUS "Building with  compiler: ${CMAKE_CXX_COMPILER_ID}" )
message( STATUS "Build type: ${CMAKE_BUILD_TYPE}" )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fexceptions -Werror \
    -Wpedantic -Wextra -Woverloaded-virtual -Wctor-dtor-privacy \
    -Wnon-virtual-dtor -Wall -Winit-self " ) ## -O3")

SET( CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
  SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
  -mwindows -Wl,-subsystem,windows " )
elseif()
  SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie" )
endif()

configure_file( "${DIG_SOURCE_DIR}/CMake/config.h.in" "${CMAKE_BINARY_DIR}/include/dig3d_config.hpp" )
include( "${DIG_SOURCE_DIR}/CMake/main.cmake" )
