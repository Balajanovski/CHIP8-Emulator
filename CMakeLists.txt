cmake_minimum_required(VERSION 3.5)
project(CHIP8-Emulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)

set(SOURCE_FILES main.cpp Chip8.cpp Chip8.h Observer.h KeyLogger.cpp KeyLogger.h Subject.h Typedefs.h)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME}
                      ${SDL2_LIBRARY})