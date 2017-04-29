#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in float vertexExpandDirection;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//hair���f���v�Z�p
uniform mat4 normalMatrix;
uniform vec3 lightPosition;
uniform float cylinderFlag;

//�w�A�Ǘ��p
uniform float thickness;
uniform float hairFlag;

void main()
{
	

	if (hairFlag == 1) {
		if (cylinderFlag == 1.0) {
			vec3 movedPosition = vertexPosition + vertexNormal * thickness;
			gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(movedPosition, 1.0);	
		} else {
			vec3 tangent = (normalMatrix * vec4(vertexTangent, 1.0)).xyz;//global

			vec4 nowGlobalPos = modelMatrix * vec4(vertexPosition, 1.0);
			vec3 viewVec = lightPosition - nowGlobalPos.xyz;
			viewVec = normalize(viewVec);
		
			vec3 binormal = cross(viewVec, tangent);
			
			vec3 offset = binormal * vertexExpandDirection * thickness;

			vec4 movedPosition = vec4(offset, 0.0) + nowGlobalPos;
			
			gl_Position = projectionMatrix * viewMatrix * movedPosition;
		}
	} else {
		//�f�v�X�}�b�v�쐬�͈ʒu���ړ����āA��͎����B
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	}
}