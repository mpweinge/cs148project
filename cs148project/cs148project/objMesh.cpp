/*
 * objMesh.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: jpolin
 */

#include "objMesh.hpp"

void objMesh::init(std::string objFile, GLuint shaderProg, std::string texFile){
  
  // Initiaize flags and pointers
  vertices = NULL;
  normals = NULL;
  textureCoords = NULL;
  
  // Parse obj file
  parseFile(objFile);
  
  // Store to vertex arrays for opengl
  // Vertex attribute object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // Positions
  GLuint pos;
  glGenBuffers(1, &pos);
  glBindBuffer(GL_ARRAY_BUFFER, pos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nVertices, vertices, GL_STATIC_DRAW);
  GLuint posIdx = glGetAttribLocation(shaderProg, "Position");
  glVertexAttribPointer(posIdx, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(posIdx);
  
  // Normals
  GLuint norm;
  glGenBuffers(1, &norm);
  glBindBuffer(GL_ARRAY_BUFFER, norm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nVertices, normals, GL_STATIC_DRAW);
  GLuint normIdx = glGetAttribLocation(shaderProg, "Normal");
  glVertexAttribPointer(normIdx, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(normIdx);
  
  if (!texFile.empty()){
    // Texture coordinates
    GLuint texCoord;
    glGenBuffers(1, &texCoord);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * nVertices, textureCoords, GL_STATIC_DRAW);
    GLuint texCoordIdx = glGetAttribLocation(shaderProg, "Texcoord");
    glVertexAttribPointer(texCoordIdx, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
    glEnableVertexAttribArray(texCoordIdx);
    
    // Bind texture
    SimpleImage texImg(texFile);
    
    std::cout << "Texture #:" << vao << std::endl;
    glGenTextures(1, &textureIndex);
    glBindTexture(GL_TEXTURE_2D, textureIndex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImg.width(),
                 texImg.height(), 0, GL_RGB, GL_FLOAT, texImg.data());
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
  }
  
  // Un-bind the vao so that no one else writes over it
  glBindVertexArray(0);
  
}

void objMesh::draw(){
  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_2D, textureIndex);
  glDrawArrays(GL_TRIANGLES, 0, nVertices);
  glBindVertexArray(0);
}

void objMesh::drawTesselation()
{
  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_2D, textureIndex);
  glPatchParameteri(GL_PATCH_VERTICES, 3);
  glDrawArrays(GL_PATCHES, 0, nVertices);
  glBindVertexArray(0);
}

objMesh::~objMesh(){
  // Free allocated memory
  delete vertices;
  delete normals;
  delete textureCoords;
}

// Cross product
vector3 objMesh::cross(vector3 a, vector3 b){
  vector3 c;
  c.x = a.y * b.z - a.z * b.y;
  c.y = a.z * b.x - a.x * b.z;
  c.z = a.x * b.y - a.y * b.x;
  // Normalize
  float mag = sqrt(c.x*c.x + c.y*c.y + c.z*c.z);
  c.x *= 1/mag;
  c.y *= 1/mag;
  c.z *= 1/mag;
  
  return c;
}

bool objMesh::parseFile(std::string fname){
  std::ifstream f;
  f.open(fname.c_str());
  // Check if it opened
  if (!f.is_open()){
    std::cout << "\nCould not open file " << fname << ".\n";
    return false;
  }
  else {
    std::cout << "Opening " << fname << std::endl;
  }
  
  // Temporary containers for structs
  std::vector<face> faces;
  std::vector<vertex> verts;
  std::vector<vector3> norms;
  std::vector<texture> texts;
  
  // Parse
  std::string line;
  while (getline(f,line) )
  {
    float x, y, z;
    char type[3];
    int n = sscanf(line.c_str(), "%s %f %f %f", type, &x, &y, &z);
    
    // Comment/blank
    if (strcmp(type, "#") == 0 || n <= 0)
    		continue;
    // Vertex
    else if (strcmp(type, "v") == 0 && n == 4){
    		vertex v(x, y, z);
    		verts.push_back(v);
    }
    //Vertex normal
    else if (strcmp(type, "vn")==0){
      // normalize
      float mag = sqrtf(x*x + y*y + z*z);
      vector3 vn(x/mag, y/mag, z/mag);
      norms.push_back(vn);
    }
    // Texture
    else if (strcmp(type, "vt") == 0 && (n == 3 || n == 4)){
    		texture t = {n == 4, x, y, z};
    		texts.push_back(t);
    }
    // Face
    else if (strcmp(type, "f") == 0){
      bool normalIncluded = false;
    		// Get textures
      float tx = 0, ty = 0, tz = 0, nx = 0, ny = 0, nz = 0;
    		// Slashes used
    		if (n == 2) {
          n = sscanf(line.c_str(), "%s %f/%f %f/%f %f/%f", type, &x, &tx, &y, &ty, &z, &tz);
          // Check if double slashes used
          if (n < 7) {
            n = sscanf(line.c_str(), "%s %f//%f %f//%f %f//%f", type, &x, &tx, &y, &ty, &z, &tz);
            // Check if normals included
            if (n < 7) {
              n = sscanf(line.c_str(), "%s %f/%f/%f %f/%f/%f %f/%f/%f",
                         type, &x, &tx, &nx, &y, &ty, &ny, &z, &tz, &nz);
              if (n != 10) {
                std::cout << "Could not parse: " << line << std::endl;
              }
              normalIncluded = true;
            }
          }
        }
    		// Create face
    		face f;
    		// Add vertices
    		assert(!(x > verts.size() || y > verts.size() || z >verts.size()));
    		f.vert[0] = verts[x - 1];
    		f.vert[1] = verts[y - 1];
    		f.vert[2] = verts[z - 1];
      // Add normals
      if (!norms.empty() && normalIncluded){
        f.norm[0] = norms[nx - 1];
        f.norm[1] = norms[ny - 1];
        f.norm[2] = norms[nz - 1];        }
      else {
        vector3 edge_1 = f.vert[1] - f.vert[0];
        vector3 edge_2 = f.vert[2] - f.vert[0];
        vector3 n = cross(edge_1, edge_2);
        for (size_t i = 0; i < 3; i++) f.norm[i] = n;
      }
    		// Textures (?)
    		f.textured = !(n == 4);
    		if (f.textured){
          assert(!(tx > texts.size() || ty > texts.size() || tz >texts.size()));
          f.text[0] = texts[tx - 1];
          f.text[1] = texts[ty - 1];
          f.text[2] = texts[tz - 1];
        }
    		faces.push_back(f);
    }
    else {
      std::cout << "Could not parse: " << line << std::endl;
    }
    
  }
  
  f.close();
  
  nVertices = faces.size() * 3.0;
  
  // Store to float arrays
  vertices = new float[nVertices * 3];
  normals = new float[nVertices * 3];
  textureCoords = new float[nVertices * 2];
  
  size_t array_counter = 0;
  size_t tex_counter = 0;
  for(std::vector<face>::iterator f = faces.begin(); f != faces.end(); f++){
    for (size_t vert = 0; vert < 3; vert++){
      float normSquared = 0;
      for (size_t d = 0; d < 3; d++){
        // Update normSquare
        normSquared += powf(f->vert[vert][d], 2.0);
        vertices[array_counter] = f->vert[vert][d];
        normals[array_counter] = f->norm[vert][d];
        array_counter++;
        if (f->textured && d < 2){
          textureCoords[tex_counter] = f->text[vert][d];
          tex_counter++;
        }
#ifdef DEBUG_TESS
        std::cout << "NORMAL x: " << normals[array_counter - 3] << " Y: ";
        std::cout << normals[array_counter - 2] << " Z: ";
        std::cout << normals[array_counter - 1] << std::endl;
#endif
      }
      // Update radius
      if (normSquared > radius)
        radius = normSquared;
    }
  }
  // Radius is currently squared
  radius = powf(radius, 0.5);
  
  return true;
  
}
