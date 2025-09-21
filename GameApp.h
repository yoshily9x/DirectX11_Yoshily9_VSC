#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "LightHelper.h"
#include "Geometry.h"

// 游戏应用程序扩展类，游戏逻辑实现，继承自D3DApp
class GameApp : public D3DApp
{
public:
    // 顶点结构体对应shader
    /*struct VertexPosColor
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT4 color;
        static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];// 描述成员用途
    };

    // 常量缓冲区
    struct ConstantBuffer
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX proj;
        DirectX::XMFLOAT4 color;
        uint32_t useCustomColor; // 是否使用自定义颜色
        uint32_t pads[3]; // 填充32位以保证对齐
    };*/

    //VS Constant Buffer
    struct VSConstantBuffer
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX proj;
        DirectX::XMMATRIX worldInvTranspose;
    };

    //PS Constant Buffer
    struct PSConstantBuffer
    {
        DirectionalLight dirLight;
        PointLight pointLight;
        SpotLight spotLight;
        Material material;
        DirectX::XMFLOAT4 eyePos;
    };

public:
    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();// 每帧绘制

private:
    bool InitEffect();
    bool InitResource();
    bool ResetMesh(const Geometry::MeshData<VertexPosNormalColor>& meshData);

private:
    ComPtr<ID3D11InputLayout> m_pVertexLayout;	    // 顶点输入布局
    ComPtr<ID3D11Buffer> m_pVertexBuffer;			// 顶点缓冲区
    ComPtr<ID3D11Buffer> m_pIndexBuffer;			// 索引缓冲区
    ComPtr<ID3D11Buffer> m_pConstantBuffers[2];	    // 常量缓冲区
    UINT m_IndexCount;							    // 绘制物体的索引数组大小

    ComPtr<ID3D11VertexShader> m_pVertexShader;	    // 顶点着色器
    ComPtr<ID3D11PixelShader> m_pPixelShader;		// 像素着色器
    VSConstantBuffer m_VSConstantBuffer;			// 用于修改用于VS的GPU常量缓冲区的变量
    PSConstantBuffer m_PSConstantBuffer;			// 用于修改用于PS的GPU常量缓冲区的变量

    DirectionalLight m_DirLight;					// 默认环境光
    PointLight m_PointLight;						// 默认点光
    SpotLight m_SpotLight;						    // 默认汇聚光

    ComPtr<ID3D11RasterizerState> m_pRSWireframe;	// 光栅化状态: 线框模式
    bool m_IsWireframeMode;							// 当前是否为线框模式

};


#endif