#include "../../../../libs/mmpengine/src/Backend/Metal/Shader.h"
#include "../Shader.h"

fragment float4 PixelQuad(
    VertexOut in                                    [[stage_in]],
    constant CameraData&          cameraData        [[ buffer(0) ]],
    device const InstanceData*    instanceData      [[ buffer(1) ]]
)
{
    return in.color;
}
