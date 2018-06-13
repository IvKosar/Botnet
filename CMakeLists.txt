cmake_minimum_required(VERSION 3.5)
project(botnet)

set(CMAKE_CXX_STANDARD 11)

add_executable(botnet attack_bot/main.cpp attack_bot/SimpleBot.cpp attack_bot/SimpleBot.h)
set(CMAKE_CXX_FLAGS_DEBUG -fpermissive)

find_package(Boost COMPONENTS system thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(botnet ${Boost_LIBRARIES} pthread)
