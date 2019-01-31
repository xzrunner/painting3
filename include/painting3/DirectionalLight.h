#pragma once

#include "painting3/Light.h"

#include <SM_Vector.h>

namespace pt3
{

class DirectionalLight : public Light
{
public:
	DirectionalLight() {}

private:
	sm::vec3 m_direction;

}; // DirectionalLight

}