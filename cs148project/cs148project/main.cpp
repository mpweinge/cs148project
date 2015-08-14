//
//  main.cpp
//  cs148project
//
//  Created by Joe Polin on 8/2/15.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

// Flag(s)
#define GLEW_VERSION_2_0 1
#define USE_TESS

// Standard libs
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>

// Window and user input
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

// openGL
//#include <OpenGL/gl.h>
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
#include "objMesh.hpp"
#include "projectile.hpp"
#include "target.hpp"
#include "constants.h"
#include "collisionCheck.hpp"

// Window management
GLFWwindow* gWindow = NULL;
static bool playing = false;

// Displaying
SimpleShaderProgram *envShader;
glm::mat4 projection;
glm::mat4 view;
static int winWidth = 900; // px
static int winHeight = 600;
static glm::vec2 winCenter(winWidth/2.0, winHeight/2.0);
static const float nearPlane = 0.01f;
static const float farPlane = 300.0f;

// Strafe
static float dx = 0.0;
static const float xmax = 0.0;
static const float xstep = 0.02;

// Rotate
static glm::vec2 orientation(0.0, 0.0); // yaw, pitch (deg)
static const float pixelToDeg = 0.1;
bool limitOrientation = true;
static glm::vec2 mouseLimits(20.0 / pixelToDeg, 15.0 / pixelToDeg); // x, y
static const float degToRad = M_PI / 180.0;

// Zoom
double zoom = 1.0;
static const float zoomMax = 2.0;

// Objects for rendering objects
float groundLevel = -1.0;
objMesh groundPlane, skyBox, reticule;
std::vector<projectile*> projectiles;
std::vector<target*> targets;
void launchProjectile(); // prototpye
SimpleShaderProgram *pshader, *tshader; // Shader for projectiles and targets

glm::vec3 touchPoint = glm::vec3(0, 0, 0);

// Debug tools
inline void printVec3(std::string n, glm::vec3 v){
  std::cout << n << ": " << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
}

/******************* GLFW Callbacks ********************/
inline double abs(double x){ return x < 0 ? -x : x; };

void cursor_position_callback(GLFWwindow *win, double x, double y){
  // Check if we should activate motion via mouse
  if (!playing) return;
  // Distance from center
  glm::vec2 mousePos = glm::vec2(x, y) - winCenter;
  // Clamp
  glm::vec2 mousePosClamp;
  if (limitOrientation)
    mousePosClamp = glm::clamp(mousePos, -mouseLimits, mouseLimits);
  else
    mousePosClamp = mousePos;
  // Make sure cursor doesn't get carried away
  if (mousePos != mousePosClamp) {
    glfwSetCursorPos(gWindow, mousePosClamp[0] + winCenter[0], mousePosClamp[1] + winCenter[1]);
  }
    orientation = pixelToDeg * mousePos;
}

inline float clamp(float x, float xmin, float xmax){
  x = x < xmin ? xmin : x;
  x = x > xmax ? xmax : x;
  return x;
}

void scrollCallback(GLFWwindow* w, double x, double y){
  float scrollToZoom = 0.1;
  zoom = clamp(zoom - y * scrollToZoom, 1.0, zoomMax);
}

void onMouseButton(GLFWwindow *win, int button, int action, int mods){
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && playing){
    launchProjectile();
  }
}

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods){
  // Only want press or hold down
  if (action != GLFW_PRESS && action != GLFW_REPEAT)
    return;
  // Play/pause
  if (key == GLFW_KEY_P){
    if (playing == false){
      glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      playing = true;
    }
    else {
      glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      playing = false;
    }
  }
  // Keys don't work when pause
  if (!playing) return;
  // Strafe (within limits)
  if (key == GLFW_KEY_LEFT && dx > -xmax)
    dx -= xstep;
  else if (key == GLFW_KEY_RIGHT && dx < xmax)
    dx += xstep;
  else if (key == GLFW_KEY_Q)
    glfwSetWindowShouldClose(gWindow, 1);
  // Screenshot
  else if (key == GLFW_KEY_S){
    time_t dateTime;
    time(&dateTime);
    struct tm * tInfo;
    tInfo = localtime(&dateTime);
    char buffer[50];
    strftime(buffer, 50, "Screenshot %F %T", tInfo);
    std::string fname(buffer);
    screenshot(fname);
  }
}

void window_resize_callback(GLFWwindow *w, int x, int y){
  winWidth = x;
  winHeight = y;
  winCenter = glm::vec2(winWidth/2.0, winHeight/2.0);
}

/******************* Display Update *******************/
void launchProjectile(){
  projectile *p = new projectile(pshader, projectileObjFile, projectileTexFile);
  // Position
  glm::mat4 viewInv = glm::inverse(view);
  glm::vec4 initPos = viewInv * glm::vec4(dx, 0.0, -1.0, 0.0) + glm::vec4(0, -0.0, 0, 0);
  p->setPosition(glm::vec3(initPos[0], initPos[1], initPos[2]));
  p->setOrientationEuler(glm::vec3(orientation[1]* degToRad, -orientation[0]* degToRad, 0.0));
  // Velocity
  glm::vec4 vel = viewInv * glm::vec4(0.0, 0.0, -7.0, 0.0);
  p->setVelocity(glm::vec3(vel[0], vel[1], vel[2]));
  p->setOrientationEuler(glm::vec3(0, M_PI, 0));
  projectiles.push_back(p);
}

inline void translateModel(double tx, double ty, double tz){
  view = glm::translate(view, glm::vec3(-tx, -ty, -tz));
}

inline void rotateModel(float ang, glm::vec3 axis){
  view = glm::rotate(view, ang * degToRad, axis);
}

inline void resetModel(){
  view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0),
                          glm::vec3(0.0, 0.0, -1.0),
                          glm::vec3(0.0, 1.0, 0.0));
}

void setProjection(){
  float fovy = 50.0f / zoom;
  projection = glm::perspective(fovy * degToRad, 1.5f, nearPlane, farPlane);
}

// Be sure to bind before calling this
void updateUniformMatrices(){
  envShader->SetUniformMatrix4fv("Modelview", glm::value_ptr(view));
  envShader->SetUniformMatrix4fv("Projection", glm::value_ptr(projection));
}

// This is a bit of a hack, but the SimpleImage class doesn't support alpha
// channel. To get around this, we'll overload a different rgb channel as
// the transparency, and then set it to zero. In this case, we're using green
// as alpha. As a result, all green in image is zero-ed in fragment shader.
void setGreenAsAlpha(SimpleShaderProgram *s, bool status){
  s->SetUniform("treatGreenAsTransparent", status ? 1 : 0);
}

void display(){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  if (!playing){
    return;
  }
  
  // Update zoom
  setProjection();
  
  // Reset view matrix
  resetModel();

  // Update view matrix for world based on rotation
  rotateModel(orientation[1], glm::vec3(1.0, 0.0, 0.0));
  rotateModel(orientation[0], glm::vec3(0.0, 1.0, 0.0));
  translateModel(-dx, 0.0, 0.0);
  
  // Draw ground and sky
  envShader->Bind();
  updateUniformMatrices();
  groundPlane.draw();
  skyBox.draw();
  envShader->UnBind();
  
  // Projectiles--draw or delete if below ground
  std::vector<projectile*>::iterator pit = projectiles.begin();
  while (pit != projectiles.end()){
    // If below ground, delete
    glm::vec3 pos = (*pit)->getPosition();
    if (pos[1] < groundLevel){
      delete *pit;
      pit = projectiles.erase(pit);
    }
    else{
      // Otherwise, draw
      (*pit)->draw(view, projection);
      pit++;
    }
  }
  
  // Targets
  std::vector<target*>::iterator tit = targets.begin();
  while(tit != targets.end()){
    // Check if target has declared that it should be removed
    if ((*tit)->deleteMe){
      delete *tit;
      tit = targets.erase(tit);
      continue;
    }
    // Check for collisions
    glm::vec3 intersectionPoint;
    for (pit = projectiles.begin(); pit != projectiles.end(); pit++){
      if (checkCollision(*pit, *tit, intersectionPoint)){
        // Tell target to explode and stop moving
        (*tit)->collisionLocation = intersectionPoint;
        (*tit)->startTimer();
        (*tit)->setVelocity(glm::vec3(0.0));
      }
    }
    (*tit)->draw(view, projection);
    tit++;
  }
  
  // Need to draw reticule last since it has transparency. Also,
  // we want view to be identity for it:
  glm::mat4 bufferView = view;
  resetModel();
  
  envShader->Bind();
  updateUniformMatrices();
  // We're using green as alpha--see not above setGreenAsAlpha
  setGreenAsAlpha(envShader, true);
  reticule.draw();
  setGreenAsAlpha(envShader, false);
  envShader->UnBind();
  
  // Restore view for when launch is called.
  view = bufferView;
}

/**************** Initialization Functions *************/
void createTargets(){
  // Target
  tshader = new SimpleShaderProgram();
  tshader->LoadVertexShader(vertexTessShaderPath);
  tshader->LoadFragmentShader(fragmentShaderPath);
  tshader->LoadTesselationShaders(tessControlShaderPath, tessEvalShaderPath, geometryShaderPath);
  
  // Glide back and forth along x at fixed depth
  std::vector<glm::vec3> traj;
  glm::vec3 p11 = glm::vec3(-1.5, 0.0, -5.0);
  glm::vec3 p12 = glm::vec3(1.5, 0.0, -5.0);
  traj.push_back(p11);
  traj.push_back(p12);
  traj.push_back(p11); // Loop
  target *t = new target(tshader, targetObjFile, targetCatTexFile);
  t->loadTraj(traj, 0.5);
  targets.push_back(t);
  
  traj.clear();
  p11 = glm::vec3(-1.5, 1.0, -5.0);
  p12 = glm::vec3(1.5, 1.0, -5.0);
  traj.push_back(p12);
  traj.push_back(p11);
  traj.push_back(p12);
  t = new target(tshader, targetObjFile, targetLaundTexFile);
  t->loadTraj(traj, 1.5);
  targets.push_back(t);
  
  traj.clear();
  p11 = glm::vec3(-1.5, 0.0, -9.0);
  p12 = glm::vec3(1.5, 0.0, -9.0);
  traj.push_back(p12);
  traj.push_back(p11);
  traj.push_back(p12);
  t = new target(tshader, targetObjFile, targetReportTexFile);
  t->loadTraj(traj, 0.2);
  targets.push_back(t);
}

void glSetup() {
  
  // Ground plane
  envShader = new SimpleShaderProgram();
  envShader->LoadVertexShader(vertexShaderPath);
  envShader->LoadFragmentShader(fragmentShaderPath);
  //envShader->LoadTesselationShaders(tessControlShaderPath, tessEvalShaderPath, geometryShaderPath);
  
  // Environment
  groundPlane.init(groundObjFile, envShader->programid, groundTexFile);
  skyBox.init(skyObjFile, envShader->programid, skyTexFile);
  reticule.init(reticuleObjFile, envShader->programid, reticuleTexFile);
  
  // Projectile
  pshader = new SimpleShaderProgram();
  pshader->LoadVertexShader(vertexShaderPath);
  pshader->LoadFragmentShader(fragmentShaderPath);
  
  createTargets();

  // Initial view
  setProjection();
  resetModel();
  
  // Rendering params
  glClearColor(0, 0.0, 0.0, 1);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glPatchParameteri(GL_PATCH_VERTICES, 3);
  glEnable(GL_DEPTH_TEST);
  
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  
  glEnableClientState(GL_VERTEX_ARRAY);
}

void glfwSetup(){
  if(!glfwInit())
    throw std::runtime_error("glfwInit failed");
  
  // Use openGL 4.1
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  
  // Create window
  gWindow = glfwCreateWindow(winWidth, winHeight, "Task Shooting: P = play/pause -- Q = quit", NULL, NULL);
  if(!gWindow)
    throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
  glfwMakeContextCurrent(gWindow);
  
  // Check version
#ifdef DEBUG
  //std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  //std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  //std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  //std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
#endif
  
  // Set up callbacks
  glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetCursorPosCallback(gWindow, cursor_position_callback);
  glfwSetMouseButtonCallback(gWindow, onMouseButton);
  glfwSetKeyCallback(gWindow, key_callback);
  glfwSetFramebufferSizeCallback(gWindow, window_resize_callback);
  glfwSetScrollCallback(gWindow, scrollCallback);
}

int main(int argc,  char * argv[]) {
  // Set up window
  glfwSetup();
  
  // Initialize GL (envShaders, lights, vertices, etc.)
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
