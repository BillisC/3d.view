//===-- model.h - Model class definition -------*- C++ -*-===//
//
// Part of the 3d.view project
// Author: BillisC
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the declaration of the Model class, which is responsible
/// for model loading and drawing
///
//===----------------------------------------------------------------------===//

#ifndef MODEL_H
#define MODEL_H

// Graphics Libraries
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

// C++ Libraries
#include <string>
#include <vector>

// Project Libraries
#include "mesh.h"
#include "shader_pipeline.h"
#include "debug.h"

class Model {
   std::vector<Texture> textures_loaded;
   std::vector<Mesh> meshes;
   std::string directory;
   bool gammaCorrection;

 public:
   Model(std::string path, bool gamma = false);
   void Draw(ShaderPipeline &shaderPipeline);

 private:
   /// --- Model Processing ---
   void loadModel(std::string path);
   void processNode(aiNode *node, const aiScene *scene);
   Mesh processMesh(aiMesh *mesh, const aiScene *scene);

   /// --- Texture Handling ---
   std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                             aiTextureType type,
                                             std::string typeName);
   GLuint TextureFromFile(const char *path, const std::string &directory,
                          bool gamma = false);
};

#endif