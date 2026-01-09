struct VSOut
{
    float3 color : Color;
    float4 position : SV_Position;
};

cbuffer CBuf
{
    matrix transform;
};

VSOut main_vs(float3 position : Position, float3 color : Color)
{
    VSOut vso;
    vso.position = mul(float4(position, 1.0f), transform);
    vso.color = color;
    
    return vso;
}

float4 main_ps(float3 color : Color) : SV_Target
{
    return float4(color, 1.0f);
}