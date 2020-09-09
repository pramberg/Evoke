// Based on Blender's grid shader.
// https://github.com/blender/blender/blob/master/source/blender/draw/engines/overlay/shaders/grid_frag.glsl
#include "Common.hlsli"

cbuffer ColorCB : register(b1)
{
    float3 Color;
}

struct PixelInput
{
    float4 Position : SV_POSITION;
    float3 WorldPosition : WORLD_POSITION;
};

PixelInput VSMain(float3 LocalPosition : POSITION)
{
    PixelInput output;
    output.WorldPosition = float3(CameraPosition.x, 0.0f, CameraPosition.z) - LocalPosition * 200.0f;
    output.Position = mul(float4(output.WorldPosition, 1.0f), ViewProjection);
    return output;
}

static const float InvSqrtPi = 0.5641895835477563f; /* 1.0f / sqrt(Math::Pi) */
static const float DiscRadius = InvSqrtPi * 1.05f;
static const float GridLineStart = 0.5f - DiscRadius;
static const float GridLineEnd = 0.5f + DiscRadius;

float GetGrid(float2 inUV, float2 inFwidth, float inGridSize)
{
    float halfGridSize = inGridSize / 2.0;
    float2 gridDomain = abs(((inUV + halfGridSize) % inGridSize) - halfGridSize);
    gridDomain /= inFwidth;

    float lineDistance = min(gridDomain.x, gridDomain.y);
    return 1.0 - smoothstep(GridLineStart, GridLineEnd, lineDistance);
}

float3 GetAxes(float3 inUVW, float3 fwidthCos, float line_size)
{
    float3 axesDomain = abs(inUVW);
    axesDomain /= fwidthCos;

    return 1.0 - smoothstep(GridLineStart, GridLineEnd, axesDomain - (line_size));
}

float4 PSMain(PixelInput input) : SV_Target
{
    float3 worldPosFWidth = fwidth(input.WorldPosition);

    float3 axesDist, axesFwidth;
    axesDist.x = input.WorldPosition.z;
    axesFwidth.x = worldPosFWidth.z;
    axesDist.y = input.WorldPosition.x;
    axesFwidth.y = worldPosFWidth.x;

    float grid = GetGrid(abs(input.WorldPosition.xz), worldPosFWidth.xz, 1.0f);
    float3 axes = GetAxes(axesDist, axesFwidth, 0.25f);

    float3 outColor = Color;
    outColor = lerp(outColor, float3(0.7f, 0.2f, 0.3f), axes.x);
    outColor = lerp(outColor, float3(0.2f, 0.3f, 0.7f), axes.y);

    float3 cameraVector = CameraPosition - input.WorldPosition;
    float distanceToPixel = length(cameraVector);
    cameraVector /= distanceToPixel;

    float angle = cameraVector.y;
    angle = 1.0f - abs(angle);
    angle *= angle;

    float fade = 1.0f - angle * angle;
    fade *= smoothstep(100.0f, 25.0f, distanceToPixel);
    return float4(outColor, max(grid, max(axes.x, axes.y)) * fade);
}
