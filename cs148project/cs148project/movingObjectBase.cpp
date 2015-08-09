//
//  movingObjectBase.cpp
//  cs148project
//
//  Created by Joe Polin on 8/7/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#include <stdio.h>
#include "movingObjectBase.hpp"

void movingObjectBase::init(){
  glm::vec3 z = glm::vec3(0.0, 0.0, 0.0);
  setPosition(z);
  setVelocity(z);
  setAcceleration(z);
  setOrientationEuler(z);
  setOmega(z);
  setAlpha(z);
  modelMat = glm::mat4(1.0f);
}

// Taken from: http://stackoverflow.com/questions/14391327
typedef std::chrono::high_resolution_clock chronosClock;
typedef std::chrono::duration<float, std::micro> duration;
float getTimeMicroseconds(){
  static chronosClock::time_point start = chronosClock::now();
  duration elapsed = chronosClock::now() - start;
  return elapsed.count();
}

void movingObjectBase::updateState(){
  
  // Get t (in seconds)
  float t = getTimeMicroseconds();
  
  // If no t_last to use
  if (t_last == -1.0)
    t_last = t;
  
  float dt = (t - t_last) / 1e6;
  
  assert(dt >= 0.0);
  
  // Update pos/vel
  position += dt * velocity;
  velocity += dt * acceleration;
  
  // TODO: Update orientation correctly. For now, treat omega/alpha as derivatives of euler
  eulerAngles += dt * omega;
  omega += dt * alpha;
  
  // Update the modelview matrix (not sure about the euler matrix)
  modelMat = glm::translate(-position) * glm::yawPitchRoll(eulerAngles.y, eulerAngles.x, eulerAngles.z);
  
  // Update last time
  t_last = t;
  
}

// True if with radii of each other
bool sphereCollision(movingObjectBase *a, movingObjectBase *b){
  float cdist = glm::length(a->getPosition() - b->getPosition());
  return cdist < (a->getRadius() + b->getRadius());
}