#pragma once

#include "painting3/Light.h"

#include <SM_Vector.h>

namespace pt3
{

class PointLight : public Light
{
public:
	PointLight(const sm::vec3& pos)
        : m_position(pos)
    {
    }

private:
	sm::vec3 m_position;

	float m_radius = 1.0f;

}; // PointLight

}