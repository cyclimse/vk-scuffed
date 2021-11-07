#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
mat4 model;
mat4 view;
mat4 proj;
} ubo;

struct ObjectData{
	mat4 model;
};

//all object matrices
layout(std140,set = 1, binding = 0) readonly buffer ObjectBuffer{
	ObjectData objects[];
} objectBuffer;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    mat4 modelMatrix = objectBuffer.objects[gl_BaseInstance].model;
    gl_Position = ubo.proj * ubo.view * modelMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
}
