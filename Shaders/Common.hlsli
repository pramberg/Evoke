cbuffer GlobalShaderData : register(b0)
{
    float4x4 View;
    float4x4 Projection;
    float4x4 ViewProjection;
    float GameTime;
};

struct Math
{
    static const float Pi = 3.141592f;
    static const float Tau = 6.283184f;
};