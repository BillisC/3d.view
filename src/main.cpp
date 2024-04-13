// Graphics Libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include "debug.h"
#include "shader_pipeline.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// Triangles
float triforce[] = {
    // vertices         //colors
    -1.0f, -1.0f, 0.0f, 0.8f, 0.5f, 0.0f,
    0.0f, -1.0f, 0.0f,  0.8f, 0.5f, 0.0f,
    -0.5f, 0.0f, 0.0f,  1.0f, 0.7f, 0.0f,

    0.0f, -1.0f, 0.0f,  0.8f, 0.5f, 0.0f,
    1.0f, -1.0f, 0.0f,  0.8f, 0.5f, 0.0f,
    0.5f, 0.0f, 0.0f,   1.0f, 0.7f, 0.0f,

    -0.5f, 0.0f, 0.0f,  1.0f, 0.7f, 0.0f,
    0.5f, 0.0f, 0.0f,   1.0f, 0.7f, 0.0f,
    0.0f, 1.0f, 0.0f,   0.9f, 0.6f, 0.0f
};

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char** argv) {
    // Initialize GLFW for use with OpenGL 4.5
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RTX Graphics", NULL, NULL);
    if (window == NULL) {
        debugMsg("GLFW", "Failed to create window");
        glfwTerminate();
        return -1;
    } else glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        debugMsg("GLAD", "Failed to initialize");
        return -2;
    }

    // Setup viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Create shader program
    ShaderPaths paths = { "src/shaders/shader.vert", "src/shaders/shader.frag" };
    ShaderPipeline* pipeline = new ShaderPipeline(paths);

    // Vertex array object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // Vertex buffer object
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Bindings for triforce
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triforce), triforce, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // GLFW window loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        (*pipeline).use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 9);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // GL deallocation
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete pipeline;

    // Program termination
    glfwTerminate();
    return 0;
}
