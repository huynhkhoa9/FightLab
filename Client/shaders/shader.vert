#version 450
#extension GL_ARB_separate_shader_objects : enable

const int MAX_JOINTS = 64;
const int MAX_WEIGHTS = 4;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(std430, set = 1, binding = 0) readonly buffer JointMatrices {
	mat4 jointMatrices[];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec4 inBoneWeights;
layout(location = 5) in ivec4 inBoneIDs;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
	mat4 model = mat4(1);
	// Calculate skinned matrix from weights and joint indices of the current vertex
	mat4 skinMat = 
		inBoneWeights.x * jointMatrices[int(inBoneIDs.x)];// +
		//inBoneWeights.y * jointMatrices[int(inBoneIDs.y)] +
		//inBoneWeights.z * jointMatrices[int(inBoneIDs.z)] +
		//inBoneWeights.w * jointMatrices[int(inBoneIDs.w)];

	
    gl_Position = ubo.proj * ubo.view * model  * skinMat* vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord ;
}
