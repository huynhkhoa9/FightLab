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

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model  * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord ;
}
