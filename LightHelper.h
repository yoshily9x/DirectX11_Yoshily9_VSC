#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <cstring>
#include <DirectXMath.h>

// Directional light
struct DirectionalLight
{
    //显式声明和默认实现所有的特殊成员函数
    DirectionalLight() = default;//默认构造函数

    DirectionalLight(const DirectionalLight&) = default;//拷贝构造函数
    DirectionalLight& operator = (const DirectionalLight&) = default;//拷贝赋值运算符
    
    DirectionalLight(DirectionalLight&&) = default;//移动构造函数
    DirectionalLight& operator = (DirectionalLight&&) = default;//移动赋值运算符

    DirectionalLight(const DirectX::XMFLOAT4& _ambient, const DirectX::XMFLOAT4& _diffuse, 
                    const DirectX::XMFLOAT4& _specular, const DirectX::XMFLOAT3& _direction) : 
                    ambient(_ambient), 
                    diffuse(_diffuse), 
                    specular(_specular), 
                    direction(_direction),
                    pad() {}//构造函数
    
    DirectX :: XMFLOAT4 ambient;
    DirectX :: XMFLOAT4 diffuse;
    DirectX :: XMFLOAT4 specular;
    DirectX :: XMFLOAT3 direction;
    float pad;
};

struct PointLight
{
    PointLight() = default;

    PointLight(const PointLight&) = default;
    PointLight& operator = (const PointLight&) = default;

    PointLight(PointLight&&) = default;
    PointLight& operator = (PointLight&&) = default;

    PointLight(const DirectX::XMFLOAT4& _ambient, const DirectX::XMFLOAT4& _diffuse,
               const DirectX::XMFLOAT4& _specular,const DirectX::XMFLOAT3& _position,
               float _range, const DirectX::XMFLOAT3& _attenuation) :
               ambient(_ambient),
               diffuse(_diffuse),
               specular(_specular),
               position(_position),
               range(_range), 
               attenuation(_attenuation), 
               pad() {}

    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;

    //pack
    DirectX::XMFLOAT3 position;
    float range;

    //pack
    DirectX::XMFLOAT3 attenuation;
    float pad;
};

struct SpotLight
{
    SpotLight() = default;

    SpotLight(const SpotLight&) = default;
    SpotLight& operator = (const SpotLight&) = default;

    SpotLight(SpotLight&&) = default;
    SpotLight& operator = (SpotLight&&) = default;

    SpotLight(const DirectX::XMFLOAT4& _ambient, const DirectX::XMFLOAT4& _diffuse,
              const DirectX::XMFLOAT4& _specular,const DirectX::XMFLOAT3& _position,
              float _range, const DirectX::XMFLOAT3& _direction, float _spot, 
              const DirectX::XMFLOAT3& _attenuation) :
              ambient(_ambient), 
              diffuse(_diffuse), 
              specular(_specular), 
              position(_position), 
              range(_range), 
              direction(_direction), 
              spot(_spot), 
              attenuation(_attenuation), 
              pad() {}

    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;

    //pack
    DirectX::XMFLOAT3 position;
    float range;

    //pack
    DirectX::XMFLOAT3 direction;
    float spot;

    //pack
    DirectX::XMFLOAT3 attenuation;
    float pad;
};


struct Material
{
    Material() = default;

    Material(const Material&) = default;
    Material& operator = (const Material&) = default;

    Material(Material&&) = default;
    Material& operator = (Material&&) = default;

    Material(const DirectX::XMFLOAT4& _ambient, const DirectX::XMFLOAT4& _diffuse,
             const DirectX::XMFLOAT4& _specular,const DirectX::XMFLOAT4& _reflect) :
             ambient(_ambient),
             diffuse(_diffuse),
             specular(_specular),
             reflect(_reflect) {}

    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;
    DirectX::XMFLOAT4 reflect;
};

#endif