#version 450
#include "../../../../libs/mmpengine/src/Backend/Vulkan/Shader.h"
#include "../Shader.h"

layout(location = 0) in vec3 inPosition;
layout(location = 0) out vec4 color;

void main() {
	vec4 worldPos = vec4(inPosition, 1.0f) * objectData.instanceData[gl_InstanceIndex].l2w;
	vec4 viewPos = worldPos * sceneData.cameraData.viewMat;
	gl_Position = viewPos * sceneData.cameraData.projMat;
	color = objectData.instanceData[gl_InstanceIndex].color;
}