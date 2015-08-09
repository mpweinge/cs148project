#version 410 core

layout(vertices = 3) out;
//in vec3 vPosition[];
//out vec3 tcPosition[];

//uniform vec3 touchLocation;
//uniform float test;

/*float distanceToTouch(vec3 vertexPosition)
{
    return clamp(distance(touchLocation, vertexPosition), 0, 5) * 2;
}*/

void main()
{
    //tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    
    if (gl_InvocationID == 0) {
        /*if ((touchLocation.x == 0.0) && (touchLocation.y == 0.0) && (touchLocation.z == 0.0) && (test == 0.0))
        {*/
            //Assume that there was no touch
            gl_TessLevelInner[0] = 4.0;
            gl_TessLevelOuter[0] = 2.0;
            gl_TessLevelOuter[1] = 2.0;
            gl_TessLevelOuter[2] = 2.0;
        /*} else {
            gl_TessLevelInner[0] = 2.0;
            gl_TessLevelOuter[0] = 1.0 + int((distanceToTouch(vPosition[0])));
            gl_TessLevelOuter[1] = 1.0 + int((distanceToTouch(vPosition[1])));
            gl_TessLevelOuter[2] = 1.0 + int((distanceToTouch(vPosition[2])));
        }*/
    }
}

