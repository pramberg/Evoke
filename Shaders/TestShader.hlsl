#include "TestShaderInclude.hlsli"

float4 VSMain(float4 inPosition : POSITION) : SV_POSITION
{
    return inPosition;
}

cbuffer TestBuffer : register(b0)
{
    float4 Something;
};

cbuffer TestBuffer2 : register(b1)
{
    float4 SomethingElse;
    float4 SomethingElseAgain;
    float Test[16];
};

float4 PSMain(float4 inPosition : SV_POSITION) : SV_TARGET
{
    return float4(Something.xyz * SomethingElseAgain.xyz, 1);
}
