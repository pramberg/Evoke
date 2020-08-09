#include "TestShaderInclude.hlsli"

cbuffer GlobalShaderData : register(b0)
{
    float4x4 View;
    float4x4 Projection;
    float4x4 ViewProjection;
    float GameTime;
};

#ifndef NUM_VERTEX_COLORS
#define NUM_VERTEX_COLORS 2
#endif

#ifndef NUM_UV_SETS
#define NUM_UV_SETS 8
#endif

struct VertexInput
{
    float3 LocalPosition : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Bitangent : BITANGENT;
    float4 Color[NUM_VERTEX_COLORS] : COLOR;
    float2 UV[NUM_UV_SETS] : UV0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
    float3 WorldPosition : WORLD_POSITION;
    float3 ViewPosition : VIEW_POSITION;
    float4 Color[NUM_VERTEX_COLORS] : COLOR;
    float2 UV[NUM_UV_SETS] : UV0;
    float3 Normal : NORMAL;
};

PixelInput VSMain(VertexInput inData)
{
    PixelInput outData;
    outData.Position = mul(float4(inData.LocalPosition.xyz, 1.0f), ViewProjection);
    outData.WorldPosition = inData.LocalPosition;
    outData.ViewPosition = mul(float4(inData.LocalPosition.xyz, 1.0f), View).xyz;
    outData.Color = inData.Color;
    outData.UV = inData.UV;
    outData.Normal = inData.Normal;
    return outData;
}

static const float3 gLightDir = { 1.0f, 1.0f, 0.0f };

float4 PSMain(PixelInput inData) : SV_TARGET
{
    float4 outColor = inData.Color[0] * 0.5f;
    outColor.a = 1.0f;
    outColor.xyz += float3(sin(GameTime) * 0.5 + 0.5, cos(GameTime) * 0.5 + 0.5, cos(GameTime + 3.14 * 0.5) * 0.5 + 0.5) * 0.5f;
    outColor.xyz = inData.Color[0].xyz;

    float angle = GameTime * 0.6f;
    float3 lightDir = { cos(angle), -0.2f, sin(angle) };
    lightDir = normalize(lightDir);

    outColor.xyz = saturate(dot(normalize(lightDir), inData.Normal)) + 0.1f;
    outColor.xyz *= inData.Color[0].xyz + inData.Color[1].xyz;

    return outColor;
}
