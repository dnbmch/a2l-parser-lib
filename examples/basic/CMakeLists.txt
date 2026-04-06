cmake_minimum_required(VERSION 3.14)
project(a2l_basic_example LANGUAGES CXX)

# Pull in a2lparser as a subdirectory.
# Adjust the path if you place this example elsewhere.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_BINARY_DIR}/a2lparser)

add_executable(a2l_basic main.cpp)
target_link_libraries(a2l_basic PRIVATE a2lextract)
