//===-- mesh.h - Mesh class definition -------*- C++ -*-===//
//
// Part of the 3d.view project
// Author: BillisC
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the declaration of the Mesh class, which is responsible
/// for storing object data such as meshes, vertexes and tectures
///
//===----------------------------------------------------------------------===//

#ifndef MESH_H
#define MESH_H

// Graphics Libraries
#include "shader_pipeline.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

// C++ Libraries
#include <string>
#include <vector>

// Project Libraries
#include "shader_pipeline.h"

struct Vertex {
   glm::vec3 Position;
   glm::vec3 Normal;
   glm::vec3 Tangent;
   glm::vec3 Bitangent;
   glm::vec2 TexCoords;
};

struct Texture {
   GLuint id;
   std::string type;
   std::string path;
};

class Mesh {
   GLuint VAO, VBO, EBO;

 public:
   Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices,
        std::vector<Texture> &textures);
   void Draw(ShaderPipeline &shaderPipeline);

   std::vector<Vertex> vertices;
   std::vector<GLuint> indices;
   std::vector<Texture> textures;

 private:
   void setup();
};

#endif