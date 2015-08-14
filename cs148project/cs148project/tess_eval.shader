#version 410 core

layout(triangles, equal_spacing, ccw) in;
in vec3 tePosition[];

in vec2 texcoord_ES[];
out vec2 texcoord_GS;

in vec3 Normal_ES[];
out vec3 Normal_GS;

in float touchDist_ES[];
out float touchDist_GS;

in vec3 touchVector_ES[];
out vec3 touchVector_GS;

out vec3 vPosition_GS;

out vec3 tessValue_GS;

uniform mat4 Projection;
uniform mat4 Modelview;

uniform mat4 Model;
uniform vec3 touchLocation;


vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
   	return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate2D(vec3 v0, vec3 v1, vec3 v2)
{
   	return (v0 + v1 + v2) / 3.0;
}

float interpolate2D(float v0, float v1, float v2)
{
  return (v0 + v1 + v2) / 3.0;
}

float distanceToTouch(vec4 vertexPosition)
{
  return (3 - clamp(distance(touchLocation, vertexPosition.xyz), 0, 3)) * 2;
}

void main()
{
  
    vec4 pos = gl_TessCoord.x * gl_in[0].gl_Position
    + gl_TessCoord.y * gl_in[1].gl_Position
    + gl_TessCoord.z * gl_in[2].gl_Position;
    
    gl_Position = Projection * Modelview * pos;
    vPosition_GS = pos.xyz;
    
    //In here add the 'explosion' effect, aka move the points with an offset based off of
    //proximity to the point of collision
    texcoord_GS = interpolate2D(texcoord_ES[0], texcoord_ES[1], texcoord_ES[2]);
    Normal_GS = interpolate2D(Normal_ES[0], Normal_ES[1], Normal_ES[2]);
    touchDist_GS = distanceToTouch(Model * vec4(vPosition_GS, 1.0));
  
    touchVector_GS = interpolate2D(touchVector_ES[0], touchVector_ES[1], touchVector_ES[2]);
  
    tessValue_GS = gl_TessCoord;
  

}