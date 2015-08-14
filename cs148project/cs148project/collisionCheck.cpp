//
//  collisionCheck.cpp
//  cs148project
//
//  Created by Joe Polin on 8/10/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#include "collisionCheck.hpp"

bool checkCollision(projectile *p, target *t, glm::vec3 &intersectPoint){
  // Fast check
  if (!sphereCollision(p, t)){
    return false;
  }

  // Longer check
  return lineFaceCollision(p, t, intersectPoint);
}

bool lineFaceCollision(projectile *p, target *t, glm::vec3 &intersectPoint){
  glm::mat4x4 targetModelMat(t->modelMat);
  // Get the face coordinates in world space
  glm::vec3 faceC1 = glm::vec3(targetModelMat * t->collisionCorner1);
  glm::vec3 faceC2 = glm::vec3(targetModelMat * t->collisionCorner2);
  // Get a third conrner--assume perp. to XZ plane
  glm::vec3 faceC3 = glm::vec3(faceC1[0], faceC2[1], faceC1[2]);
  // Get plane normal
  glm::vec3 faceNormal = glm::cross(faceC1-faceC3, faceC2-faceC3);
  // Get projectile coordinates
  glm::mat4x4 projModelMat(p->modelMat);
  glm::vec3 projE1 = glm::vec3(projModelMat * p->collisionSegmentA);
  glm::vec3 projE2 = glm::vec3(projModelMat * p->collisionSegmentB);
  glm::vec3 projDir = projE2 - projE1;
  // Check for intersection in first half of face
  float intersectionDistance;
  bool collide = glm::intersectRayPlane(projE1, projDir, faceC1, faceNormal, intersectionDistance);

  // If we're past it
  if (!collide){
    return false;
  }
  
  // if we're not close enough
  if (glm::length(projE2 - projE1) < intersectionDistance){
    return false;
  }
  
  // See if intersection points within bounds
  intersectPoint = intersectionDistance * projDir + projE1;
  return glm::all(glm::lessThanEqual((faceC1 - intersectPoint) * (faceC2 - intersectPoint), glm::vec3(0.0)));
}


// True if with radii of each other
bool sphereCollision(movingObjectBase *a, movingObjectBase *b){
  float cdist = glm::length(a->getPosition() - b->getPosition());
  return cdist < (a->getRadius() + b->getRadius());
}