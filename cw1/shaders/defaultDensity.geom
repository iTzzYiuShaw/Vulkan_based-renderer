#version 460 
#extension GL_KHR_vulkan_glsl : enable


layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


layout( location = 0 ) in vec3 v2fTexCoord[];
layout( location = 1 )in vec3 v2fDifOrCons[];
layout (location = 2) in vec3 v2fPosition[];

layout(location = 0) out float meshDensity;


float triangleArea(vec3 a, vec3 b, vec3 c) {
    return 0.5 * length(cross(b.xyz - a.xyz, c.xyz - a.xyz));
}

vec3 clipToScreen(vec4 clipPos)
{
    vec3 ndcPos = clipPos.xyz / clipPos.w;
    vec2 screenSize = vec2(1920, 1080);
    vec2 screenPos = ((ndcPos.xy + vec2(1.0)) / 2.0) * screenSize;
    return vec3(screenPos, ndcPos.z);
}



void main() 
{  

    vec3 screenPos0 = clipToScreen(gl_in[0].gl_Position);
    vec3 screenPos1 = clipToScreen(gl_in[1].gl_Position);
    vec3 screenPos2 = clipToScreen(gl_in[2].gl_Position);


    float area = triangleArea(screenPos0, screenPos1, screenPos2);
    
    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position;
        meshDensity = area * 0.01;
        EmitVertex();
    }
    EndPrimitive();
} 