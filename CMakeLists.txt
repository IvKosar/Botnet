cmake_minimum_required(VERSION 3.7)
project(botnet)

set(CMAKE_CXX_STANDARD 11)

add_executable(botnet main.cpp SimpleBot.cpp SimpleBot.h)

find_package(Boost COMPONENTS system thread timer REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(botnet ${Boost_LIBRARIES} pthread)
