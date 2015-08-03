//
//  main.cpp
//  cs148project
//
//  Created by Michael Weingert on 2015-08-01.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include <cmath>
#include <iostream>

#include "SimpleShaderProgram.h"
#include "constants.h"

using namespace std;

int currX;
int currY;
float xRot=0.0;
float yRot=0.0;
bool leftButton = false;
bool middleButton = false;
float zOffs = 0;

GLuint gVAO = 0;
GLuint gVBO = 0;

static GLsizei IndexCount;
GLuint positions;
static const GLuint PositionSlot = 0;
GLuint indices;
GLuint vao;

GLFWwindow* gWindow = NULL;

SimpleShaderProgram *shader;

float Modelview[16];

float Projective[] = \
{ 1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1};

vector<Triangle> tris;
vector<Vertex> vertices;
vector<Vertex> textCoordinates;

//Compat method: gluLookAt deprecated
void compat_gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
                      GLdouble centerx, GLdouble centery, GLdouble centerz,
                      GLdouble upx, GLdouble upy, GLdouble upz)
{
  float x[3], y[3], z[3];
  float mag;
  
  /* Make rotation matrix */
  
  /* Z vector */
  z[0] = eyex - centerx;
  z[1] = eyey - centery;
  z[2] = eyez - centerz;
  mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
  if (mag) {			/* mpichler, 19950515 */
    z[0] /= mag;
    z[1] /= mag;
    z[2] /= mag;
  }
  
  /* Y vector */
  y[0] = upx;
  y[1] = upy;
  y[2] = upz;
  
  /* X vector = Y cross Z */
  x[0] = y[1] * z[2] - y[2] * z[1];
  x[1] = -y[0] * z[2] + y[2] * z[0];
  x[2] = y[0] * z[1] - y[1] * z[0];
  
  /* Recompute Y = Z cross X */
  y[0] = z[1] * x[2] - z[2] * x[1];
  y[1] = -z[0] * x[2] + z[2] * x[0];
  y[2] = z[0] * x[1] - z[1] * x[0];
  
  /* mpichler, 19950515 */
  /* cross product gives area of parallelogram, which is < 1.0 for
   * non-perpendicular unit-length vectors; so normalize x, y here
   */
  
  mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
  if (mag) {
    x[0] /= mag;
    x[1] /= mag;
    x[2] /= mag;
  }
  
  mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
  if (mag) {
    y[0] /= mag;
    y[1] /= mag;
    y[2] /= mag;
  }
  
#define M(row,col)  Modelview[col*4+row]
  M(0, 0) = x[0];
  M(0, 1) = x[1];
  M(0, 2) = x[2];
  M(0, 3) = 0.0;
  M(1, 0) = y[0];
  M(1, 1) = y[1];
  M(1, 2) = y[2];
  M(1, 3) = 0.0;
  M(2, 0) = z[0];
  M(2, 1) = z[1];
  M(2, 2) = z[2];
  M(2, 3) = 0.0;
  M(3, 0) = -eyex;
  M(3, 1) = -eyey;
  M(3, 2) = -eyez;
  M(3, 3) = 1.0;
#undef M
  Modelview[12] = -eyex;
  Modelview[13] = -eyey;
  Modelview[14] = -eyez;
}

static void CreateIcosahedron()
{
  IndexCount = sizeof(icoFaces) / sizeof(icoFaces[0]);
  
  // Create the VAO:
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // Create the VBO for positions:
  GLsizei stride = 3 * sizeof(float);
  glGenBuffers(1, &positions);
  glBindBuffer(GL_ARRAY_BUFFER, positions);
  glBufferData(GL_ARRAY_BUFFER, sizeof(icoVerts), icoVerts, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(PositionSlot);
  glVertexAttribPointer(PositionSlot, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(0));
  
  // Create the VBO for indices:
  glGenBuffers(1, &indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(icoFaces), icoFaces, GL_STATIC_DRAW);
}

void perspective(float left, float right, float bottom, float top,
                 float znear, float zfar)
{
  float temp, temp2, temp3, temp4;
  temp = 2.0 * znear;
  temp2 = right - left;
  temp3 = top - bottom;
  temp4 = zfar - znear;
  Projective[0] = temp / temp2;
  Projective[1] = 0.0;
  Projective[2] = 0.0;
  Projective[3] = 0.0;
  Projective[4] = 0.0;
  Projective[5] = temp / temp3;
  Projective[6] = 0.0;
  Projective[7] = 0.0;
  Projective[8] = (right + left) / temp2;
  Projective[9] = (top + bottom) / temp3;
  Projective[10] = (-zfar - znear) / temp4;
  Projective[11] = -1.0;
  Projective[12] = 0.0;
  Projective[13] = 0.0;
  Projective[14] = (-temp * zfar) / temp4;
  Projective[15] = 0.0;
}

// loads a triangle into the VAO global
static void LoadTriangle() {
  // make and bind the VAO
  glGenVertexArrays(1, &gVAO);
  glBindVertexArray(gVAO);
  
  // make and bind the VBO
  glGenBuffers(1, &gVBO);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO);
  
  // Put the three triangle verticies into the VBO
  GLfloat vertexData[] = {
    0.0f, 0.8f, 0.0f,
    -0.8f,-0.8f, 0.0f,
    0.8f,-0.8f, 0.0f,
  };
  //    glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  
  // connect the xyz to the "vert" attribute of the vertex shader
  glEnableVertexAttribArray(PositionSlot);
  glVertexAttribPointer(PositionSlot, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  
  // unbind the VBO and VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  ushort pindices[6];
  pindices[0] = 0;
  pindices[1] = 1;
  pindices[2] = 2;
  
  //IndexCount = sizeof(Faces) / sizeof(Faces[0]);
  
  glGenBuffers(1, &indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * IndexCount, Faces, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * 3, pindices, GL_STATIC_DRAW);
}


// draws a single frame
static void Render() {
  // clear everything
  glClearColor(0, 0, 0, 1); // black
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // bind the program (the shaders)
  shader->Bind();
  
  perspective(-1, 1, -1, 1, 1, 5);
  compat_gluLookAt(0, 0, 1, 0, 0, -2, 0, 1, 0);
  shader->SetUniformMatrix4fv("Projection", Projective);
  shader->SetUniformMatrix4fv("Modelview", Modelview);
  
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glPatchParameteri(GL_PATCH_VERTICES, 3);
  
  // bind the VAO (the triangle)
  glBindVertexArray(gVAO);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
  // draw the VAO
  glDrawElements(GL_PATCHES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  
  // unbind the VAO
  glBindVertexArray(0);
  
  // unbind the program
  glUseProgram(0);
  
  // swap the display buffers (displays what was just drawn)
  glfwSwapBuffers(gWindow);
}

void Setup()
{
  //loadTriFile(objFilename);
  
  //We are using triangles here
  //glPatchParameteri(GL_PATCH_VERTICES, 3);
  shader = new SimpleShaderProgram();
  shader->LoadVertexShader(vertexShaderPath);
  shader->LoadFragmentShader(fragmentShaderPath);
  shader->LoadTesselationShaders(tessControlShaderPath, tessEvalShaderPath, geometryShaderPath);
  
  //CreateIcosahedron();
  //glDrawArrays(GL_PATCHES, 0, 8);
}

void onMouseButton( GLFWwindow* window, int button, int action, int mods ) {
  if( button==GLFW_MOUSE_BUTTON_RIGHT )
  {
    // ... some code
    if (action == GLFW_PRESS )
    {
      
    }
  }
  else if( button==GLFW_MOUSE_BUTTON_LEFT )
  {
    if (action == GLFW_PRESS )
    {
      leftButton = true;
      
      //touchLocation
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      shader->Bind();
      
      GLdouble nearPointX, nearPointY, nearPointZ;
      screenToWorld(xpos, ypos, nearPointX, nearPointY, nearPointZ, Modelview, Projective);
      nearPointX = ((xpos / 600.0) - 0.5) * 2;
      nearPointY = ((400 - ypos) / 400.0);
      nearPointZ = 0.0;
      
      cout << "X: " << nearPointX << "Y: " << nearPointY << "Z: " << nearPointZ << endl;
      shader->SetUniform("touchLocation", nearPointX, nearPointY, nearPointZ);
    } else if (action == GLFW_RELEASE) {
      leftButton = false;
    }
  } else {
    if (action == GLFW_PRESS )
    {
      middleButton = true;
    } else if (action == GLFW_RELEASE) {
      middleButton = false;
    }
    
  }
}

void onMouseMove(int x, int y)
{
  if (leftButton)
  {
    xRot += ((float)(x-currX)) / 4.0;
    yRot += ((float)(currY-y)) / 4.0;
  }
  if (middleButton)
  {
    zOffs += ((float)(currY - y)) / 30.0;
  }
  
  currX = x;
  currY = y;
  //glutPostRedisplay();
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  onMouseMove((int)xpos, (int)ypos);
}

int main(int argc,  char * argv[]) {
  // initialise GLFW
  //glfwSetErrorCallback(OnError);
  if(!glfwInit())
    throw std::runtime_error("glfwInit failed");
  
  // open a window with GLFW
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  gWindow = glfwCreateWindow((int)600, (int)400, "OpenGL Tutorial", NULL, NULL);
  if(!gWindow)
    throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
  
  // GLFW settings
  glfwMakeContextCurrent(gWindow);
  
  // print out some info about the graphics drivers
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  
  // load vertex and fragment shaders into opengl
  Setup();
  
  LoadTriangle();
  glfwSetCursorPosCallback(gWindow, cursor_position_callback);
  glfwSetMouseButtonCallback(gWindow, onMouseButton);
  
  // run while the window is open
  while(!glfwWindowShouldClose(gWindow)){
    // process pending events
    glfwPollEvents();
    
    // draw one frame
    Render();
  }
  
  // clean up and exit
  glfwTerminate();
}