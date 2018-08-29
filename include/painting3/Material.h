#pragma once

#include <SM_Vector.h>
#include <unirender/Texture.h>

namespace pt3
{

class Material
{
public:
	sm::vec3 ambient = { 0.04f, 0.04f, 0.04f };
	sm::vec3 diffuse = { 1, 1, 1 };
	sm::vec3 specular = { 1, 1, 1 };
	float    shininess = 50;

	ur::TexturePtr diffuse_tex = nullptr;

}; // Material

}