#include "TestShaderInclude.hlsli"

float4 VSMain(float4 inPosition : POSITION) : SV_POSITION
{
    return inPosition;
}

float4 PSMain(float4 inPosition : SV_POSITION) : SV_TARGET
{
    return float4(inPosition / float3(1280, 720, 1.0), 1);
}
