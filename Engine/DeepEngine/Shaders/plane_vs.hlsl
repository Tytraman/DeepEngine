cbuffer PerFrame : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer PerObject : register(b1)
{
    matrix world_view_proj;
};

float4 main_vs(float3 position : Position) : SV_Position
{
    return mul(float4(position, 1.0f), world_view_proj);
}