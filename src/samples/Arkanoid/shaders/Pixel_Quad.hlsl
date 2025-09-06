#include "../../../../libs/mmpengine/src/Backend/Dx12/Shader.h"
#include "../Shader.h"

float4 EntryPoint(VertexOut pin) : SV_Target
{
    return pin.col;
}
