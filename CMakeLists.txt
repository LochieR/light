cmake_minimum_required(VERSION 3.15)

project(light VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

add_library(light STATIC)
target_sources(light PRIVATE ${CPP_SOURCES} ${HEADERS})

include("${CMAKE_SOURCE_DIR}/dxc/cmake/caches/PredefinedParams.cmake")

set(LLVM_ENABLE_PROJECTS "" CACHE STRING "" FORCE)
set(LLVM_INCLUDE_TESTS OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_DOCS OFF CACHE BOOL "" FORCE)

set(LLVM_ENABLE_RTTI ON CACHE BOOL "" FORCE)
set(LLVM_ENABLE_EH ON CACHE BOOL "" FORCE)

set(DXC_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(DXC_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(DXC_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

add_subdirectory(dxc)

target_include_directories(light
    PUBLIC
    dxc/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(light
    PUBLIC
    dxcompiler
)

target_compile_definitions(light PRIVATE
    $<$<PLATFORM_ID:Windows>:LI_PLATFORM_WINDOWS>
    $<$<PLATFORM_ID:Darwin>:LI_PLATFORM_MAC>
    $<$<PLATFORM_ID:Linux>:LI_PLATFORM_LINUX>

    $<$<CONFIG:Debug>:LI_DEBUG>
    $<$<CONFIG:Release>:LI_RELEASE>

    $<$<PLATFORM_ID:Windows>:NOMINMAX>
)

add_executable(lighter src/lighter/main.cpp)

target_link_libraries(lighter PRIVATE light)
