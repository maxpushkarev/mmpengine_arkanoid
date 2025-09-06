#ifndef ARKANOID_TEST_SHADER
#define ARKANOID_TEST_SHADER 1
#include "../../../libs/mmpengine/src/Frontend/Shader.h"

#if MMPENGINE_HLSL

struct InstanceData
{
	float4x4 l2w;
	float4 color;
};

cbuffer cbPerCamera : register(b0)
{
	CameraData cameraData;
};

StructuredBuffer<InstanceData> instanceData : register(t0);

struct VertexIn
{
	float3 pos : POSITION;
	uint instanceId : SV_InstanceID;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
};

#endif


#if MMPENGINE_GLSL

struct InstanceData
{
	mat4 l2w;
	vec4 color;
};

layout(std140, set = 0, binding = 0, column_major) uniform SceneData {
	CameraData cameraData;
} sceneData;

layout(std140, set = 1, binding = 0, column_major) buffer ObjectData {
	InstanceData[] instanceData;
} objectData;


#endif

#if MMPENGINE_MSL

struct VertexIn {
	float3 position [[attribute(0)]];
};

struct VertexOut {
	float4 position [[position]];
};

#endif

#endif
