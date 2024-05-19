#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

// Graphics Libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// C++ Libraries
#include <vector>

// Project Libraries
#include "shader_pipeline.h"

class LightSource {
   // Model (Optional)
   std::vector<GLfloat> vertices;
   std::vector<GLuint> indices;
   GLuint VAO, VBO, EBO;

 public:
   glm::vec3 Position;

   // Light parameters
   glm::vec3 AmbientStrength = glm::vec3(0.2f);
   glm::vec3 SpecularStrength = glm::vec3(1.0f);
   glm::vec3 Color = glm::vec3(1.0f);

 public:
   LightSource(glm::vec3 Position);
   void Draw();

 private:
   void setup();
};

#endif