cmake_minimum_required(VERSION 3.14)
project(a2lextract-lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Find protobuf (needed for proto generation and linking)
# ---------------------------------------------------------------------------
find_package(Protobuf CONFIG QUIET)
if(NOT TARGET protobuf::libprotobuf)
    find_package(Protobuf REQUIRED)
endif()
find_package(absl CONFIG QUIET)

# ---------------------------------------------------------------------------
# Prebuilt static library
# ---------------------------------------------------------------------------
# Set A2L_LIB_DIR to the directory containing liba2lextract.a
# e.g. cmake -B build -DA2L_LIB_DIR=lib
if(NOT A2L_LIB_DIR)
    set(A2L_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib" CACHE PATH
        "Directory containing prebuilt liba2lextract.a and generated .pb.h files")
endif()
get_filename_component(A2L_LIB_DIR "${A2L_LIB_DIR}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(a2lextract STATIC IMPORTED GLOBAL)
set_target_properties(a2lextract PROPERTIES
    IMPORTED_LOCATION "${A2L_LIB_DIR}/liba2lextract.a"
)

target_include_directories(a2lextract INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${A2L_LIB_DIR}
)

target_link_libraries(a2lextract INTERFACE protobuf::libprotobuf)

# ---------------------------------------------------------------------------
# Examples
# ---------------------------------------------------------------------------
option(A2L_BUILD_EXAMPLES "Build example applications" ON)

if(A2L_BUILD_EXAMPLES)
    add_executable(a2l_basic examples/basic/main.cpp)
    target_link_libraries(a2l_basic PRIVATE a2lextract)

    add_executable(a2l_json examples/json_export/main.cpp)
    target_link_libraries(a2l_json PRIVATE a2lextract)
endif()
