cmake_minimum_required(VERSION 3.1)

project(MJLevelTool)

#add_definitions(-DDEBUG)

set (CMAKE_CXX_STANDARD 11)

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
    "../src"
    )

set (Common_sources
    ${Json_Sources}
    ${Plist_Sources}
    "../src/Utils/Utils.h"
    "../src/Utils/Utils.cpp"
    "../src/Utils/Constants.h"
    "../src/Utils/Constants.cpp"
    "../src/Exceptions/Exceptions.cpp"
    "../src/Exceptions/Exceptions.h"
    "../src/LevelObject/LevelObject.h"
    "../src/LevelObject/LevelObject.cpp"
    "../src/LevelObject/Chip.h"
    "../src/LevelObject/Chip.cpp"
    "../src/Config/ToolConfig.h"
    "../src/Config/ToolConfig.cpp"
    "../src/Randomizer/Randomizer.cpp"
    "../src/Randomizer/Randomizer.h"
    "../src/Randomizer/RandomWrapper.h"
    "../src/Randomizer/Match3Random.cpp"
    "../src/Randomizer/Match3Random.h"
    "../src/Tool/Tool.cpp"
    "../src/Tool/Tool.h"
    )

add_executable(${PROJECT_NAME}
    ${Common_sources}
    "../src/main.cpp"
    )

add_executable(${PROJECT_NAME}_TEST
    ${Common_sources}
    "../src/test_main.cpp"
    )
    
add_executable(play_level
    ${Common_sources}
    "../src/play_level.cpp"
    )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for cxx_constexpr cxx_nullptr)

