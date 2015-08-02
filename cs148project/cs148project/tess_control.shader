#version 410 core

layout(vertices = 3) out;
//in vec3 vPosition[];
//out vec3 tcPosition[];

void main()
{
    //tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = 4.0;
        gl_TessLevelOuter[0] = 2.0;
        gl_TessLevelOuter[1] = 2.0;
        gl_TessLevelOuter[2] = 2.0;
    }
}

