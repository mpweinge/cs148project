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
#include "objMesh.hpp"
#include "projectile.hpp"
#include "constants.h"

// Window management
GLFWwindow* gWindow = NULL;

// Displaying
SimpleShaderProgram *gpShader;
glm::mat4 projection;
glm::mat4 view;
static const int winWidth = 600; // px
static const int winHeight = 400;
static const glm::vec2 winCenter(winWidth/2.0, winHeight/2.0);
static const float nearPlane = 0.1;

// Strafe
static float dx = 0.0;
static const float xmax = 1.0;
static const float xstep = 0.02;

// Rotate
static glm::vec2 orientation(0.0, 0.0); // yaw, pitch (deg)
static const float pixelToDeg = 0.1;
static glm::vec2 mouseLimits(10.0 / pixelToDeg, 7.0 / pixelToDeg); // x, y
static const float degToRad = M_PI / 180.0;
static bool orientationActive = false;

// Objects for rendering objects
float groundLevel = -1.0;
objMesh groundPlane;
const std::string groundObjFile = "../../cs148project/groundPlane.obj";
const std::string groundTexFile = "../../cs148project/square_stones.png";
std::vector<projectile*> projectiles;
void launchProjectile(); // prototpye
SimpleShaderProgram *pshader; // Shader for projectiles

// Testing -- delete later
static float zdist = 25.0;
float triVerts[] = {-0.5, -0.5, zdist,
                     0.5, -0.5, zdist,
                     0.0,  0.5, zdist};

/******************* GLFW Callbacks ********************/
inline double abs(double x){ return x < 0 ? -x : x; };

void cursor_position_callback(GLFWwindow *win, double x, double y){
  // Check if we should activate motion via mouse
  if (!orientationActive) return;
  // Distance from center
  glm::vec2 mousePos = glm::vec2(x, y) - winCenter;
  // Clamp
  glm::vec2 mousePosClamp = glm::clamp(mousePos, -mouseLimits, mouseLimits);
  // Make sure cursor doesn't get carried away
  if (mousePos != mousePosClamp) {
    glfwSetCursorPos(gWindow, mousePosClamp[0] + winCenter[0], mousePosClamp[1] + winCenter[1]);
  }
    orientation = pixelToDeg * mousePos;
 }

void onMouseButton(GLFWwindow *win, int button, int action, int mods){
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
    std::cout << "Launching projectile " << std::endl;
    launchProjectile();
  }
  
}

void cursor_enter_callback(GLFWwindow* window, int entered){
  if (entered && !orientationActive){
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    orientationActive = true;
  }
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

void launchProjectile(){
  projectile *p = new projectile(pshader, "../../cs148project/cylinder.obj", "../../cs148project/blue.png");
  // Position
  glm::mat4 viewInv = glm::inverse(view);
  glm::vec4 initPos = viewInv * glm::vec4(dx, 0.0, 1.0, 0.0);
  p->setPosition(glm::vec3(initPos[0], initPos[1], initPos[2]));
  p->setOrientationEuler(glm::vec3(orientation[1]* degToRad, -orientation[0]* degToRad, 0.0));
  // Velocity
  glm::vec4 vel = viewInv * glm::vec4(0.0, 0.0, 5.0, 0.0);
  p->setVelocity(glm::vec3(vel[0], vel[1], vel[2]));
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
                          glm::vec3(0.0, 0.0, 1.0),
                          glm::vec3(0.0, -1.0, 0.0));
}

void setProjection(double zoom){
  float fovy = 50.0 / zoom;
  projection = glm::perspective(fovy, 1.5f, nearPlane, 100.0f);
}

// Be sure to bind before calling this
void updateUniformMatrices(){
  gpShader->SetUniformMatrix4fv("Modelview", glm::value_ptr(view));
  gpShader->SetUniformMatrix4fv("Projection", glm::value_ptr(projection));
}

void display(){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Set matrix based on view
  resetModel();
  
  rotateModel(-orientation[1], glm::vec3(1.0, 0.0, 0.0));
  rotateModel(orientation[0], glm::vec3(0.0, 1.0, 0.0));
  translateModel(-dx, 0.0, 0.0);
  
  // Drawing
  gpShader->Bind();
  updateUniformMatrices();
  groundPlane.draw();
  gpShader->UnBind();
  
  // Projectiles
  for (size_t i = 0; i < projectiles.size(); i++){
    // If below ground, delete
    glm::vec3 pos = projectiles[i]->getPosition();
    if (-pos[1] < groundLevel){
      delete projectiles[i];
      std::cout << "Deleting projectile\n";
      projectiles.erase(projectiles.begin() + i);
    }
    // Otherwise, draw
    projectiles[i]->draw(view, projection);
  }
  
  
}

/**************** Initialization Functions *************/

void glSetup() {
  
  // Ground plane
  gpShader = new SimpleShaderProgram();
  gpShader->LoadVertexShader(vertexShaderPath);
  gpShader->LoadFragmentShader(fragmentShaderPath);
  //gpShader->LoadTesselationShaders(tessControlShaderPath, tessEvalShaderPath, geometryShaderPath);
  
  groundPlane.init(groundObjFile, gpShader->programid, groundTexFile);
  
  // Projectile
  pshader = new SimpleShaderProgram();
  pshader->LoadVertexShader(vertexShaderPath);
  pshader->LoadFragmentShader(fragmentShaderPath);


  // Initial view
  setProjection(1.0);
  
  // Rendering params
  glClearColor(0, 0.0, 0.0, 1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glPatchParameteri(GL_PATCH_VERTICES, 3);
  glEnable(GL_DEPTH_TEST);
  
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
  //glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(gWindow, cursor_position_callback);
  glfwSetMouseButtonCallback(gWindow, onMouseButton);
  glfwSetKeyCallback(gWindow, key_callback);
  glfwSetCursorEnterCallback(gWindow, cursor_enter_callback);
  
}

int main(int argc,  char * argv[]) {
  // Set up window
  glfwSetup();
  
  // Initialize GL (gpShaders, lights, vertices, etc.)
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
