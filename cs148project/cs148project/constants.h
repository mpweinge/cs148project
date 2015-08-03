//
//  constants.h
//  cs148project
//
//  Created by Michael Weingert on 2015-08-02.
//  Copyright (c) 2015 Michael Weingert. All rights reserved.
//

#ifndef cs148project_constants_h
#define cs148project_constants_h

#include <vector>

#define GLEW_VERSION_2_0 1

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const std::string objFilename = "../../cs148project/cube.obj";
const std::string vertexShaderPath = "../../cs148project/vertex.shader";
const std::string fragmentShaderPath = "../../cs148project/fragment.shader";
const std::string tessControlShaderPath = "../../cs148project/tess_control.shader";
const std::string tessEvalShaderPath = "../../cs148project/tess_eval.shader";
const std::string geometryShaderPath = "../../cs148project/geometry.shader";

const int icoFaces[] = {
    2, 1, 0,
    3, 2, 0,
    4, 3, 0,
    5, 4, 0,
    1, 5, 0,
    
    11, 6,  7,
    11, 7,  8,
    11, 8,  9,
    11, 9,  10,
    11, 10, 6,
    
    1, 2, 6,
    2, 3, 7,
    3, 4, 8,
    4, 5, 9,
    5, 1, 10,
    
    2,  7, 6,
    3,  8, 7,
    4,  9, 8,
    5, 10, 9,
    1, 6, 10 };

const float icoVerts[] = {
    0.000f,  0.000f,  1.000f,
    0.894f,  0.000f,  0.447f,
    0.276f,  0.851f,  0.447f,
    -0.724f,  0.526f,  0.447f,
    -0.724f, -0.526f,  0.447f,
    0.276f, -0.851f,  0.447f,
    0.724f,  0.526f, -0.447f,
    -0.276f,  0.851f, -0.447f,
    -0.894f,  0.000f, -0.447f,
    -0.276f, -0.851f, -0.447f,
    0.724f, -0.526f, -0.447f,
    0.000f,  0.000f, -1.000f };

struct Vertex {
    float x,y,z;
    bool permaHeat;
    float distToClosestTouchedVertice;
};

struct Triangle {
    int v0, v1, v2;
    float t0, t1, t2;
};

void loadTriFile( const std::string& filename,
                 std::vector<Vertex> & vertices,
                 std::vector<Triangle> &tris,
                 std::vector<Vertex> &textCoordinates)
{
    std::ifstream ifs(filename.c_str());
    if (ifs.is_open()) {
        
        std::string firstChar;
        
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

void screenToWorld(GLdouble x, GLdouble y, GLdouble & nearPointX, GLdouble & nearPointY, GLdouble & nearPointZ, GLfloat * Modelview, GLfloat * Projective)
{
    GLint viewportMatrix[4];
    
    glGetIntegerv(GL_VIEWPORT, viewportMatrix);
    
    y = (float)viewportMatrix[3] - y;
    
    GLdouble winZ;
    
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    
    GLdouble modelViewMatrix[16];
    GLdouble projectionMatrix[16];
    
    for (int i = 0; i < 16; i++)
    {
        modelViewMatrix[i] = Modelview[i];
        projectionMatrix[i] = Projective[i];
    }
    
    gluUnProject(x,y,winZ,modelViewMatrix, projectionMatrix, viewportMatrix, &nearPointX,  &nearPointY, &nearPointZ);
}

#endif
