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
  setAcceleration(0.05f * (glm::vec3(0.0, -9.81, 0.0)));
  
  // For cylinder.obj
  collisionSegmentA = glm::vec4(0.0, 0.0, 0.7, 1.0);
  collisionSegmentB = glm::vec4(0.0, 0.0, -0.1, 1.0);
}

float projectile::getRadius() {
  return mesh.radius;
}

void projectile::draw(glm::mat4 viewMat, glm::mat4 projMat){
  
  // Keep the orientation such that it stays along the current velocity direction
  if(glm::length(velocity) > 0){
    float yawDes = glm::atan(velocity[0] / velocity[2]);
    float pitchDes = glm::atan(velocity[1] / velocity[2]);
    setOrientationEuler(glm::vec3(-pitchDes, yawDes, 0.0));
  }
  
  updateState();
  
 
#ifdef DEBUG_TESS
  std::cout << "X: " << position.x << "Y: " << position.y <<"Z: " << position.z << std::endl;
#endif
  
  pshader->Bind();
  glm::mat4 modelView = viewMat * getModelMat();
  pshader->SetUniformMatrix4fv("Modelview", glm::value_ptr(modelView));
  pshader->SetUniformMatrix4fv("Projection", glm::value_ptr(projMat));
  mesh.draw();
  pshader->UnBind();
}