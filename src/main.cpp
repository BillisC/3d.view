#define STB_IMAGE_IMPLEMENTATION

// Graphics Libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include "debug.h"
#include "shader_pipeline.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

// Quad
float quad[] = {
	// positions          // colors           // texture coords
	0.5f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	0.5f,  -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.5f, 0.5f,  0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3	 // second triangle
};

GLuint genTexture(std::string path) {
	// Generate texture buffer
    GLuint texture;
	glGenTextures(1, &texture);

    // Binding
	glBindTexture(GL_TEXTURE_2D, texture);

    // Set parameters
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image to texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else debugMsg("Texture", "Failed to load image data");

	stbi_image_free(data);

	return texture;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, char **argv) {
	// Initialize GLFW for use with OpenGL 4.5
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3d.view", NULL, NULL);
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
	ShaderPaths paths = {"src/shaders/shader.vert", "src/shaders/shader.frag"};
	ShaderPipeline *pipeline = new ShaderPipeline(paths);

	// Generate buffer objects
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO); // Vertex Array Object
	glGenBuffers(1, &VBO);      // Vertex Buffer Object
	glGenBuffers(1, &EBO);      // Element Buffer Object

	// Bindings for quad
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Data structure
    // | X Y Z | R G B | TX TY |
    // |   0   |   1   |   2   |
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Texture
	GLuint texture = genTexture("assets/wall.jpg");

	// GLFW window loop
	while (!glfwWindowShouldClose(window)) {
		// Event Handling
        processInput(window);

        // Clear window buffer
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

        // Enable shader program
		(*pipeline).use();

		// Model transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f),glm::vec3(0.0f, 1.0f, 0.0f));

		// View transformation
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		// Apply projection
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,	0.1f, 100.0f);

		(*pipeline).setUniMat4f("model", glm::value_ptr(model));
		(*pipeline).setUniMat4f("view", glm::value_ptr(view));
		(*pipeline).setUniMat4f("projection", glm::value_ptr(projection));

        // Draw window
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
