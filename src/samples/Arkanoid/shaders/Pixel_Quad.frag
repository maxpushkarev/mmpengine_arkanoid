#version 450
#include "../../../../libs/mmpengine/src/Backend/Vulkan/Shader.h"
#include "../Shader.h"

layout(location = 0) in vec4 inColor;
layout(location = 0) out vec4 outColor;


void main() {
	outColor = inColor;
}