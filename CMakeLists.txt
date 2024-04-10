cmake_minimum_required(VERSION 3.10)
project(MyOGL VERSION 0.1)

# Add GLAD library
set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/libs/glad/")
add_subdirectory("${GLAD_SOURCES_DIR}")

# Add GLFW3 library
set(GLFW_SOURCES_DIR "${PROJECT_SOURCE_DIR}/libs/glfw/")
add_subdirectory("${GLFW_SOURCES_DIR}")

# Compile executable
add_executable(myogl src/main.cpp)

# Link executable
target_include_directories(myogl PUBLIC "${PROJECT_SOURCE_DIR}/include/")
target_link_libraries(myogl PUBLIC glad glfw)