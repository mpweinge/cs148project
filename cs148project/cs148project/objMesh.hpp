/*
 * objMesh.hpp
 *
 *  Created on: Jul 23, 2015
 *      Author: jpolin
 */

#ifndef OBJMESH_CPP_
#define OBJMESH_CPP_

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <assert.h>
#include <math.h>

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include "SimpleImage.h"
#include "DataStructs.hpp"



/* Usage Notes:
 - The array buffers for position, normals, and texture coordinates are expecting the shaders
   to use:
      in vec3 Position;
      in vec3 Normal;
      in vec2 Texcoord;
 - If no texture is desired, just pass "" as the texFile
 - The shaderProg is the number of the shader that should be used for this object. Before calling
   the draw function, make sure the appropriate shader is bound.
 */

class objMesh {
public:
  void init(std::string objFile, GLuint shaderProg, std::string texFile = "");
  void draw();
  void drawTesselation();
  ~objMesh();
  // Distance from 0,0,0 to farthest point
  float radius = 0.0;
  
private:
  float *vertices;
  float *normals;
  float *textureCoords;
  
  GLsizei nVertices;
  
  bool parseFile(std::string fname);
  vector3 cross(vector3 a, vector3 b);
  GLuint vao;  
  GLuint textureIndex;
  
};

#endif /* OBJMESH_CPP_ */
