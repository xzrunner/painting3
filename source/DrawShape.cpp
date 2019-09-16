#include "painting3/DrawShape.h"
#include "painting3/Viewport.h"

#include <geoshape/Point3D.h>
#include <geoshape/Polyline3D.h>
#include <tessellation/Painter.h>

namespace pt3
{

void DrawShape::Draw(tess::Painter& pt, const gs::Shape3D& shape,
                     const Viewport& vp, const sm::mat4& cam_mat,
                     float radius, uint32_t col)
{
    auto trans3d = [&](const sm::vec3& pos3)->sm::vec2 {
        return vp.TransPosProj3ToProj2(pos3, cam_mat);
    };

    auto type = shape.get_type();
    if (type == rttr::type::get<gs::Point3D>())
    {
        auto& p3d = static_cast<const gs::Point3D&>(shape);
        pt.AddCircleFilled(trans3d(p3d.GetPos()), radius, col);
    }
    else if (type == rttr::type::get<gs::Polyline3D>())
    {
        auto& polyline = static_cast<const gs::Polyline3D&>(shape);
        auto& vertices = polyline.GetVertices();
        pt.AddPolyline3D(&vertices[0], vertices.size(), trans3d, col);
    }
}

}