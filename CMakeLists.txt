cmake_minimum_required(VERSION 3.5)
project(CHIP8-Emulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)

file(GLOB SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/*.h"
        "${PROJECT_SOURCE_DIR}/*.hpp"
        "${PROJECT_SOURCE_DIR}/*.cpp"
        )

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

include_directories(${SDL2_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
        ${SDL2_LIBRARY})