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
const std::string groundTexFile = "../../cs148project/crackedGround.png";
const std::string skyObjFile = "../../cs148project/skybox.obj";
const std::string skyTexFile = "../../cs148project/sky2.png";
const std::string projectileObjFile = "../../cs148project/arrowFlip.obj";
const std::string projectileTexFile = "../../cs148project/darkWoodTexture.png";
const std::string targetObjFile = "../../cs148project/targetSign.obj";
const std::string targetTexFile = "../../cs148project/lightWoodTexture.png";
const std::string targetLaundTexFile = "../../cs148project/lightWoodTexture_LAUNDRY.png";
const std::string targetCatTexFile = "../../cs148project/lightWoodTexture_CAT.png";
const std::string targetReportTexFile = "../../cs148project/lightWoodTexture_REPORT.png";
const std::string reticuleObjFile= "../../cs148project/reticule.obj";
const std::string reticuleTexFile = "../../cs148project/reticuleGreen.png";
#endif
