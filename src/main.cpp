#include "glm/fwd.hpp"
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
#include "camera.h"
#include "debug.h"

void debugMsg(std::string source, std::string error) {
   std::cout << "[DEBUG] " << source << ": " << error << "\n" << std::flush;
}

// Rectangle
/* clang-format off */
float rect[] = {
	//X     Y      Z     TX    TY     NX     NY     NZ
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f
};
/* clang-format on */

// Window defines
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

// Camera parameters
Camera camera;
float lastX = float(WINDOW_WIDTH) / 2;
float lastY = float(WINDOW_HEIGHT) / 2;

bool firstMouse = false;

// Delta time
float deltaTime, lastTime = 0.0f;

// Light parameters
glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

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

   camera.setDirection(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
   camera.setZoom((float)yoffset);
}

void processInput(GLFWwindow *window) {
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.moveForward(deltaTime);
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.moveBackward(deltaTime);
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.moveLeft(deltaTime);
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.moveRight(deltaTime);
}

int main(int argc, char **argv) {
   // --- Initialize GLFW for use with OpenGL 4.5 ---
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

   // --- Create shader programs ---
   ShaderPaths cubePaths = {"src/shaders/cubeShader.vert",
                            "src/shaders/cubeShader.frag"};
   ShaderPipeline *cubePipeline = new ShaderPipeline(cubePaths);

   ShaderPaths lightPaths = {"src/shaders/lightShader.vert",
                             "src/shaders/lightShader.frag"};
   ShaderPipeline *lightPipeline = new ShaderPipeline(lightPaths);

   // --- Generate model buffer object ---
   GLuint VBO;
   glGenBuffers(1, &VBO); // Vertex Buffer Object

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

   // --- Bindings for model attributes ---
   GLuint cubeVAO;
   glGenVertexArrays(1, &cubeVAO); // Vertex Array Object

   glBindVertexArray(cubeVAO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                         (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                         (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                         (void *)(5 * sizeof(float)));
   glEnableVertexAttribArray(2);

   // --- Bindings for light attributes ---
   GLuint lightVAO;
   glGenVertexArrays(1, &lightVAO); // Vertex Array Object

   glBindVertexArray(lightVAO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                         (void *)0);
   glEnableVertexAttribArray(0);

   // --- Texture ---
   GLuint texture = genTexture("assets/wall.jpg");

   // --- Enable depth ---
   glEnable(GL_DEPTH_TEST);

   // --- GLFW window loop ---
   while (!glfwWindowShouldClose(window)) {
      // Event Handling
      processInput(window);

      float currentTime = glfwGetTime();
      deltaTime = currentTime - lastTime;
      lastTime = currentTime;

      // Clear window buffer
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Light updates
      lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
      lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

      // Enable shader program
      (*cubePipeline).use();
      (*cubePipeline)
          .setVec3("baseColor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
      (*cubePipeline)
          .setVec3("lightColor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
      (*cubePipeline)
          .setVec3("lightPos", glm::value_ptr(lightPos));

      // Model transformations
      glm::mat4 model = glm::mat4(1.0f);
      (*cubePipeline).setMat4("model", glm::value_ptr(model));
      (*cubePipeline).setMat4("view", glm::value_ptr(camera.getView()));
      (*cubePipeline)
          .setMat4("projection", glm::value_ptr(camera.getProjection(
                                     (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT,
                                     0.1f, 100.0f)));
      (*cubePipeline).setVec3("cameraPos", glm::value_ptr(camera.getPos()));

      // Draw object
      glBindTexture(GL_TEXTURE_2D, texture);
      glBindVertexArray(cubeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // Same but for light
      (*lightPipeline).use();

      model = glm::mat4(1.0f);
      model = glm::translate(model, lightPos);
      model = glm::scale(model, glm::vec3(0.4f));
      (*lightPipeline).setMat4("model", glm::value_ptr(model));
      (*lightPipeline).setMat4("view", glm::value_ptr(camera.getView()));
      (*lightPipeline)
          .setMat4("projection", glm::value_ptr(camera.getProjection(
                                     (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT,
                                     0.1f, 100.0f)));
      glBindVertexArray(lightVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   // GL deallocation
   glDeleteVertexArrays(1, &cubeVAO);
   glDeleteVertexArrays(1, &lightVAO);
   glDeleteBuffers(1, &VBO);
   delete cubePipeline;
   delete lightPipeline;

   // Program termination
   glfwTerminate();
   return 0;
}
