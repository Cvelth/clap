float4 main(float3 color : SV_Target) : SV_Target{
    float4 output;
    output = float4(color, 1.0);
    return output;
}