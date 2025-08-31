#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;//使用DirectX命名空间, 方便使用XMFLOAT3等类型

//顶点结构体对应shader, 描述成员用途
const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


GameApp::GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : D3DApp(hInstance, windowName, initWidth, initHeight)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
    if (!D3DApp::Init())
        return false;

    if (!InitEffect())
        return false;

    if (!InitResource())
        return false;

    return true;
}


void GameApp::OnResize()
{
    D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
}

void GameApp::DrawScene()
{
    // assert函数在调试时有效, 用于检查条件是否为真, 如果条件为假, 程序会中断并报告错误信息
    assert(m_pd3dImmediateContext);// 设备上下文，d3dApp类中创建
    assert(m_pSwapChain);// 交换链，d3dApp类中创建
    
    static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };// RGBA = (0,0,0,255) 清屏颜色
    m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);// 用黑色清屏
    m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);// 清除深度模板缓冲区

    // 绘制三角形
    m_pd3dImmediateContext->Draw(3, 0);//绘制三角形, 3个顶点, 从第0个顶点开始
    HR(m_pSwapChain->Present(0, 0));//交换前后缓冲区并显示到屏幕上
}

bool GameApp::InitEffect()
{
    ComPtr<ID3DBlob> blob;

    // 创建顶点着色器
    HR(CreateShaderFromFile(L"HLSL\\Triangle_VS.cso", L"HLSL\\Triangle_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
    // 创建并绑定顶点布局
    HR(m_pd3dDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));

    // 创建像素着色器
    HR(CreateShaderFromFile(L"HLSL\\Triangle_PS.cso", L"HLSL\\Triangle_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));

    return true;
}

bool GameApp::InitResource()
{
    // 创建一组三角形顶点
    VertexPosColor vertices[] =
    {
        { XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
    };
    // 设置顶点缓冲区描述
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;//不可修改，传入的是固定三角形顶点
    vbd.ByteWidth = sizeof vertices;//缓冲区大小
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;

    //新建顶点缓冲区
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;//初始化数据, 传入三角形顶点
    HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));

    //输入装配阶段的顶点缓冲区设置
    UINT stride = sizeof(VertexPosColor);//步长, 每个顶点的字节数
    UINT offset = 0;//偏移量, 从顶点数组的开头开始

    m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);//绑定顶点缓冲区到输入装配阶段
    //设置图元类型，设置输入布局
    m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//图元类型为三角形列表
    m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());//设置输入布局
    //将着色器绑定到渲染管线
    m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

    // 设置调试对象名
    //
    D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosColorLayout");
    D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
    D3D11SetDebugObjectName(m_pVertexShader.Get(), "Trangle_VS");
    D3D11SetDebugObjectName(m_pPixelShader.Get(), "Trangle_PS");

    return true;
}