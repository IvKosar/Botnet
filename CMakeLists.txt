cmake_minimum_required(VERSION 3.9)
project(Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project main.cpp curlGames.cpp curlGames.h Bot.cpp Bot.h)

include_directories(/usr/include/curlpp)
include_directories(/usr/include/x86_64-linux-gnu/curl)

target_link_libraries(Project curlpp curl)
