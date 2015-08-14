#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 texcoord_GS[3];
out vec2 texcoord_FS;
in vec3 vPosition_GS[3];
out vec3 vPosition_FS;
in vec3 touchVector_GS[3];

in vec3 Normal_GS[3];

in float touchDist_GS[3];

in vec3 tessValue_GS[3];

uniform float timeS;

vec3 averageVectorValue(vec3 v0, vec3 v1, vec3 v2)
{
  vec3 ret;
  ret.x = (v0.x + v1.x + v2.x) / 3.0;
  ret.y = (v0.y + v1.y + v2.y) / 3.0;
  ret.z = (v0.z + v1.z + v2.z) / 3.0;
  return ret;
}

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    //We receive 3 coordinates with tess coordinates tessValue_GS[0], 1, 2...
    //We want to treat this as a face and move them all in some way
    float averageTouchDist = (touchDist_GS[0] + touchDist_GS[1] + touchDist_GS[2]) / 3.0;
    vec3 averageTess = averageVectorValue(tessValue_GS[0], tessValue_GS[1], tessValue_GS[2]);
    vec3 averageNormal = averageVectorValue(Normal_GS[0], Normal_GS[1], Normal_GS[2]);
    vec3 averageTouchV = averageVectorValue(touchVector_GS[0], touchVector_GS[1], touchVector_GS[2]);
    
    float randNum1 = rand(averageTess.xy);
    float randNum2 = rand(averageTess.xz);
    float randNum3 = rand(averageTess.yz);
    
    averageTouchV.x += randNum1 / 3.0;
    averageTouchV.y += randNum2 / 3.0;
    averageTouchV.z += randNum3 / 3.0;
    
    if ((averageNormal.x == 0.0) && (averageNormal.y == 0.0) && (averageNormal.z == 0.0) )
    {
      averageNormal.z = 1.0;
    }
    averageNormal.z = 0.02;


    vPosition_FS = vPosition_GS[0];
    texcoord_FS = texcoord_GS[0];
    
    vec4 offset = vec4(0.0, 0.0, 1.0, 0.0);

    {
       offset = vec4(-averageTouchV * timeS * pow(averageTouchDist, 3) + averageTess.x + averageTess.y + averageTess.z, 1.0) / 10.0;
    }
  
    gl_Position = gl_in[0].gl_Position + offset; EmitVertex();


    vPosition_FS = vPosition_GS[1];
    texcoord_FS = texcoord_GS[1];
    gl_Position = gl_in[1].gl_Position + offset; EmitVertex();


    vPosition_FS = vPosition_GS[2];
    texcoord_FS = texcoord_GS[2];
    gl_Position = gl_in[2].gl_Position + offset; EmitVertex();

    EndPrimitive();
}