//
//  main.cpp
//  cs148project
//
//  Created by Michael Weingert on 2015-08-01.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#define GLEW_VERSION_2_0 1

#include <iostream>

#include <GLUT/glut.h>
#include <OpenGL/gl.h>

#include "SimpleShaderProgram.h"


int currX;
int currY;
float xRot=0.0;
float yRot=0.0;
bool leftButton = false;
bool middleButton = false;
float zOffs = 0;

SimpleShaderProgram *shader;

void DrawWithShader(){
    //shader->Bind();
    //Draw stuff here
    //shader->UnBind();
}

void DisplayCallback(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Spock settings on top
    GLfloat light_position[] = { -4.0, 0.0, 4.0, 0.0 };
    //GLfloat light_position[] = { 3.0, -2.0, 4.0, 0.0 };
    glShadeModel (GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat lightcolor[] = { 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightcolor);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightcolor);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightcolor);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat specular[] = { 0.3, 0.3, 0.3 };
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0.5);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 10);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //Spock settings on top
    gluLookAt(0, 0, 2.5, 0, 0, 0, 0, 1, 0);
    //gluLookAt(0, 0, 5.5, 0, 0, 0, 0, 1, 0);
    
    glRotatef(-yRot, 1.0, 0.0, 0.0);
    glRotatef(xRot, 0.0, 0.0, 1.0);
    
    glTranslatef(0, 0, zOffs);
    //glTranslatef(0.f, 0.f, -10.f);
    
    DrawWithShader();
    glutSwapBuffers();
}

void ReshapeCallback(int w, int h){
    glViewport(0, 0, w, h);
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(30.0f, (float)w/(float)h, 0.1f, 100000.f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void KeyCallback(unsigned char key, int x, int y)
{
    switch(key) {
        case 'q':
            exit(0);
        default:
            break;
    }
    
    glutPostRedisplay();
}

void Setup()
{
    
}

void mouseFunction(int button, int state, int x, int y)
{
    currX = x;
    currY = y;
    leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));
    middleButton = ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN));

    glutPostRedisplay();
}

void mouseMoveFunction(int x, int y)
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
    glutPostRedisplay();
}

int main(int argc,  char * argv[]) {
    // insert code here...
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(20, 20);
    glutInitWindowSize(640, 480);
    glutCreateWindow("CS148 Final Project");
    
    Setup();
    
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyCallback);
    glutIdleFunc(DisplayCallback);
    
    glutMouseFunc(&mouseFunction);
    glutMotionFunc(&mouseMoveFunction);
    
    glutMainLoop();
    return 0;
}
