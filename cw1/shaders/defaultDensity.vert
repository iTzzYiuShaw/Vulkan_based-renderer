#version 450


layout( location = 0 ) in vec3 iPosition;
layout( location = 1 ) in vec3 iTexCoord;
layout( location = 2 ) in vec3 iDiffuseOrConstant;

layout( set = 0, binding = 0 ) uniform UScene
{
	mat4 camera;
	mat4 projection;
	mat4 projCam;
} uScene;


layout( location = 0 ) out vec3 v2fTexCoord;

layout( location = 1 ) out vec3 v2fDifOrCons;

layout (location = 2) out vec3 v2fPosition;

void main()
{
	v2fTexCoord = iTexCoord;
	v2fDifOrCons = iDiffuseOrConstant;
	v2fPosition = iPosition;

	gl_Position = uScene.projCam * vec4( iPosition, 1.f ); 
}
