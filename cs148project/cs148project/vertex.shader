#version 410

uniform mat4 Projection;
uniform mat4 Modelview;
uniform float scale;

in vec3 Position;
out vec3 vPosition;

void main()
{
  gl_Position = Projection*Modelview * vec4(Position.xyz,1.0);
  vPosition = gl_Position.xyz;
}