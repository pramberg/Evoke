#include "TestShaderInclude.hlsli"

float4 VSMain(float4 inPosition : POSITION) : SV_POSITION
{
    return inPosition;
}

cbuffer TestBuffer : register(b0)
{
    float4 Red;
};

cbuffer TestBuffer2 : register(b1)
{
    float4 Green;
    float4 Blue;
};

float4 PSMain(float4 inPosition : SV_POSITION) : SV_TARGET
{
    return float4(Red.xyz, 1);
}
