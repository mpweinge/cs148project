#version 410 core

//out vec4 FragColor;
//layout(location = 0, index = 0)

uniform sampler2D tex;

in vec3 vPosition_FS;
in vec2 texcoord_FS;

out vec4 FragColor;

/*in vec3 gFacetNormal;
in vec3 gTriDistance;
in vec3 gPatchDistance;
in float gPrimitive;
uniform vec3 LightPosition;
uniform vec3 DiffuseMaterial;
uniform vec3 AmbientMaterial;*/

/*float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}*/

uniform vec3 touchLocation;

uniform mat4 Model;
//uniform float test;

float distanceToTouch(vec4 vertexPosition)
{
    return (3 - clamp(distance(touchLocation, vertexPosition.xyz), 0, 3));
}

void main()
{
    /*vec3 N = normalize(gFacetNormal);
    vec3 L = LightPosition;
    float df = abs(dot(N, L));
    vec3 color = AmbientMaterial + df * DiffuseMaterial;

    float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
    float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
    color = amplify(d1, 40, -0.5) * amplify(d2, 60, -0.5) * color;*/

    //Calculate distance here from position to touch point and colour accordingly
    
    float distToTouch = (distanceToTouch(Model * vec4(vPosition_FS, 1.0)) / 3.0);
    FragColor = vec4(distToTouch, 0.0, 1.0, 1.0);
    //FragColor = vec4(1.0, 0.0, 1.0, 1.0);

  // Use texture
    //FragColor = texture(tex, texcoord_FS);
  //FragColor = vec4(0.1, 0.1, texture(tex, texcoord_FS).z, 1.0);
  
  // Debug Texture
  /*vec4 a = vec4(1.0, 1.0, 1.0, 1.0);
  vec4 b = vec4(1.0, 0.0, 0.0, 1.0);
    
//  
  if (texcoord_FS.x > 1)
    FragColor = a;
  else
    FragColor = b;*/
}