#include "TestShaderInclude.hlsli"

cbuffer GlobalShaderData : register(b0)
{
    float4x4 View;
    float4x4 Projection;
    float4x4 ViewProjection;
    float GameTime;
};

struct VertexInput
{
    float3 LocalPosition : POSITION;
    float4 Color0 : COLOR;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
    float3 WorldPosition : WORLD_POSITION;
    float3 ViewPosition : VIEW_POSITION;
    float4 Color0 : COLOR0;
};

PixelInput VSMain(VertexInput inData)
{
    PixelInput outData;
    outData.Position = mul(float4(inData.LocalPosition.xyz, 1.0f), ViewProjection);
    outData.WorldPosition = inData.LocalPosition;
    outData.ViewPosition = mul(float4(inData.LocalPosition.xyz, 1.0f), View).xyz;
    outData.Color0 = inData.Color0;
    return outData;
}

static const float3 gLightDir = { 1.0f, 1.0f, 0.0f };

float4 PSMain(PixelInput inData) : SV_TARGET
{
    float4 outColor = inData.Color0 * 0.5f;
    outColor.a = 1.0f;
    outColor.xyz += float3(sin(GameTime) * 0.5 + 0.5, cos(GameTime) * 0.5 + 0.5, cos(GameTime + 3.14 * 0.5) * 0.5 + 0.5) * 0.5f;
    outColor.xyz = inData.Color0.xyz;

    outColor.xyz = saturate(dot(normalize(gLightDir), inData.Color0.xyz)) + 0.1f;

    return outColor;
}
