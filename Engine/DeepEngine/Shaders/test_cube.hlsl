cbuffer CBuf
{
    matrix transform;
};

cbuffer ColorBuffer
{
    float4 face_colors[6];
};

float4 main_vs(float3 position : Position) : SV_Position
{
    return mul(float4(position, 1.0f), transform);
}

float4 main_ps(uint tid : SV_PrimitiveID) : SV_Target
{
    return face_colors[tid / 2];
}