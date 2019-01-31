#pragma once

#include <painting0/Color.h>

namespace pt3
{

class Light
{
public:
    Light() {}

private:
    pt0::Color m_color;
    float m_intensity = 1.0f;

}; // Light

}