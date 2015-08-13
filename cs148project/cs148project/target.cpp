//
//  target.cpp
//  cs148project
//
//  Created by Joe Polin on 8/7/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#include "target.hpp"

target::target(SimpleShaderProgram *targShader, std::string objName, std::string texName){
  tshader = targShader;
  
  // Load mesh
  mesh.init(objName, tshader->programid, texName);
  
  // Set state to 0's
  init();

}



float target::getRadius() {
  return mesh.radius;
}

void target::loadTraj(std::vector<glm::vec3> &traj, float trajSpeed){
  // Store
  trajectory = traj;
  speed = trajSpeed;
  segmentIndex = 0;
  nSegments = trajectory.size() - 1;
  
  // Set initial position
  if (!trajectory.empty())
    setPosition(trajectory[0]);
  
  // Set initial vel
  glm::vec3 desVel;
  if (nSegments > 0)
    desVel = speed * glm::normalize(trajectory[1] - trajectory[0]);
  else
    desVel = glm::vec3(0.0f);
  setVelocity(desVel);
}
  

void target::updateVelocity(){
  
  // Check if we have overshot goal
  float prog = glm::length(position - trajectory[segmentIndex]);
  float segLength = glm::length(trajectory[segmentIndex + 1] - trajectory[segmentIndex]);
  if ( prog > segLength){
    // Update segment (wrap)
    segmentIndex = segmentIndex == nSegments-1 ? 0 : segmentIndex + 1;
    glm::vec3 a = trajectory[segmentIndex];
    glm::vec3 b = trajectory[segmentIndex + 1];
    setVelocity(speed * glm::normalize(b - a));
  }
  
  // For wooden target
  collisionCorner1 = glm::vec4(0.0, 0.00, 0.05, 1.0);
  collisionCorner2 = glm::vec4(1.0, 0.5, 0.05, 1.0);
  
}

void target::draw(glm::mat4 viewMat, glm::mat4 projMat){
  updateVelocity();
  updateState();
    
  tshader->Bind();
  glm::mat4 modelView = viewMat * getModelMat();
    
    glm::mat4 model = getModelMat();
    
    glm::vec4 projVec = model * glm::vec4(mesh.vertices[0], mesh.vertices[1], mesh.vertices[2], 1.0);
  
#ifdef DEBUG_TESS
    std::cout << "PROJ X: " << projVec.x << " Y: " << projVec.y << " Z: " << projVec.z << std::endl;
    std::cout << "TOUCH X: " << collisionLocation.x << " Y: " << collisionLocation.y << " Z: " << collisionLocation.z << std::endl;
    std::cout << "Distance: " << glm::distance(projVec, glm::vec4(collisionLocation, 1.0)) << std::endl;
#endif
  
  tshader->SetUniformMatrix4fv("Model", glm::value_ptr(model));
  tshader->SetUniformMatrix4fv("Modelview", glm::value_ptr(modelView));
  tshader->SetUniformMatrix4fv("Projection", glm::value_ptr(projMat));
  // The -0.5 is to get the point of impact in the middle of the cube
  glm::vec3 explodeLocation = collisionLocation + explodeOffset;
  tshader->SetUniform("touchLocation", explodeLocation.x, explodeLocation.y, explodeLocation.z);
  
  tshader->SetUniform("timeS", getElapsedTimerTime() / 1e6);
  mesh.drawTesselation();
  
  // Check if it's been long enough since the explosion
  if (getElapsedTimerTime() > postCollisionLife)
    deleteMe = true;
    
  //When we have tesselation here, we want to call GL_PATCHES instead of GL_TRIANGLES
  tshader->UnBind();
}