#include "shader_pipeline.h"

ShaderPipeline::ShaderPipeline(ShaderPaths paths) {
   vertexShader = genShader(GL_VERTEX_SHADER, paths.vertexPath);
   fragmentShader = genShader(GL_FRAGMENT_SHADER, paths.fragmentPath);
   shaderProgram = genProgram();
}

ShaderPipeline::~ShaderPipeline() { glDeleteProgram(shaderProgram); }

GLuint ShaderPipeline::genShader(GLenum type, std::string file) {
   // Create shader object and obtain its ID
   GLuint shader = glCreateShader(type);

   // Read shader code from file
   std::string shaderSource;
   std::ifstream shaderFile(file);

   if (shaderFile.is_open()) {
      std::stringstream sstream;
      sstream << shaderFile.rdbuf();
      shaderSource = sstream.str();
      shaderFile.close();
   } else {
      debugMsg("Shader", "Failed to read file");
   }

   const char *shaderSourcePointer = shaderSource.c_str();

   // Compile shader
   glShaderSource(shader, 1, &shaderSourcePointer, NULL);
   glCompileShader(shader);

   // Check for errors
   GLint result, infoLogLength;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
   glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
   if (!result) {
      char infoLog[infoLogLength];
      glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
      debugMsg("Shader", infoLog);
   }

   return shader;
}

GLuint ShaderPipeline::genProgram() {
   // Create shader program and obtain its ID
   GLuint program = glCreateProgram();

   // Link shaders to program
   glAttachShader(program, vertexShader);
   glAttachShader(program, fragmentShader);
   glLinkProgram(program);

   // Check for errors
   GLint result, infoLogLength;
   glGetProgramiv(program, GL_LINK_STATUS, &result);
   glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
   if (!result) {
      char infoLog[infoLogLength];
      glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
      debugMsg("Program", infoLog);
   }

   // Destroy unneeded objects
   glDetachShader(program, vertexShader);
   glDetachShader(program, fragmentShader);

   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   return program;
}

void ShaderPipeline::setVec3(const std::string &name,
                             const GLfloat *value) const {
   uint16_t uniLoc = glGetUniformLocation(this->shaderProgram, name.c_str());
   glUniform3fv(uniLoc, 1, value);
}

void ShaderPipeline::setMat4(const std::string &name,
                             const GLfloat *value) const {
   uint16_t uniLoc = glGetUniformLocation(this->shaderProgram, name.c_str());
   glUniformMatrix4fv(uniLoc, 1, GL_FALSE, value);
}

void ShaderPipeline::setInt(const std::string &name, const GLint value) const {
   uint16_t uniLoc = glGetUniformLocation(this->shaderProgram, name.c_str());
   glUniform1i(uniLoc, value);
}
