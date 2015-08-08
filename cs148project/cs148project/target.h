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
