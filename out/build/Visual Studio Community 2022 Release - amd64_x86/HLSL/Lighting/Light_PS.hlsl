#include "Light.hlsli"

//PS
float4 PS(VertexOut pIn) : SV_Target
{
    pIn.normalW = normalize(pIn.normalW);

    float3 toEyeW = normalize(g_EyePosW - pIn.posW);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float4 tempAmbient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 tempDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 tempSpec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComputeDirectionalLight(g_Material, g_DirLight, pIn.normalW,toEyeW, tempAmbient, tempDiffuse, tempSpec);
    ambient += tempAmbient;
    diffuse += tempDiffuse;
    spec += tempSpec;

    ComputePointLight(g_Material, g_PointLight, pIn.posW, pIn.normalW, toEyeW, tempAmbient, tempDiffuse, tempSpec);
    ambient += tempAmbient;
    diffuse += tempDiffuse;
    spec += tempSpec;

    ComputeSpotLight(g_Material, g_SpotLight, pIn.posW, pIn.normalW, toEyeW, tempAmbient, tempDiffuse, tempSpec);
    ambient += tempAmbient;
    diffuse += tempDiffuse;
    spec += tempSpec;

    float4 litColor = pIn.color * (ambient + diffuse) + spec;

    litColor.a = g_Material.diffuse.a * pIn.color.a;

    return litColor;

}