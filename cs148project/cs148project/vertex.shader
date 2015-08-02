#version 410

uniform mat4 Projection;
uniform mat4 Modelview;

in vec3 Position;

void main()
{
    gl_Position = Projection*Modelview * vec4(Position.xyz,1.0);
    //gl_Position = vec4(Position,1.0);
    //gl_Position = Projection*Modelview*gl_Vertex;
    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    //vPosition = Position.xyz;
}