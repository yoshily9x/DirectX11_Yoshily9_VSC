#include "Basic.hlsli"

//2D Pixel Shader
float4 PS(VertexPosHTex pIn) : SV_Target
{
    return g_Tex.Sample(g_SamLinear, pIn.tex);
}