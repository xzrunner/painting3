#pragma once

#include <painting0/Color.h>

#include <rttr/registration>

namespace pt3
{

class Light
{
public:
    Light() {}

    auto& GetColor() const { return m_color; }
    void  SetColor(const pt0::Color& color) { m_color = color; }

    float GetIntensity() const { return m_intensity; }
    void  SetIntensity(float intensity) { m_intensity = intensity; }

private:
    pt0::Color m_color;
    float m_intensity = 1.0f;

    RTTR_ENABLE()

}; // Light

}