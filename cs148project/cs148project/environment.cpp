//
//  environment.cpp
//  cs148project
//
//  Created by Joe Polin on 8/4/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#include <stdio.h>
#include "environment.hpp"

inline float abs(float x) {return x > 0 ? x : -x;}

void environment::initializeGroundplane(float width, float ahead, float behind, float height, std::string textureFile){
  groundInitialized = true;
  useTexture = !textureFile.empty();
  
  // Vertex attribute object
  glGenVertexArrays(1, &groundPlaneVerts);
  glBindVertexArray(groundPlaneVerts);
  
  // Going to make 2 triangles that form a rectangle:
  float groundCorners[18];
  // X coordinates
  groundCorners[0] = -width / 2;
  groundCorners[3] = width / 2;
  groundCorners[6] = width / 2;
  
  groundCorners[9] = -width / 2;
  groundCorners[12] = groundCorners[0];
  groundCorners[15] = groundCorners[6];
  // Y
  for (int i = 1; i < 18; i += 3) groundCorners[i] = height;
  // Z
  groundCorners[2] = behind;
  groundCorners[5] = behind;
  groundCorners[8] = -ahead;
  
  groundCorners[11] = -ahead;
  groundCorners[14] = groundCorners[2];
  groundCorners[17] = groundCorners[8];
  
  // Create Vertex array object for corner positions  
  GLuint cornerVBO;
  glGenBuffers(1, &cornerVBO);
  glBindBuffer(GL_ARRAY_BUFFER, cornerVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(groundCorners), groundCorners, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);
  
  // Texture stuff
  if (useTexture){
    // Load file
    std::cout << "Loading texture file from: " + textureFile << std::endl;
    groundImg.load(textureFile);
    // Bind texture
    glGenTextures(1, &groundTex);
    glBindTexture(GL_TEXTURE_2D, groundTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, groundImg.width(),
                 groundImg.height(), 0, GL_RGB, GL_FLOAT, groundImg.data());
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    // Make texture coordinates
    float groundTexCoords[12];
    float textureWidthMeters = 1.0;
    float textureHeightMeters = (float)groundImg.height() / (float)groundImg.width() * textureWidthMeters;
    float xmax = width / textureWidthMeters;
    float ymax = (ahead + behind) / textureHeightMeters;
    // xvals:
    groundTexCoords[0] = 0;
    groundTexCoords[2] = xmax;
    groundTexCoords[4] = xmax;
    
    groundTexCoords[6] = 0;
    groundTexCoords[8] = 0;
    groundTexCoords[10] = xmax;
    
    // yvals (same as -z above)
    groundTexCoords[1] = 0;
    groundTexCoords[3] = 0;
    groundTexCoords[5] = ymax;
    
    groundTexCoords[7] = ymax;
    groundTexCoords[9] = 0;
    groundTexCoords[11] = ymax;
    
    // Put coordinates into buffer array
    GLuint texVBO;
    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundTexCoords), groundTexCoords, GL_STATIC_DRAW);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(1);
  }
  
  // Release buffer
  glBindVertexArray(0);

}

void environment::drawGroundplane(){
  if (!groundInitialized) {
    std::cout << "Need to initialize ground plane before drawing it.\n";
    return;
  }
  // Bind, draw, un-bind
  glBindVertexArray(groundPlaneVerts);
  //glDrawArrays(GL_PATCHES, 0, 6);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}