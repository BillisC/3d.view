#include "light_source.h"

LightSource::LightSource(glm::vec3 Position) {
   this->Position = Position;

   // Vertices for a cube
   indices = {// Top
              2, 6, 7, 2, 3, 7,
              // Bottom
              0, 4, 5, 0, 1, 5,
              // Left
              0, 2, 6, 0, 4, 6,
              // Right
              1, 3, 7, 1, 5, 7,
              // Front
              0, 2, 3, 0, 1, 3,
              // Back
              4, 6, 7, 4, 5, 7};

   vertices = {-1, -1, 0.5,  1, -1, 0.5,  -1, 1, 0.5,  1, 1, 0.5,
               -1, -1, -0.5, 1, -1, -0.5, -1, 1, -0.5, 1, 1, -0.5};

   setup();
}

void LightSource::setup() {
   // Generate buffers
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   // Bindings
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                &vertices[0], GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                &indices[0], GL_STATIC_DRAW);

   // Structure
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                         (void *)0);
   glEnableVertexAttribArray(0);
}

void LightSource::Draw() {
   glBindVertexArray(VAO);
   glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()),
                  GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);
}