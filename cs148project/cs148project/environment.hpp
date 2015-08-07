//
//  environment.hpp
//  cs148project
//
//  Created by Joe Polin on 8/4/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#ifndef cs148project_environment_hpp
#define cs148project_environment_hpp

#include <string>
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "SimpleImage.h"
#include "objMesh.hpp"

class environment {
public:
   // Ground plane stuff (all params except height should be positive)
  void initializeGroundplane(float width, float ahead, float behind, float height, std::string textureFile = "");
   void drawGroundplane();
   // Background
 //  void initializeBackground();
 //  void drawBackground();
  
private:
  GLuint groundPlaneVerts = 0;
  // Params for ground plane
  bool groundInitialized = false;
  // Textures for ground plane
  GLuint groundTex;
  SimpleImage groundImg;
  bool useTexture;
  
  
};


#endif
