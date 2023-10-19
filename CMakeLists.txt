cmake_minimum_required(VERSION 3.0.0)

project("ozone")

include_directories("lua")
link_directories("lua")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wno-multichar")

add_executable("ozone" "src/ozone.c" "src/avl.c" "src/screen.c" "src/time.c" "src/string.c" "src/utf8.c")
target_link_libraries("ozone" "ncurses" "lua" "m")


install(DIRECTORY "src/scripts" DESTINATION "$ENV{HOME}/.ozone" USE_SOURCE_PERMISSIONS)