#version 410 core

uniform mat4 Projection;
uniform mat4 Modelview;
//uniform float scale;

in vec3 Position;
in vec2 Texcoord;

out vec3 vPosition;
out vec2 TexCoord_TC;

void main()
{
  gl_Position = vec4(Position.xyz,1.0);
  vPosition = gl_Position.xyz;
  TexCoord_TC = Texcoord;
}