#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300

// Triangle vertices
float vertices[] = {
    -1.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    -0.5f, 0.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.5f, 0.0f, 0.0f
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
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // Vertex buffer object
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // Bindings
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // GLFW window loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // GL deallocation
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Program termination
    glfwTerminate();
    return 0;
}