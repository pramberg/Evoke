#include "TestShaderInclude.hlsli"

cbuffer GlobalShaderData : register(b0)
{
    float4x4 View;
    float4x4 Projection;
    float4x4 ViewProjection;
    float GameTime;
};

float4 VSMain(float4 inPosition : POSITION) : SV_POSITION
{
    return mul(float4(inPosition.xyz, 1.0f), ViewProjection);
}

float4 PSMain(float4 inPosition : SV_POSITION) : SV_TARGET
{
    return float4(sin(GameTime) * 0.5 + 0.5, cos(GameTime) * 0.5 + 0.5, cos(GameTime + 3.14 * 0.5) * 0.5 + 0.5, 1);
}
