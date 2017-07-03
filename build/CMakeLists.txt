cmake_minimum_required(VERSION 3.1)

project(MJLevelTool)

set(Plist_DIR ../lib/PlistCpp)
set(Json_DIR  ../lib/JsonCpp)

file(GLOB Plist_Sources
    ${Plist_DIR}/src/*.cpp
    ${Plist_DIR}/src/*.hpp
    )

file(GLOB Json_Sources
    ${Json_DIR}/*.cpp
    ${Json_DIR}/*.h
    )

include_directories(
    "${Plist_DIR}/include"
    "${Plist_DIR}/src"
    "${Json_DIR}"
    )

add_executable(${PROJECT_NAME}
    ${Json_Sources}
    ${Plist_Sources}
    "../src/Config/ToolConfig.h"
    "../src/Config/ToolConfig.cpp"
    "../src/main.cpp"
    )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)
