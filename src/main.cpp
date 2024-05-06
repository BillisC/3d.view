#include <iostream>

// Graphics Libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Project Libraries
#include "shader_pipeline.h"
#include "debug.h"

void debugMsg(std::string source, std::string error) {
   std::cout << "[DEBUG] " << source << ": " << error << "\n" << std::flush;
}

// Rectangle
/* clang-format off */
float rect[] = {
	//X     Y      Z     TX    TY
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
};
/* clang-format on */

// Window defines
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

// Camera parameters
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float lastX = float(WINDOW_WIDTH) / 2;
float lastY = float(WINDOW_HEIGHT) / 2;

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

bool firstMouse = false;

// Delta time
float deltaTime, lastTime = 0.0f;

GLuint genTexture(std::string path) {
   // Generate texture buffer
   GLuint texture;
   glGenTextures(1, &texture);

   // Binding
   glBindTexture(GL_TEXTURE_2D, texture);

   // Set parameters
   glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Load image to texture
   int width, height, nrChannels;
   unsigned char *data =
       stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
   if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   } else
      debugMsg("Texture", "Failed to load image data");

   stbi_image_free(data);

   return texture;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
   glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
   if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
   }

   float xoffset = xpos - lastX;
   float yoffset = lastY - ypos;
   lastX = xpos;
   lastY = ypos;

   float sensitivity = 0.1f;
   xoffset *= sensitivity;
   yoffset *= sensitivity;

   yaw += xoffset;
   pitch += yoffset;

   if (pitch > 89.0f)
      pitch = 89.0f;
   if (pitch < -89.0f)
      pitch = -89.0f;

   glm::vec3 direction;
   direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   direction.y = sin(glm::radians(pitch));
   direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
   cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
   fov -= (float)yoffset;
   if (fov < 1.0f)
      fov = 1.0f;
   if (fov > 45.0f)
      fov = 45.0f;
}

void processInput(GLFWwindow *window) {
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

   const float cameraSpeed = 2.3f * deltaTime;
   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cameraPos += cameraSpeed * cameraFront;
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cameraPos -= cameraSpeed * cameraFront;
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cameraPos -=
          glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cameraPos +=
          glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main(int argc, char **argv) {
   // Initialize GLFW for use with OpenGL 4.5
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // Create window
   GLFWwindow *window =
       glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3d.view", NULL, NULL);

   if (window == NULL) {
      debugMsg("GLFW", "Failed to create window");
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent(window);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);

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
   GLuint VAO, VBO;
   glGenVertexArrays(1, &VAO); // Vertex Array Object
   glGenBuffers(1, &VBO);      // Vertex Buffer Object

   // Bindings for rect
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

   // Data structure
   // | X Y Z | TX TY |
   // |   0   |   1   |
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                         (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                         (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   // Texture
   GLuint texture = genTexture("assets/wall.jpg");

   // Enable depth
   glEnable(GL_DEPTH_TEST);

   // GLFW window loop
   while (!glfwWindowShouldClose(window)) {
      // Event Handling
      processInput(window);

      float currentTime = glfwGetTime();
      deltaTime = currentTime - lastTime;
      lastTime = currentTime;

      // Clear window buffer
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Enable shader program
      (*pipeline).use();

      // Model transformation
      glm::mat4 model = glm::mat4(1.0f);

      // View transformation
      glm::mat4 view = glm::lookAt(cameraPos,               // Camera position
                                   cameraPos + cameraFront, // Camera target
                                   cameraUp);               // Upwards vector

      // Apply projection
      glm::mat4 projection;
      projection = glm::perspective(glm::radians(fov),
                                    (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
                                    0.1f, 100.0f);

      (*pipeline).setUniMat4f("model", glm::value_ptr(model));
      (*pipeline).setUniMat4f("view", glm::value_ptr(view));
      (*pipeline).setUniMat4f("projection", glm::value_ptr(projection));

      // Draw window
      glBindTexture(GL_TEXTURE_2D, texture);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);

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
