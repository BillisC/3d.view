#ifndef SHADER_H
#define SHADER_H

// Graphics Libraries
#include <glad/glad.h>

// C++ Libraries
#include <string>
#include <sstream>
#include <fstream>

// Project Libraries
#include "debug.h"

struct ShaderPaths {
   std::string vertexPath;
   std::string fragmentPath;
};

class ShaderPipeline {
   GLuint shaderProgram;

   // Shaders
   GLuint vertexShader;
   GLuint fragmentShader;

 public:
   ShaderPipeline(ShaderPaths paths);
   ~ShaderPipeline();

   void use() { glUseProgram(shaderProgram); }
   void setVec3(const std::string &name, const GLfloat *value) const;
   void setMat4(const std::string &name, const GLfloat *value) const;

 private:
   GLuint genShader(GLenum type, std::string file);
   GLuint genProgram();
};

#endif
