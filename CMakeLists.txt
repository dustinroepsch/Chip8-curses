cmake_minimum_required(VERSION 3.9)

project (Chip8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Curses REQUIRED)
include_directories("${CURSES_INCLUDE_DIR}/src")

add_executable (Chip8 src/main.c src/Chip8State.c)

target_link_libraries(Chip8 ${CURSES_LIBRARIES})