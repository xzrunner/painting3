#pragma once

#include <unirender/Texture3D.h>

#include <stdint.h>

namespace pt3
{

class Texture3D : public ur::Texture3D
{
public:
	Texture3D(ur::RenderContext* rc, uint16_t w, uint16_t h, uint16_t d, uint32_t id, int format);

}; // Texture3D

}