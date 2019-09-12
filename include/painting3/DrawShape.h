#pragma once

#include <SM_Matrix.h>

namespace tess { class Painter; }
namespace gs { class Shape3D; }

namespace pt3
{

class Viewport;

class DrawShape
{
public:
	static void Draw(tess::Painter& pt, const gs::Shape3D& shape,
        const Viewport& vp, const sm::mat4& cam_mat, 
        float radius = 10.0f, uint32_t col = 0xffffffff);

}; // DrawShape

}