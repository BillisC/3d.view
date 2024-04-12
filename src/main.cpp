// Graphics Libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// C++ Libraries
#include <iostream>
#include <fstream>
#include <sstream>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// Triangles
float trA[] = {
    // vertices         //colors
    -1.0f, -1.0f, 0.0f, 1.0f, 0.7f, 0.0f,
    0.0f, -1.0f, 0.0f,  1.0f, 0.7f, 0.0f,
    -0.5f, 0.0f, 0.0f,  1.0f, 0.7f, 0.0f,

    0.0f, -1.0f, 0.0f, 1.0f, 0.7f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.7f, 0.0f,
    0.5f, 0.0f, 0.0f,  1.0f, 0.7f, 0.0f
};

float trB[] = {
    -0.5f, 0.0f, 0.0f, 1.0f, 0.7f, 0.0f,
    0.5f, 0.0f, 0.0f,  1.0f, 0.7f, 0.0f,
    0.0f, 1.0f, 0.0f,  1.0f, 0.7f, 0.0f
};

void debugMsg(std::string source, std::string error) {
    std::cout << "[DEBUG] " << source << ": " << error << std::endl;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLuint genShader(GLenum type, std::string file) {
    GLuint shader = glCreateShader(type);

    std::string shaderSource;
    std::ifstream shaderFile(file);

    if (shaderFile.is_open()) {
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        shaderSource = sstream.str();
        shaderFile.close();
    } else debugMsg("Shader", "Failed to read file");

    const char* shaderSourcePointer = shaderSource.c_str();

    glShaderSource(shader, 1, &shaderSourcePointer, NULL);
    glCompileShader(shader);

    GLint result;
    GLint infoLogLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (!result) {
        char infoLog[infoLogLength];
        glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
        debugMsg("Shader", infoLog);
    }

    return shader;
}

GLuint genProgram(GLuint &vertexShader, GLuint &fragmentShader) {
    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint result;
    GLint infoLogLength;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (!result) {
        char infoLog[infoLogLength];
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
        debugMsg("Program", infoLog);
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
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

    // Compile shaders
    GLuint vertexShader   = genShader(GL_VERTEX_SHADER,   "src/shaders/shader.vert");
    GLuint fragmentShader = genShader(GL_FRAGMENT_SHADER, "src/shaders/shader.frag");

    // Create shader program
    GLuint shaderProgram = genProgram(vertexShader, fragmentShader);

    // Vertex array object
    GLuint VAO[2];
    glGenVertexArrays(2, VAO);

    // Vertex buffer object
    GLuint VBO[2];
    glGenBuffers(2, VBO);

    // Bindings for trA
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trA), trA, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Bindings for trB
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trB), trB, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // GLFW window loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // GL deallocation
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram);

    // Program termination
    glfwTerminate();
    return 0;
}
