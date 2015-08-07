//
//  projectile.cpp
//  cs148project
//
//  Created by Joe Polin on 8/7/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#include "projectile.hpp"

projectile::projectile(SimpleShaderProgram *projShader, std::string objName, std::string texName){
  pshader = projShader;
  
  // Load mesh
  mesh.init(objName, pshader->programid, texName);
  
  // Set state to 0's
  init();
  
  // Set gravity
  setAcceleration(0.05f * (glm::vec3(0.0, 9.81, 0.0)));
}

void projectile::draw(glm::mat4 viewMat, glm::mat4 projMat){
  
  updateState();
  
  pshader->Bind();
  glm::mat4 modelView = viewMat * getModelMat();
  pshader->SetUniformMatrix4fv("Modelview", glm::value_ptr(modelView));
  pshader->SetUniformMatrix4fv("Projection", glm::value_ptr(projMat));
  mesh.draw();
  pshader->UnBind();
}