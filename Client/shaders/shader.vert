#version 450
#extension GL_ARB_separate_shader_objects : enable

const int MAX_JOINTS = 64;
const int MAX_WEIGHTS = 4;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	mat4 joints[MAX_JOINTS];
} ubo;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec4 inBoneWeights;
layout(location = 5) in ivec4 inBoneIDs;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

out gl_PerVertex 
{
	vec4 gl_Position;   
};

void main() {
	mat4 JointTransform = ubo.joints[inBoneIDs[0]] * inBoneWeights[0];
	JointTransform     += ubo.joints[inBoneIDs[1]] * inBoneWeights[1];
	JointTransform     += ubo.joints[inBoneIDs[2]] * inBoneWeights[2];
	JointTransform     += ubo.joints[inBoneIDs[3]] * inBoneWeights[3];

    gl_Position = ubo.proj * ubo.view * ubo.model * JointTransform * vec4(inPosition.xyz, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord ;
}
