float4 main(float4 pos : SV_POSITION) : SV_TARGET
{
    return float4(pos.xyz * 0.001f, 1);
}
