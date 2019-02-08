#pragma once

#include "painting3/Light.h"

#include <SM_Vector.h>

namespace pt3
{

class DirectionalLight : public Light
{
public:
	DirectionalLight() {}

    auto& GetDirection() const { return m_direction; }
    void  SetDirection(const sm::vec3& direction) { m_direction = direction; }

private:
	sm::vec3 m_direction;

    RTTR_ENABLE(Light)

}; // DirectionalLight

}