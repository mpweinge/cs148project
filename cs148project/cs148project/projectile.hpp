//
//  projectile.h
//  cs148project
//
//  Created by Joe Polin on 8/7/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#ifndef __cs148project__projectile__
#define __cs148project__projectile__

#include <stdio.h>
#include <string>

#include "SimpleShaderProgram.h"
#include "movingObjectBase.hpp"
#include "objMesh.hpp"

class projectile : public movingObjectBase {
public:
  projectile(SimpleShaderProgram *projShader, std::string objName, std::string texName = "");
  void draw(glm::mat4 viewMat, glm::mat4 projMat);
  virtual float getRadius();
  
private:
  objMesh mesh;
  SimpleShaderProgram *pshader;
};

#endif /* defined(__cs148project__projectile__) */
