#version 460

#extension GL_KHR_vulkan_glsl : enable

layout( location = 0 ) in vec3 v2fTexCoord;
layout( location = 1) in vec3 v2fDifOrCons;
layout (location = 2) in vec4 v2fPosition;


layout( set = 1, binding = 0 ) uniform sampler2D uTexColor; 

layout( set = 2, binding = 0 ) uniform MaxMipLevelsInfo {

    uint maxMipLevels;

} maxMipLevelsInfo;;

layout( location = 0 ) out vec4 oColor; 

void main() 
{  

	if(v2fTexCoord.z < 0)
	{

		//this is a textureCoodinate

//		vec2 textureCoordinate = vec2(v2fTexCoord.x,v2fTexCoord.y);

//		float mipLevel = textureQueryLod(uTexColor,textureCoordinate).x;
//
//		vec3 color = mix(vec3(0.5, 0.2, 0.0).rgb, vec3(0.0, 0.5, 0.2).rgb, 1.0);
//
//		oColor = vec4(color,1.0);
		
		vec4 diffuseColor = vec4( v2fDifOrCons,1.f);
		vec2 textureCoordinate = vec2(v2fTexCoord.x,v2fTexCoord.y);

		oColor = diffuseColor * vec4( texture( uTexColor, textureCoordinate ).rgb, 1.f ) ;
	}else
	{

		oColor = vec4(v2fTexCoord,1.f);
	}


//    vec2 gradient = vec2(length(dFdx(v2fPosition.xy)), length(dFdy(v2fPosition.xy)));
//
//    float density = max(gradient.x, gradient.y);
//
//    vec3 lowDensityColor = vec3(1.0, 1.0, 1.0); 
//
//    vec3 midDensityColor = vec3(0.5, 0.0, 0.5);
//
//    vec3 highDensityColor = vec3(1.0, 0.0, 1.0);
//
//
//    vec3 color = mix(lowDensityColor, midDensityColor, density);
//    vec3 finalColor = mix(color, highDensityColor, density);
//
//    oColor = vec4(finalColor,1.0);
} 