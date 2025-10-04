//***************************************************************************************
// Transform.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// 描述对象缩放、旋转(欧拉角为基础)、平移
// Provide 1st person(free view) and 3rd person cameras.
//***************************************************************************************

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <DirectXMath.h>

class Transform
{
public:
    Transform() = default; // 默认构造函数
    Transform(const DirectX::XMFLOAT3& scale,
              const DirectX::XMFLOAT3& rotation,
              const DirectX::XMFLOAT3& position); // 构造函数
    ~Transform() = default; // 析构函数

    Transform(const Transform&) = default; // 拷贝构造函数
    Transform& operator=(const Transform&) = default; // 赋值操作符

    Transform(Transform&&) = default; // 移动构造函数
    Transform& operator=(Transform&&) = default; // 移动赋值操作符


    DirectX::XMFLOAT3 GetScale() const; // 获取缩放
    DirectX::XMVECTOR GetScaleXM() const; // 获取缩放(DirectX::XMFLOAT3 -> DirectX::XMVECTOR)

    DirectX::XMFLOAT3 GetRotation() const; // 获取旋转(欧拉角, 弧度制) - 顺序Z-X-Y
    DirectX::XMVECTOR GetRotationXM() const; // 获取旋转(欧拉角, 弧度制) - 顺序Z-X-Y (DirectX::XMFLOAT3 -> DirectX::XMVECTOR)

    DirectX::XMFLOAT3 GetPosition() const; // 获取位置
    DirectX::XMVECTOR GetPositionXM() const; // 获取位置(DirectX::XMFLOAT3 -> DirectX::XMVECTOR)

    DirectX::XMFLOAT3 GetRightAxis() const; // 获取右轴
    DirectX::XMVECTOR GetRightAxisXM() const; // 获取右轴(DirectX::XMFLOAT3 -> DirectX::XMVECTOR)

    DirectX::XMFLOAT3 GetUpAxis() const; // 获取上轴
    DirectX::XMVECTOR GetUpAxisXM() const; // 获取上轴(DirectX::XMFLOAT3 -> DirectX::XMVECTOR)

    DirectX::XMFLOAT3 GetForwardAxis() const; // 获取前轴
    DirectX::XMVECTOR GetForwardAxisXM() const; // 获取前轴(DirectX::XMFLOAT3 -> DirectX::XMVECTOR)

    DirectX::XMFLOAT4X4 GetLocalToWorldMatrix() const; // 获取世界变换矩阵
    DirectX::XMMATRIX GetLocalToWorldMatrixXM() const; // 获取世界变换矩阵(DirectX::XMFLOAT4X4 -> DirectX::XMMATRIX)

    DirectX::XMFLOAT4X4 GetWorldToLocalMatrix() const; // 获取局部变换矩阵(世界矩阵的逆矩阵)
    DirectX::XMMATRIX GetWorldToLocalMatrixXM() const; // 获取局部变换矩阵(世界矩阵的逆矩阵)(DirectX::XMFLOAT4X4 -> DirectX::XMMATRIX)


    void SetScale(const DirectX::XMFLOAT3& scale); // 设置缩放
    void SetScale(float x, float y, float z); // 设置缩放

    void SetRotation(const DirectX::XMFLOAT3& eulerAnglesInRadian); // 设置旋转(欧拉角, 弧度制) - 顺序Z-X-Y
    void SetRotation(float x, float y, float z); // 设置旋转(欧拉角, 弧度制) - 顺序Z-X-Y

    void SetPosition(const DirectX::XMFLOAT3& position); // 设置位置
    void SetPosition(float x, float y, float z); // 设置位置

    void Rotate(const DirectX::XMFLOAT3& eulerAnglesInRadian); // 欧拉角旋转 (欧拉角, 弧度制) - 顺序Z-X-Y
    void RotateAxis(const DirectX::XMFLOAT3& axis, float radian); // 绕轴旋转 (弧度制)
    void RotateAround(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& axis, float radian); // 指定以point为旋转中心绕轴旋转 (弧度制)

    void Translate(const DirectX::XMFLOAT3& direction, float magnitude); // 沿着某一方向平移

    void LookAt(const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up = {0.0f, 1.0f, 0.0f}); // 朝向目标点
    void LookTo(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& up = {0.0f, 1.0f, 0.0f}); // 沿着某一方向朝向

private:
    DirectX::XMFLOAT3 GetEulerAnglesFromRotationMatrix(const DirectX::XMFLOAT4X4& rotationMatrix); // 从旋转矩阵获取旋转欧拉角

// 参数成员
private:
    DirectX::XMFLOAT3 m_Scale = {1.0f, 1.0f, 1.0f}; // 缩放
    DirectX::XMFLOAT3 m_Rotation= {}; // 旋转
    DirectX::XMFLOAT3 m_Position = {}; // 位置

};

#endif