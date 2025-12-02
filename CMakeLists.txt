cmake_minimum_required(VERSION 3.5)
project(C_Text_Game C)

set(CMAKE_C_STANDARD 11)

add_executable(C_Text_Game main.c game_functions/rooms.c)
