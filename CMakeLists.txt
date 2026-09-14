cmake_minimum_required(VERSION 3.15)
project(a2lparser-lib LANGUAGES CXX)

find_package(a2lparser CONFIG REQUIRED)

# ---------------------------------------------------------------------------
# Examples
# ---------------------------------------------------------------------------
option(A2L_BUILD_EXAMPLES "Build example applications" ON)

if(A2L_BUILD_EXAMPLES)
    add_executable(a2l_basic examples/basic/main.cpp)
    target_link_libraries(a2l_basic PRIVATE a2lparser::a2lparser)

    add_executable(a2l_json examples/json_export/main.cpp)
    target_link_libraries(a2l_json PRIVATE a2lparser::a2lparser)
endif()
