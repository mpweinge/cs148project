#version 410 core

layout(triangles, equal_spacing, ccw) in;
//in vec3 tcPosition[];

uniform mat4 Projection;
uniform mat4 Modelview;

/*out vec3 tePosition;
out vec3 tePatchDistance;
uniform mat4 Projection;
uniform mat4 Modelview;*/

void main()
{
    /*vec3 p0 = gl_TessCoord.x * tcPosition[0];
    vec3 p1 = gl_TessCoord.y * tcPosition[1];
    vec3 p2 = gl_TessCoord.z * tcPosition[2];
//    tePatchDistance = gl_TessCoord;
    vec3 tePosition = normalize(p0 + p1 + p2);
    gl_Position = Projection * Modelview * vec4(tePosition, 1);*/
    
    vec4 pos = gl_TessCoord.x * gl_in[0].gl_Position
    + gl_TessCoord.y * gl_in[1].gl_Position
    + gl_TessCoord.z * gl_in[2].gl_Position;
    
    gl_Position = Projection * Modelview * pos;
}