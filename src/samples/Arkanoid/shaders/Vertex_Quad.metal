#include "../../../../libs/mmpengine/src/Backend/Metal/Shader.h"
#include "../Shader.h"

vertex VertexOut VertexQuad(
    VertexIn in                                     [[stage_in]],
    uint instanceId                                 [[instance_id]],
    constant CameraData&          cameraData        [[ buffer(1) ]],
    device const InstanceData*          instanceData      [[ buffer(2) ]]
)
{
    VertexOut out;
    
    float4 worldPos = float4(in.position, 1.0f) *  instanceData[instanceId].l2w;
    float4 viewPos = worldPos * cameraData.viewMat;
    out.position = viewPos * cameraData.projMat;
    out.color = instanceData[instanceId].color;
    
    return out;
}
