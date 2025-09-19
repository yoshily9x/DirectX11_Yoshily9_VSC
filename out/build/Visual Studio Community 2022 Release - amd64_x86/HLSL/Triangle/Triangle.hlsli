struct VertexIn
{
    float3 pos:POSITION;
    float4 color:COLOR;
};

struct VertexOut
{   
    // SV_POSITION说明该顶点的位置从顶点着色器输出后，后续着色器都不能改变它的值，作为光栅化的输入
    // 它表示：
    // - 在顶点着色器中：变换到齐次裁剪空间的顶点位置
    // - 在像素着色器中：当前像素在屏幕上的位置
    float4 posH:SV_POSITION;
    float4 color:COLOR;
};
