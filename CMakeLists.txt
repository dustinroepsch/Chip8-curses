cmake_minimum_required(VERSION 3.9)

project (Chip8)


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable (Chip8 src/main.c src/Chip8State.c)

target_link_libraries(Chip8 ${CURSES_LIBRARIES})