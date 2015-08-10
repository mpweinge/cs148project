#version 410 core

uniform mat4 Projection;
uniform mat4 Modelview;
uniform float scale;

in vec3 Position;
in vec2 Texcoord;

out vec3 vPosition_FS;
out vec2 texcoord_FS;

void main()
{
  gl_Position = Projection * Modelview * vec4(Position.xyz,1.0);
  vPosition_FS = gl_Position.xyz;
  texcoord_FS = Texcoord;
}