cmake_minimum_required(VERSION 3.7)
project(botnet)

set(CMAKE_CXX_STANDARD 14)

add_executable(botnet main.cpp curlGames.cpp curlGames.h Bot.cpp Bot.h)

include_directories(${CURLPP_LIB})
include_directories(${CURL_LIB})

target_link_libraries(botnet curlpp curl)
