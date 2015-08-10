#version 410 core

layout(vertices = 3) out;
in vec3 vPosition[];
out vec3 tePosition[];

in vec2 TexCoord_TC[];
out vec2 texcoord_ES[];

in vec3 Normal_TC[];
out vec3 Normal_ES[];
out float touchDist_ES[];

out vec3 touchVector_ES[];

uniform vec3 touchLocation;

uniform mat4 Model;
//uniform float test;

float distanceToTouch(vec4 vertexPosition)
{
    return (5 - clamp(distance(touchLocation, vertexPosition.xyz), 0, 5)) * 2;
}

void main()
{
    //tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    texcoord_ES[gl_InvocationID] = TexCoord_TC[gl_InvocationID];
    Normal_ES[gl_InvocationID] = Normal_TC[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tePosition[gl_InvocationID] = vPosition[gl_InvocationID];
  
    vec4 newPosition = Model * vec4(vPosition[gl_InvocationID], 1.0);
  
    touchVector_ES[gl_InvocationID] = touchLocation - newPosition.xyz;
  
    float touchDist = distanceToTouch(newPosition);
    touchDist_ES[gl_InvocationID] = touchDist;
  
    if (gl_InvocationID == 0) {
        if ((touchLocation.x == 0.0) && (touchLocation.y == 0.0) && (touchLocation.z == 0.0) )
        {
            //Assume that there was no touch
            gl_TessLevelInner[0] = 2.0;
            gl_TessLevelOuter[0] = 1.0;
            gl_TessLevelOuter[1] = 1.0;
            gl_TessLevelOuter[2] = 1.0;
        } else {
            /*gl_TessLevelInner[0] = 2.0 + int((distanceToTouch(Model * vec4(vPosition[0], 1.0))));
            gl_TessLevelOuter[0] = 1.0 + int((distanceToTouch(Model * vec4(vPosition[0], 1.0))));
            gl_TessLevelOuter[1] = 1.0 + int((distanceToTouch(Model * vec4(vPosition[1], 1.0))));
            gl_TessLevelOuter[2] = 1.0 + int((distanceToTouch(Model * vec4(vPosition[2], 1.0))));*/
          
          gl_TessLevelInner[0] = 2.0 + int(touchDist);
          gl_TessLevelOuter[0] = 1.0 + int(touchDist);
          gl_TessLevelOuter[1] = 1.0 + int(touchDist);
          gl_TessLevelOuter[2] = 1.0 + int(touchDist);
          
        }
    }
}

