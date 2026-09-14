cmake_minimum_required(VERSION 3.15)
project(a2l_json_export LANGUAGES CXX)

find_package(a2lparser CONFIG REQUIRED)
add_executable(a2l_json main.cpp)
target_link_libraries(a2l_json PRIVATE a2lparser::a2lparser)
