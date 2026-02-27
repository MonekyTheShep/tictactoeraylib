cmake_minimum_required(VERSION 4.1)
project(E621RayLib C)
set(CMAKE_C_STANDARD 99)

find_package(CURL REQUIRED)
find_package(cjson REQUIRED)
find_package(raylib REQUIRED)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

# Adding raygui
FetchContent_Declare(
        raygui
        GIT_REPOSITORY "https://github.com/raysan5/raygui.git"
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raygui)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include/") # Define PROJECT_INCLUDE to be the path to the include directory of the project


# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_include_directories(${PROJECT_NAME} PRIVATE ${raygui_SOURCE_DIR}/src)


target_link_libraries(${PROJECT_NAME}
        PRIVATE
        raylib
        cjson
        CURL::libcurl
)

target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -fsanitize=address -g
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable