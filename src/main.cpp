#include <iostream>

// Graphics Libraries
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Project Libraries
#include "shader_pipeline.h"
#include "light_source.h"
#include "model.h"
#include "camera.h"
#include "debug.h"

void debugMsg(std::string source, std::string error) {
   std::cout << "[DEBUG] " << source << ": " << error << "\n" << std::flush;
}

// Window defines
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

// Camera parameters
Camera camera;
float lastX = float(WINDOW_WIDTH) / 2;
float lastY = float(WINDOW_HEIGHT) / 2;
float acceleration = 1.0;

bool firstMouse = false;

// Delta time
float deltaTime, lastTime = 0.0f;

// Light parameters

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

   if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      acceleration = 2.5f;
   } else {
      acceleration = 1.0f;
   }

   if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.moveForward(deltaTime * acceleration);
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.moveBackward(deltaTime * acceleration);
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.moveLeft(deltaTime * acceleration);
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.moveRight(deltaTime * acceleration);
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

   // stbi parameters
   stbi_set_flip_vertically_on_load(true);

   // --- Create shader programs ---
   ShaderPaths modelPaths = {"src/shaders/modelShader.vert",
                             "src/shaders/modelShader.frag"};
   ShaderPipeline *modelPipeline = new ShaderPipeline(modelPaths);

   ShaderPaths lightPaths = {"src/shaders/simpleShader.vert",
                             "src/shaders/simpleShader.frag"};
   ShaderPipeline *lightPipeline = new ShaderPipeline(lightPaths);

   // Create lamp
   LightSource lamp(glm::vec3(1.2f, 1.0f, 2.0f));

   // Load model
   Model loadedModel("assets/wood/wood.obj");

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
      glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Enable shader program
      (*modelPipeline).use();
      (*modelPipeline).setVec3("light.color", glm::value_ptr(lamp.Color));
      (*modelPipeline)
          .setVec3("light.ambient", glm::value_ptr(lamp.AmbientStrength));
      (*modelPipeline)
          .setVec3("light.specular", glm::value_ptr(lamp.SpecularStrength));

      (*modelPipeline).setVec3("lightPos", glm::value_ptr(lamp.Position));
      (*modelPipeline).setVec3("viewPos", glm::value_ptr(camera.Position));

      // Transformations
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      (*modelPipeline).setMat4("model", glm::value_ptr(model));
      (*modelPipeline).setMat4("view", glm::value_ptr(camera.getView()));
      (*modelPipeline)
          .setMat4("projection", glm::value_ptr(camera.getProjection(
                                     (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
                                     0.1f, 100.0f)));

      loadedModel.Draw(*modelPipeline);

      // Same but for light
      (*lightPipeline).use();
      model = glm::mat4(1.0f);
      model = glm::translate(model, lamp.Position);
      model = glm::scale(model, glm::vec3(0.2f));
      (*lightPipeline).setMat4("model", glm::value_ptr(model));
      (*lightPipeline).setMat4("view", glm::value_ptr(camera.getView()));
      (*lightPipeline)
          .setMat4("projection", glm::value_ptr(camera.getProjection(
                                     (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
                                     0.1f, 100.0f)));
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      lamp.Draw();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   // GL deallocation
   delete modelPipeline;
   delete lightPipeline;

   // Program termination
   glfwTerminate();
   return 0;
}
