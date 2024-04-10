#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

// Triangle vertices
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLuint genShader(GLenum type, std::string file) {
    GLuint shader = glCreateShader(type);

    std::string shaderSource;
    std::fstream shaderFile(file, std::ios::in);
    if (shaderFile.is_open()) {
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        shaderSource = sstream.str();
        shaderFile.close();
    }

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
        std::cout << "Shader: \n" << infoLog << std::endl;
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
        std::cout << "Program: " << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main(int argc, char** argv) {
    const uint16_t WINDOW_WIDTH = 400;
    const uint16_t WINDOW_HEIGHT = 300;

    // Initialize GLFW to use OpenGL 4.5
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RTX Graphics", NULL, NULL);
    if (window == NULL) {
        std::cout << "GLFW: Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    } else glfwMakeContextCurrent(window);


    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD: Failed to initialize" << std::endl;
        return -2;
    }

    // Setup viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Vertex buffer object
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate shaders
    GLuint vertexShader   = genShader(GL_VERTEX_SHADER,     "shaders/shader.vert");
    GLuint fragmentShader = genShader(GL_FRAGMENT_SHADER,   "shaders/shader.frag");

    // Shader program
    GLuint shaderProgram = genProgram(vertexShader, fragmentShader);

    // GLFW window loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(1.0, 1.0, 1.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Program termination
    glfwTerminate();
    return 0;
}