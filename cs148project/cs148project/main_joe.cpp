//
//  main.cpp
//  cs148project
//
//  Created by Joe Polin on 8/2/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

// Flag(s)
#define GLEW_VERSION_2_0 1

// Standard libs
#include <stdio.h>
#include <iostream>
#include <string>

// Window and user input
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

// openGL
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

// (Matrix) Math
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <cmath>
#include <ctgmath>
#include "glm/glm/gtx/quaternion.hpp"

 // Custom libs
#include "SimpleShaderProgram.h"

// Window management
GLFWwindow* gWindow = NULL;

// Displaying
const std::string vertexShaderPath = "../../cs148project/vertex.shader";
const std::string fragmentShaderPath = "../../cs148project/fragment.shader";
SimpleShaderProgram *shader;
glm::mat4 projection;
glm::mat4 modelView;
static const int winWidth = 600; // px
static const int winHeight = 400;
static const glm::vec2 winCenter(winWidth/2.0, winHeight/2.0);
static const float nearPlane = 0.1;

// Strafe
static float dx = 0.0;
static const float xmax = 1.0;
static const float xstep = 0.02;

// Rotate (to look where mouse is)
static glm::vec2 orientation(0.0, 0.0); // yaw, pitch (deg)
static const float pixelToDeg = 0.1;
static glm::vec2 mouseLimits(10.0 / pixelToDeg, 7.0 / pixelToDeg); // x, y
static const float degToRad = M_PI / 180.0;

// Testing -- delete later
static float zdist = 25.0;
float triVerts[] = {-0.5, -0.5, zdist,
                     0.5, -0.5, zdist,
                     0.0,  0.5, zdist};

/******************* GLFW Callbacks ********************/
inline double abs(double x){ return x < 0 ? -x : x; };

void cursor_position_callback(GLFWwindow *win, double x, double y){
  // Distance from center
  glm::vec2 mousePos = glm::vec2(x, y) - winCenter;
  // Clamp
  glm::vec2 mousePosClamp = glm::clamp(mousePos, -mouseLimits, mouseLimits);
  // Make sure cursor doesn't get carried away
  if (mousePos != mousePosClamp) {
    glfwSetCursorPos(gWindow, mousePosClamp[0] + winCenter[0], mousePosClamp[1] + winCenter[1]);  }
    orientation = pixelToDeg * mousePos;
 }

void onMouseButton(GLFWwindow *win, int x, int y, int button){
  
}

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods){
  // Only want press or hold down
  if (action != GLFW_PRESS && action != GLFW_REPEAT)
    return;
  // Strafe (within limits)
  if (key == GLFW_KEY_LEFT && dx > -xmax)
    dx -= xstep;
  else if (key == GLFW_KEY_RIGHT && dx < xmax)
    dx += xstep;
  else if (key == GLFW_KEY_Q)
    glfwSetWindowShouldClose(gWindow, 1);
}

/******************* Display Update *******************/

inline void translateModel(double tx, double ty, double tz){
  modelView = glm::translate(modelView, glm::vec3(-tx, -ty, -tz));
}

inline void rotateModel(float ang, glm::vec3 axis){
  modelView = glm::rotate(modelView, ang * degToRad, axis);
}

inline void resetModel(){
  modelView = glm::lookAt(glm::vec3(0.0, 0.0, 0.0),
                          glm::vec3(0.0, 0.0, 1.0),
                          glm::vec3(0.0, -1.0, 0.0));
}

void setProjection(double zoom){
  float fovy = 50.0 / zoom;
  projection = glm::perspective(fovy, 1.5f, nearPlane, 100.0f);
}

void updateUniformMatrices(){
  shader->SetUniformMatrix4fv("ModelView", glm::value_ptr(modelView));
  shader->SetUniformMatrix4fv("Projection", glm::value_ptr(projection));
}

// Called during initialization
void bufferVertices(){
  
  // Create Vertex array object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triVerts), triVerts, GL_STATIC_DRAW);  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  
  GLuint vao = 0;
  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);
  glEnableVertexAttribArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void display(){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Update view from callback updates
  resetModel();
  rotateModel(-orientation[1], glm::vec3(1.0, 0.0, 0.0));
  rotateModel(orientation[0], glm::vec3(0.0, 1.0, 0.0));
  translateModel(-dx, 0.0, 0.0);
  
  // Drawing
  shader->Bind();
  updateUniformMatrices();
  glDrawArrays(GL_TRIANGLES, 0, 3);
  shader->UnBind();
  
}

/**************** Initializaiton Functions *************/

void glSetup() {
  
  // Shader(s)
  shader = new SimpleShaderProgram();
  shader->LoadVertexShader(vertexShaderPath);
  shader->LoadFragmentShader(fragmentShaderPath);
  
  // Initial view
  setProjection(1.0);
  
  // Rendering params
  glClearColor(0, 0.0, 0.0, 1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  
  // Load triangle into buffer
  bufferVertices();

}

void glfwSetup(){
  if(!glfwInit())
    throw std::runtime_error("glfwInit failed");
  
  // Use openGL 4.1
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  
  // Create window
  gWindow = glfwCreateWindow(winWidth, winHeight, "Task Shooting", NULL, NULL);
  if(!gWindow)
    throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
  glfwMakeContextCurrent(gWindow);
  
  // Check version
#ifdef DEBUG
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
#endif
  
  // Set up callbacks
  glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(gWindow, cursor_position_callback);
  glfwSetMouseButtonCallback(gWindow, onMouseButton);
  glfwSetKeyCallback(gWindow, key_callback);
  
}


int main(int argc,  char * argv[]) {
  // Set up window
  glfwSetup();
  
  // Initialize GL (shaders, lights, vertices, etc.)
  glSetup();
  
  // glfw loop
  while(!glfwWindowShouldClose(gWindow)){
    display();
    glfwSwapBuffers(gWindow);
    glfwPollEvents ();
  }
  
  // clean up and exit
  glfwDestroyWindow(gWindow);
  glfwTerminate();
}