#version 410

in vec3 Position;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform float scale;

void main()
{
   gl_Position = Projection * ModelView * vec4(Position,1.0);
  //gl_Position *= vec4(100 * Position.xy, Position.z, 1.0);
   // gl_Position = vec4(Position,1.0);
    //gl_Position = Projection*Modelview*gl_Vertex;
    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    //vPosition = Position.xyz;
}