cmake_minimum_required(VERSION 3.10)
project(3dview VERSION 0.1)

include(FetchContent)

# Add GLM library
set(GLM_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/glm/")
add_subdirectory("${GLM_SOURCES_DIR}")

# Add GLAD library
set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/include/glad/")
add_subdirectory("${GLAD_SOURCES_DIR}")

# Add GLFW3 library
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(glfw)

# Add GLFW3 library
FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

# Parameters
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # <- use clangd

# Compile executable
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(viewer ${SOURCES})

# Link executable
target_include_directories(viewer PUBLIC "${PROJECT_SOURCE_DIR}/include/")
target_link_libraries(viewer PUBLIC glm glad glfw assimp)
