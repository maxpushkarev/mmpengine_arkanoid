#include "../../../../libs/mmpengine/src/Backend/Dx12/Shader.h"
#include "../Shader.h"

VertexOut EntryPoint(VertexIn vin)
{
    VertexOut vout;

    float4 worldPos = mul(float4(vin.pos, 1.0f), instanceData[vin.instanceId].l2w);
    float4 viewPos = mul(worldPos, cameraData.viewMat);
    vout.pos = mul(viewPos, cameraData.projMat);
    vout.col = instanceData[vin.instanceId].color;
    return vout;
}