#include "Triangle.hlsli"

//Pixel Shader
float4 PS(VertexOut pIn):SV_TARGET // SV_TARGET说明输出的颜色值将会直接保存到渲染目标视图的后被缓冲区对应位置
{
    return pIn.color;
}