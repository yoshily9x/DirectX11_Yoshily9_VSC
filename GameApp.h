#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
// 游戏应用程序扩展类，游戏逻辑实现，继承自D3DApp
class GameApp : public D3DApp
{
public:
    // 顶点结构体对应shader
    struct VertexPosColor
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

private:
    ComPtr<ID3D11InputLayout> m_pVertexLayout;  // 顶点布局
    ComPtr<ID3D11Buffer> m_pVertexBuffer;       // 顶点缓冲区
    ComPtr<ID3D11Buffer> m_pIndexBuffer;        // 索引缓冲区
    ComPtr<ID3D11Buffer> m_pConstantBuffer;     // 常量缓冲区

    ComPtr<ID3D11VertexShader> m_pVertexShader; // 顶点着色器
    ComPtr<ID3D11PixelShader> m_pPixelShader;   // 像素着色器
    ConstantBuffer m_CBuffer;                  // 用于修改GPU常量缓冲区的变量
};


#endif