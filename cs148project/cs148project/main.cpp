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

#include <cmath>

#include "SimpleShaderProgram.h"

using namespace std;


int currX;
int currY;
float xRot=0.0;
float yRot=0.0;
bool leftButton = false;
bool middleButton = false;
float zOffs = 0;

SimpleShaderProgram *shader;

struct Vertex {
    float x,y,z;
    bool permaHeat;
    float distToClosestTouchedVertice;
};

struct Triangle {
    int v0, v1, v2;
    float t0, t1, t2;
};

struct attributes{
    GLfloat temp[2];
};

vector<Triangle> tris;
vector<Vertex> vertices;
vector<Vertex> textCoordinates;

void DrawWithShader(){
    //shader->Bind();
    //Draw stuff here
    //shader->UnBind();
}

void DrawPLAIN()
{
    glBegin(GL_TRIANGLES);
    
    for (auto& t : tris) {
        glColor3f(0.8, 0.8, 0.8);
        Vertex v1 = vertices[t.v0-1];
        Vertex v2 = vertices[t.v1-1];
        Vertex v3 = vertices[t.v2-1];
        
        //Here we want to compute the normals before we render the mesh
        Vertex u;
        u.x = v2.x - v1.x;
        u.y = v2.y - v1.y;
        u.z = v2.z - v1.z;
        
        Vertex v;
        v.x = v3.x - v1.x;
        v.y = v3.y - v1.y;
        v.z = v3.z - v1.z;
        
        //Compute the cross product of u and v
        // n = u * v / ||u * v||
        float n[] = {u.y*v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x *v.y - u.y * v.x};
        //Normalize the vector
        float nMag = sqrt(pow(n[0], 2) + pow(n[1], 2) + pow(n[2],2));
        
        glNormal3f(n[0] / nMag, n[1] / nMag, n[2] / nMag);
        
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }
    glEnd();
    glFlush();
}

void loadTriFile( string& filename)
{
    ifstream ifs(filename.c_str());
    if (ifs.is_open()) {
        
        string firstChar;
        
        while(ifs >> firstChar )
        {
            if (firstChar == "#") {
                //Comment line so gobble it up
                getline(ifs, firstChar);
            } else if (firstChar == "v") {
                Vertex nextVertex;
                ifs >> nextVertex.x;
                ifs >> nextVertex.y;
                ifs >> nextVertex.z;
                
                vertices.push_back(nextVertex);
                
            } else if (firstChar == "vt") {
                Vertex textCoordinate;
                ifs >> textCoordinate.x;
                ifs >> textCoordinate.y;
                
                textCoordinates.push_back(textCoordinate);
            } else if (firstChar == "vn") {
                //DGAF about vertex normals, so gobble it up
                getline(ifs, firstChar);
            } else if (firstChar == "f") {
                Triangle nextTri;
                ifs >> nextTri.v0;
                
                char nextChar;
                nextChar = ifs.peek();
                if (nextChar == '/') {
                    ifs >> nextChar;
                    ifs >> nextTri.t0;
                    ifs >> nextChar;
                    
                    //Eat up the vector normal
                    ifs >> nextTri.v1;
                    //Second vertex
                    ifs >> nextTri.v1;
                    ifs >> nextChar;
                    ifs >> nextTri.t1;
                    ifs >> nextChar;
                    
                    //Eat up the vector normal
                    ifs >> nextTri.v2;
                    //Third vertex
                    ifs >> nextTri.v2;
                    ifs >> nextChar;
                    ifs >> nextTri.t2;
                    
                    //Gobble up the rest of the line
                    getline(ifs, firstChar);
                } else {
                    ifs >> nextTri.v1;
                    ifs >> nextTri.v2;
                }
                
                tris.push_back(nextTri);
            }
        }
    }
}

void DisplayCallback(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Spock settings on top
    GLfloat light_position[] = { -4.0, 0.0, 10.0, 0.0 };
    //GLfloat light_position[] = { 3.0, -2.0, 4.0, 0.0 };
    glShadeModel (GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat lightcolor[] = { 0.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightcolor);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightcolor);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightcolor);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    //GLfloat specular[] = { 0.3, 0.3, 0.3 };
    //glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
    //glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0.5);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 10);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0, 0, 2.5, 0, 0, 0, 0, 1, 0);
    
    glRotatef(-yRot, 1.0, 0.0, 0.0);
    glRotatef(xRot, 0.0, 0.0, 1.0);
    
    glTranslatef(0, 0, zOffs);
    
    DrawPLAIN();
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
    string filename = "../../cs148project/cube.obj";
    loadTriFile(filename);
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
