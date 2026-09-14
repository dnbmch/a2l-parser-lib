cmake_minimum_required(VERSION 3.15)
project(a2l_basic_example LANGUAGES CXX)

find_package(a2lparser CONFIG REQUIRED)
add_executable(a2l_basic main.cpp)
target_link_libraries(a2l_basic PRIVATE a2lparser::a2lparser)
