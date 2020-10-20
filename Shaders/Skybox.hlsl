#include "Common.hlsli"

struct VertexInput
{
    float3 LocalPosition : POSITION;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
    float3 SampleVector : TEXCOORD;
};

PixelInput VSMain(VertexInput inData)
{
    PixelInput outData;
    const float4x4 skyboxView = float4x4((float3x4)View, float4(0.0f, 0.0f, 0.0f, 0.0f));
    float4x4 skyboxProjection = mul(skyboxView, Projection);
    outData.Position = mul(float4(inData.LocalPosition.xyz, 1.0f), skyboxProjection).xyww;
    outData.SampleVector = inData.LocalPosition.xyz;
    return outData;
}

Texture2D SkyboxTexture : register(t0);
SamplerState Sampler;

float4 PSMain(PixelInput inData) : SV_TARGET
{
    float3 vec = normalize(inData.SampleVector);
    float2 uv = float2((atan2(vec.z, vec.x) / Math::Tau) + 0.5f, acos(vec.y) / Math::Pi);

    float3 color = SkyboxTexture.Sample(Sampler, uv, 0).rgb;
    color = color / (color + 1.0f);

    return float4(color, 1.0f);
}
