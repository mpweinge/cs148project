//
//  collisionCheck.h
//  cs148project
//
//  Created by Joe Polin on 8/10/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#ifndef __cs148project__collisionCheck__
#define __cs148project__collisionCheck__

#include <stdio.h>
#include "movingObjectBase.hpp"
#include "projectile.hpp"
#include "target.hpp"

bool checkCollision(projectile *p, target *t, glm::vec3 &intersectPoint);
bool sphereCollision(movingObjectBase *a, movingObjectBase *b);
bool lineFaceCollision(projectile *p, target *t, glm::vec3 &intersectPoint);

#endif /* defined(__cs148project__collisionCheck__) */
