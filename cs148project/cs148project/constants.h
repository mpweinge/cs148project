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
#include <GL/glu.h>

#define GLEW_VERSION_2_0 1

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//#define DEBUG_TESS 1

// Shaders
const std::string objFilename = "../../cs148project/cube.obj";
const std::string vertexShaderPath = "../../cs148project/vertex.shader";
const std::string vertexTessShaderPath = "../../cs148project/vertex_tess.shader";
const std::string fragmentShaderPath = "../../cs148project/fragment.shader";
const std::string tessControlShaderPath = "../../cs148project/tess_control.shader";
const std::string tessEvalShaderPath = "../../cs148project/tess_eval.shader";
const std::string geometryShaderPath = "../../cs148project/geometry.shader";

// Objects and textures
const std::string groundObjFile = "../../cs148project/groundPlane.obj";
const std::string groundTexFile = "../../cs148project/square_stones.png";
const std::string projectileObjFile = "../../cs148project/cylinder.obj";
const std::string projectileTexFile = "../../cs148project/blue.png";
const std::string targetObjFile = "../../cs148project/cube.obj";
const std::string targetTexFile = "../../cs148project/blue.png";
#endif
