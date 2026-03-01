cbuffer ColorBuffer : register(b0)
{
    float4 face_color;
};

float4 main_ps() : SV_Target
{
    return face_color;
}