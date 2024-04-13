cmake_minimum_required(VERSION 3.10)
project(MyOGL VERSION 0.1)

# Add GLAD library
set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/libs/glad/")
add_subdirectory("${GLAD_SOURCES_DIR}")

# Add GLFW3 library
include(FetchContent)
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(glfw)

# Compile executable
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(myogl ${SOURCES})

# Link executable
target_include_directories(myogl PUBLIC "${PROJECT_SOURCE_DIR}/include/")
target_link_libraries(myogl PUBLIC glad glfw)