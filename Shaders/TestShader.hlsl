float4 VSMain(float4 InPosition : POSITION) : SV_POSITION
{
    return InPosition + float4(0,0,0,0);
}

float4 PSMain(float4 pos : SV_POSITION) : SV_TARGET
{
    return float4(pos.xyz * 0.001f, 1);
}
