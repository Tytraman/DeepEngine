cbuffer PerFrame : register(b0)
{
    matrix view;
    matrix projection;
};

cbuffer PerObject : register(b1)
{
    matrix world_view_proj;
};

cbuffer ColorBuffer : register(b0)
{
    float4 face_colors[6];
};

float4 main_vs(float3 position : Position) : SV_Position
{   
    return mul(float4(position, 1.0f), world_view_proj);
}

float4 main_ps(uint tid : SV_PrimitiveID) : SV_Target
{
    return face_colors[tid / 2];
}