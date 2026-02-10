cbuffer PerObject : register(b1)
{
    matrix world_view_proj;
};

struct VSOut
{
    float2 tex : TexCoord;
    float4 position : SV_Position;
};

VSOut main_vs(float3 position : Position, float2 tex : TexCoord)
{
    VSOut vso;
    vso.position = mul(float4(position, 1.0f), world_view_proj);
    vso.tex = tex;
    
    return vso;
}

Texture2D tex : register(t0);
SamplerState splr : register(s0);

float4 main_ps(float2 tc : TexCoord) : SV_Target
{
    return tex.Sample(splr, tc);
}