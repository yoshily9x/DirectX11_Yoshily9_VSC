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
    : D3DApp(hInstance, windowName, initWidth, initHeight), m_CBuffer()
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

// 每帧更新
void GameApp::UpdateScene(float dt)
{
    //----------------------ImGui示例代码-----------------------
    // ImGui示例窗口
    //ImGui::ShowAboutWindow();
    //ImGui::ShowDemoWindow();
    //ImGui::ShowUserGuide();

    // 获取IO事件
    ImGuiIO& io = ImGui::GetIO();
    //
    // 自定义窗口与操作
    //
    static float tx = 0.0f, ty = 0.0f, phi = 0.0f, theta = 0.0f, scale = 1.0f, fov = XM_PIDIV2;
    static bool animateCube = true, customColor = false;
    if (animateCube)
    {
        phi += 0.3f * dt, theta += 0.37f * dt;
        phi = XMScalarModAngle(phi);
        theta = XMScalarModAngle(theta);
    }
    if (ImGui::Begin("Use ImGui"))
    {
        ImGui::Checkbox("Animate Cube", &animateCube);// 复选框
        ImGui::SameLine(0.0f, 25.0f);// 下一个控件在同一行往右25像素单位
        if (ImGui::Button("Reset Params"))// 按钮
        {
            tx = ty = phi = theta = 0.0f;
            scale = 1.0f;
            fov = XM_PIDIV2;
        }
        ImGui::SliderFloat("Scale", &scale, 0.2f, 2.0f);// 拖动控制物体大小

        ImGui::Text("Phi: %.2f degrees", XMConvertToDegrees(phi));// 显示文字，用于描述下面的控件 
        ImGui::SliderFloat("##1", &phi, -XM_PI, XM_PI, "");     // 不显示文字，但避免重复的标签
        ImGui::Text("Theta: %.2f degrees", XMConvertToDegrees(theta));
        ImGui::SliderFloat("##2", &theta, -XM_PI, XM_PI, "");

        ImGui::Text("Position: (%.1f, %.1f, 0.0)", tx, ty);

        ImGui::Text("FOV: %.2f degrees", XMConvertToDegrees(fov));
        ImGui::SliderFloat("##3", &fov, XM_PIDIV4, XM_PI / 3 * 2, "");
        
        // Custom color functionality removed - ConstantBuffer structure doesn't support it
        if (ImGui::Checkbox("Use Custom Color", &customColor))
            m_CBuffer.useCustomColor = customColor;  
        // 下面的控件受上面的复选框影响
        if (customColor)
        {
            ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&m_CBuffer.color));// 编辑颜色
        }
    }
    ImGui::End();

    // 不允许在操作UI时操作物体
    if (!ImGui::IsAnyItemActive())
    {
        // 鼠标左键拖动平移
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            tx += io.MouseDelta.x * 0.01f;
            ty -= io.MouseDelta.y * 0.01f;
        }
        // 鼠标右键拖动旋转
        else if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            phi -= io.MouseDelta.y * 0.01f;
            theta -= io.MouseDelta.x * 0.01f;
            phi = XMScalarModAngle(phi);
            theta = XMScalarModAngle(theta);
        }
        // 鼠标滚轮缩放
        else if (io.MouseWheel != 0.0f)
        {
            scale += 0.02f * io.MouseWheel;
            if (scale > 2.0f)
                scale = 2.0f;
            else if (scale < 0.2f)
                scale = 0.2f;
        }
    }

    m_CBuffer.world = XMMatrixTranspose(
        XMMatrixScalingFromVector(XMVectorReplicate(scale)) * 
        XMMatrixRotationX(phi) * XMMatrixRotationY(theta) * 
        XMMatrixTranslation(tx, ty, 0.0f)); // 更新世界矩阵
    m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(fov, AspectRatio(), 1.0f, 1000.0f)); //更新投影矩阵
    // 更新常量缓冲区，让立方体转起来
    D3D11_MAPPED_SUBRESOURCE mappedData; // 映射子资源
    HR(m_pd3dImmediateContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData)); // 映射，获取指向缓冲区中数据的指针并拒绝GPU访问
    memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer)); // 复制数据
    m_pd3dImmediateContext->Unmap(m_pConstantBuffer.Get(), 0); // 解除映射，让指向资源的指针无效并重启GPU的访问
}

// 每帧绘制
void GameApp::DrawScene()
{
    // assert函数在调试时有效, 用于检查条件是否为真, 如果条件为假, 程序会中断并报告错误信息
    assert(m_pd3dImmediateContext);// 设备上下文，d3dApp类中创建
    assert(m_pSwapChain);// 交换链，d3dApp类中创建
    
    static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };// RGBA = (0,0,0,255) 清屏颜色
    m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);// 用黑色清屏
    m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);// 清除深度模板缓冲区

    // 绘制三角形
    //m_pd3dImmediateContext->Draw(3, 0);//绘制三角形, 3个顶点, 从第0个顶点开始

    // 绘制立方体
    m_pd3dImmediateContext->DrawIndexed(36, 0, 0);// 绘制立方体, 36个顶点, 从第0个顶点开始

    // 绘制ImGui
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    HR(m_pSwapChain->Present(0, 0));//交换前后缓冲区并显示到屏幕上
}

bool GameApp::InitEffect()
{
    ComPtr<ID3DBlob> blob;


    // 创建顶点着色器
    //HR(CreateShaderFromFile(L"HLSL\\Triangle\\Triangle_VS.cso", L"HLSL\\Triangle\\Triangle_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf())); // 三角形顶点着色器
    HR(CreateShaderFromFile(L"HLSL\\Cube\\Cube_VS.cso", L"HLSL\\Cube\\Cube_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf())); // 立方体顶点着色器
    HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
    // 创建并绑定顶点布局
    HR(m_pd3dDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));

    // 创建像素着色器
    //HR(CreateShaderFromFile(L"HLSL\\Triangle\\Triangle_PS.cso", L"HLSL\\Triangle\\Triangle_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf())); // 三角形像素着色器
    HR(CreateShaderFromFile(L"HLSL\\Cube\\Cube_PS.cso", L"HLSL\\Cube\\Cube_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf())); // 立方体像素着色器
    HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));

    return true;
}

bool GameApp::InitResource()
{
    // **********顶点缓冲区**********
    // 创建一组三角形顶点
    /*VertexPosColor vertices[] =
    {
        { XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
    };*/

    /* 创建一组立方体顶点
       设置立方体顶点
        5________ 6
        /|      /|
       /_|_____/ |
      1|4|_ _ 2|_|7
       | /     | /
       |/______|/
      0       3
    */
    VertexPosColor vertices[] ={
        //坐标                          //颜色
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f) }
    };

    // 设置顶点缓冲区描述
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;//不可修改，传入的是固定顶点
    vbd.ByteWidth = sizeof vertices;//缓冲区大小
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//顶点缓冲区
    vbd.CPUAccessFlags = 0;//CPU不需要访问

    //新建顶点缓冲区
    D3D11_SUBRESOURCE_DATA InitData;//初始化数据
    ZeroMemory(&InitData, sizeof(InitData));//清空
    InitData.pSysMem = vertices;//初始化数据, 传入顶点
    HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));//创建顶点缓冲区


    // **********索引缓冲区**********
    // 索引数组
    DWORD indices[] = {
        // 正面
        0, 1, 2,
        2, 3, 0,
        // 左面
        4, 5, 1,
        1, 0, 4,
        // 顶面
        1, 5, 6,
        6, 2, 1,
        // 背面
        7, 6, 5,
        5, 4, 7,
        // 右面
        3, 2, 6,
        6, 7, 3,
        // 底面
        4, 0, 3,
        3, 7, 4
    };

    // 设置索引缓冲区描述
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;//不可修改，
    ibd.ByteWidth = sizeof indices;//缓冲区大小
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;//索引缓冲区
    ibd.CPUAccessFlags = 0;//CPU不需要访问

    // 新建索引缓冲区
    InitData.pSysMem = indices;//初始化数据, 传入索引
    HR(m_pd3dDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));//创建索引缓冲区


    // **********常量缓冲区**********
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DYNAMIC;//动态使用，CPU需要修改
    cbd.ByteWidth = sizeof(ConstantBuffer);//常量缓冲区大小
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//常量缓冲区
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//CPU需要写入
    
    // 新建常量缓冲区
    HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffer.GetAddressOf()));//创建常量缓冲区

    // 初始化常量缓冲区值
    m_CBuffer.world = XMMatrixIdentity(); //单位矩阵
    m_CBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f), // 眼睛位置
        XMVectorSet(0.0f, 0.0f, 0.0f ,0.0f),  // 观察目标点
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // UpDirection
    ));
    m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
    m_CBuffer.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_CBuffer.useCustomColor = false;


    // **********给渲染管线各个阶段绑定好所需资源**********
    //输入装配阶段的顶点缓冲区设置
    UINT stride = sizeof(VertexPosColor);//步长, 每个顶点的字节数
    UINT offset = 0;//偏移量, 从顶点数组的开头开始

    m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);// 绑定顶点缓冲区到输入装配阶段
    m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);// 绑定索引缓冲区到输入装配阶段
    //设置图元类型，设置输入布局
    m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);// 图元类型为三角形列表
    m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());// 设置输入布局


    //将着色器绑定到渲染管线
    m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    // 将更新好的常量缓冲区绑定到顶点着色器
    m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
    m_pd3dImmediateContext->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());//如果像素着色器有调用到常量缓冲区数据，就要绑定

    m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

    
    // 设置调试对象名

    D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosColorLayout");
    D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
    //D3D11SetDebugObjectName(m_pVertexShader.Get(), "Trangle_VS");
    //D3D11SetDebugObjectName(m_pPixelShader.Get(), "Trangle_PS");
    D3D11SetDebugObjectName(m_pIndexBuffer.Get(), "IndexBuffer");
    D3D11SetDebugObjectName(m_pConstantBuffer.Get(), "ConstantBuffer");
    D3D11SetDebugObjectName(m_pVertexShader.Get(), "Cube_VS");
    D3D11SetDebugObjectName(m_pPixelShader.Get(), "Cube_PS");


    return true;
}
