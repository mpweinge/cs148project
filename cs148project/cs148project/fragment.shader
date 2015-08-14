#version 410 core

uniform sampler2D tex;
uniform vec3 touchLocation;
uniform int treatGreenAsTransparent;

in vec3 vPosition_FS;
in vec2 texcoord_FS;

out vec4 FragColor;

uniform mat4 Model;

void main()
{
    // Use texture
    FragColor = texture(tex, texcoord_FS);

    // Since alpha channel is not included, overload green channel in some cases
    if (treatGreenAsTransparent == 1){
      FragColor.a = 1.0 - FragColor.g;
      FragColor.g = 0.0;
    }
}