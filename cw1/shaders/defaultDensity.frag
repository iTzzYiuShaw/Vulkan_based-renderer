#version 460 
#extension GL_KHR_vulkan_glsl : enable


layout(location = 0) in float meshDensity;

layout( location = 0 ) out vec4 oColor; 

void main() 
{  


    //Get the greatest gradient between screenX direction and screenY direction
    float density = meshDensity;

    oColor = vec4 (density,0.6 * density,1-density,1.0);
} 