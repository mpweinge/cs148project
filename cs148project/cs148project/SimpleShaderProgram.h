// SimpleShaderProgram.h
#ifndef __SIMPLESHADERPROGRAM_H__
#define __SIMPLESHADERPROGRAM_H__

/* Include-order dependency!
 *
 * GLEW must be included before the standard GL.h header.
 */


//#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <OpenGL/glext.h>

#include <GL/glu.h>
#include <string>
#include <vector>
#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>
#include "constants.h"
#include <iostream>

/**
 * SimpleShaderProgram - class to use GLSL programs.
 * Call LoadVertexShader and UploadFragmentShader
 * to add shaders to the program. Call Bind() to begin
 * using the shader and UnBind() to stop using it.
 */

class SimpleShaderProgram {
public:
  SimpleShaderProgram() {
    if(GLEW_VERSION_2_0)
      programid = glCreateProgram();
#ifndef __APPLE__
    else
      programid = glCreateProgramObjectARB();
#endif
  }
  ~SimpleShaderProgram() {
    if(GLEW_VERSION_2_0)
      glDeleteProgram(programid);
#ifndef __APPLE__
    else
      glDeleteObjectARB(programid);
#endif
  }
  
  // OpenGL program object id.
  unsigned int programid;
  
  //
  //    You need to add at least one vertex shader and one fragment shader. You can add multiple shaders
  //    of each type as long as only one of them has a main() function.
  //
  
  void ShaderCreationRoutine(const std::string& filename, GLenum type)
  {
    std::ifstream in(filename.c_str());
    if(!in) {
      fprintf(stderr, "Failed to open shader file '%s'\n", filename.c_str());
      assert(false);
      return;
    }
    std::stringstream ss;
    ss << in.rdbuf();
    
    std::string str = ss.str();
    const char* ptr = str.c_str();
    
    // Buffer for error messages
    static const int kBufferSize = 1024;
    char buffer[1024];
    
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &ptr, NULL);
    glCompileShader(shader);
    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if(result != GL_TRUE) {
      GLsizei length = 0;
      glGetShaderInfoLog(shader, kBufferSize-1,
                         &length, buffer);
      fprintf(stderr, "%s: GLSL error\n%s\n", filename.c_str(), buffer);
      assert(false);
    }
    glAttachShader(programid, shader);
    glLinkProgram(programid);
  }
  
  void LoadVertexShader(const std::string& filename) {
    ShaderCreationRoutine(filename, GL_VERTEX_SHADER);
  }
  
  void LoadTesselationShaders(const std::string tessControlShader, const std::string tessEvalShader, const std::string geometryShader)
  {
    ShaderCreationRoutine(tessControlShader, GL_TESS_CONTROL_SHADER);
    ShaderCreationRoutine(tessEvalShader, GL_TESS_EVALUATION_SHADER);
    ShaderCreationRoutine(geometryShader, GL_GEOMETRY_SHADER);
  }
  
  void LoadFragmentShader(const std::string& filename) {
    ShaderCreationRoutine(filename, GL_FRAGMENT_SHADER);
  }
  
  void Bind() {
    if(GLEW_VERSION_2_0)
      glUseProgram(programid);
#ifndef __APPLE__
    else
      glUseProgramObjectARB(programid);
#endif
    
    for (unsigned int i = 0; i < textures.size(); i++) {
      if (GLEW_VERSION_2_0)
        glUniform1i(textures[i].location, textures[i].tex_id);
#ifndef __APPLE__
      else
        glUniform1iARB(textures[i].location, textures[i].tex_id);
#endif
    }
  }
  void UnBind() {
    if(GLEW_VERSION_2_0)
      glUseProgram(0);
    else
      glUseProgramObjectARB(0);
  }
  
  //
  // Set a uniform global parameter of the program by name.
  //
  void SetTexture(const std::string& name, int tex_index) {
    GLint location = glGetUniformLocation(programid, name.c_str());
    UnboundTexture tex;
    tex.location = location;
    tex.tex_id   = tex_index;
    textures.push_back(tex);
  }
  
  void SetUniform(const std::string& name, float value) {
    GLint location = GetUniformLocation(name);
      if (location == -1) {
          std::cout << "Failed to set uniform: " + name + ".\n";
          return;
      }
    if(GLEW_VERSION_2_0) {
      glUniform1f(location, value);
    }
    else {
      glUniform1fARB(location, value);
    }
  }
  void SetUniform(const std::string& name, float v0, float v1) {
    GLint location = GetUniformLocation(name);
    if (location == -1) return;
    if(GLEW_VERSION_2_0) {
      glUniform2f(location, v0, v1);
    }
    else {
      glUniform2fARB(location, v0, v1);
    }
  }
  void SetUniform(const std::string& name, int status) {
    GLint location = GetUniformLocation(name);
    if (location == -1) return;
    if(GLEW_VERSION_2_0) {
      glUniform1i(location, status);
    }
    else {
      glUniform1iARB(location, status);
    }
  }
  void SetUniform(const std::string& name, float v0, float v1, float v2) {
    GLint location = GetUniformLocation(name);
    if (location == -1) return;
    if(GLEW_VERSION_2_0) {
      glUniform3f(location, v0, v1, v2);
    }
    else {
      glUniform3fARB(location, v0, v1, v2);
    }
  }
  void SetUniform(const std::string& name, float v0, float v1, float v2, float v3) {
    GLint location = GetUniformLocation(name);
    if (location == -1) return;
    if(GLEW_VERSION_2_0) {
      glUniform4f(location, v0, v1, v2, v3);
    }
    else {
      glUniform4fARB(location, v0, v1, v2, v3);
    }
  }
  
  void SetUniformMatrix4fv(const std::string&name, float * vector)
  {
    GLint location = GetUniformLocation(name);
    if (location == -1) {
      std::cout << "Failed to set uniform: " + name + ".\n";
      return;
    }
    glUniformMatrix4fv(location, 1, 0, vector);
  }
  
private:
  //
  // Helper routine - get the location for a uniform shader parameter.
  //
  GLint GetUniformLocation(const std::string& name) {
    if(GLEW_VERSION_2_0) {
      return glGetUniformLocation(programid, name.c_str());
    }
#ifndef __APPLE__
    else {
      return glGetUniformLocationARB(programid, name.c_str());
    }
#endif
  }
  
  struct UnboundTexture {
    GLint location;
    GLint tex_id;
  };
  
  std::vector<UnboundTexture> textures;
};


#endif //__SIMPLESHADERPROGRAM_H__




