cmake_minimum_required(VERSION 3.1)

project(MJLevelTool)

set(Plist_DIR ../lib/PlistCpp)
file(GLOB Plist_SOURCES
    ${Plist_DIR}/src/*.cpp
    ${Plist_DIR}/src/*.hpp
    )

include_directories(
    "${Plist_DIR}/include"
    "${Plist_DIR}/src"
    )

add_executable(${PROJECT_NAME}
    ${Plist_SOURCES}
    "../src/main.cpp"
    )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)
