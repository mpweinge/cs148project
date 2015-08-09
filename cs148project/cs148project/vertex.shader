#version 410 core

uniform mat4 Projection;
uniform mat4 Modelview;
uniform float scale;

in vec3 Position;
in vec2 Texcoord;

out vec3 vPosition;
out vec2 texcoord;

void main()
{
  gl_Position = Projection * Modelview * vec4(Position.xyz,1.0);
  vPosition = gl_Position.xyz;
  texcoord = Texcoord;
}