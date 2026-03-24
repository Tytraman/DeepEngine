cmake_minimum_required(VERSION 3.15.7)

find_package(assimp CONFIG REQUIRED)

target_link_libraries(DeepEngine PRIVATE assimp::assimp)

target_include_directories(DeepEngine
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}")

target_sources(DeepEngine
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/Assimp/loader.cpp")