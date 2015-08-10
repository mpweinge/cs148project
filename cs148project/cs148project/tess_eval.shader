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

/*out vec3 tePosition;
out vec3 tePatchDistance;
uniform mat4 Projection;
uniform mat4 Modelview;*/

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
    /*vec3 p0 = gl_TessCoord.x * tcPosition[0];
    vec3 p1 = gl_TessCoord.y * tcPosition[1];
    vec3 p2 = gl_TessCoord.z * tcPosition[2];
//  tePatchDistance = gl_TessCoord;
    vec3 tePosition = normalize(p0 + p1 + p2);
    gl_Position = Projection * Modelview * vec4(tePosition, 1);*/
    
    /*float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    
    vec4 a = mix(gl_in[1].gl_Position, gl_in[0].gl_Position, u);
    vec4 b = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, u);*/
    
    //vec4 position = mix(a, b, v);
    //texcoord = position.xy;
    
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
  
  
    //texcoord_GS = vec2(1.1, 1.1);
    //texcoord_FS = vec2(1.0, 1.0);
}