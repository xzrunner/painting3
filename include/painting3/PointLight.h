#pragma once

#include "painting3/Light.h"

#include <SM_Vector.h>

namespace pt3
{

class PointLight : public Light
{
public:
    PointLight() {}
	PointLight(const sm::vec3& pos)
        : m_position(pos)
    {
    }

    auto& GetPosition() const { return m_position; }
    void  SetPosition(const sm::vec3& position) { m_position = position; }

    float GetRadius() const { return m_radius; }
    void  SetRadius(float radius) { m_radius = radius; }

private:
	sm::vec3 m_position;

	float m_radius = 1.0f;

    RTTR_ENABLE(Light)

}; // PointLight

}