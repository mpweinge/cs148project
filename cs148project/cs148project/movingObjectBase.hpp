//
//  movingObjectBase.hpp
//  cs148project
//
//  Created by Joe Polin on 8/7/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

// Essentially manages the model matrix for a moving object

#ifndef cs148project_movingObjectBase_hpp
#define cs148project_movingObjectBase_hpp

// (Matrix) Math
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <cmath>
#include <ctgmath>
#include "glm/glm/gtx/quaternion.hpp"
#include "glm/glm/gtx/euler_angles.hpp"

#include <chrono>
#include <iostream>

class movingObjectBase {
  
public:
  // Liniear
  void setPosition(glm::vec3 pos) {position = pos;}
  void setVelocity(glm::vec3 vel) {velocity = vel;}
  void setAcceleration(glm::vec3 acc) {acceleration = acc;}
  // Orientation
  void setOrientationEuler(glm::vec3 ori) {eulerAngles = ori;}
  void setOmega(glm::vec3 om) {omega = om;}
  void setAlpha(glm::vec3 al) {alpha = al; }
  glm::vec3 getPosition() {return position; }
  glm::mat4 getModelMat() {return modelMat; }
  
protected:
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  glm::vec3 eulerAngles;
  glm::vec3 omega;
  glm::vec3 alpha;
  
  glm::mat4 modelMat;
  float t_last = -1.0;

  void updateState();
  void init();
  
};

#endif
