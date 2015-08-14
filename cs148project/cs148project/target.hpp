//
//  target.h
//  cs148project
//
//  Created by Joe Polin on 8/7/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#ifndef __cs148project__target__
#define __cs148project__target__

#include <stdio.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "SimpleShaderProgram.h"
#include "movingObjectBase.hpp"
#include "objMesh.hpp"

class target : public movingObjectBase {
public:
  target(SimpleShaderProgram *projShader, std::string objName, std::string texName = "");
  void draw(glm::mat4 viewMat, glm::mat4 projMat);
  void loadTraj(std::vector<glm::vec3> &traj, float trajSpeed);
  virtual float getRadius();
  // For collision checking, just idealize as a rectangular face
  // which is defined by opposite corners (object coordinates)
  glm::vec4 collisionCorner1;
  glm::vec4 collisionCorner2;
  glm::vec3 collisionLocation;
  glm::vec3 explodeOffset = glm::vec3(0.0, 0.0, 0.5);
  
  bool deleteMe = false;
  const float postCollisionLife = 2e6; // microseconds
  
private:
  objMesh mesh;
  SimpleShaderProgram *tshader;
  // Trajectory
  void updateVelocity();
  std::vector<glm::vec3> trajectory;
  float speed;
  float segmentLength = 0;
  float segmentProgress = 0;
  size_t segmentIndex = 0;
  size_t nSegments = 0;
};


#endif /* defined(__cs148project__target__) */
