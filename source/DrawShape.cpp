#include "painting3/DrawShape.h"
#include "painting3/Viewport.h"

#include <geoshape/config.h>
#include <geoshape/Point3D.h>
#include <geoshape/PointSet3D.h>
#include <geoshape/Polyline3D.h>
#include <tessellation/Painter.h>

namespace pt3
{

void DrawShape::Draw(tess::Painter& pt, const gs::Shape3D& shape,
                     const Viewport& vp, const sm::mat4& cam_mat)
{
    auto trans3d = [&](const sm::vec3& pos3)->sm::vec2 {
        return vp.TransPosProj3ToProj2(pos3, cam_mat);
    };

    auto type = shape.get_type();
    if (type == rttr::type::get<gs::Point3D>())
    {
        auto& p3d = static_cast<const gs::Point3D&>(shape);
        pt.AddCircleFilled(trans3d(p3d.GetPos()), gs::NODE_QUERY_RADIUS, 0x88888888);
    }
    else if (type == rttr::type::get<gs::PointSet3D>())
    {
        auto& pos_set = static_cast<const gs::PointSet3D&>(shape);
        auto& vertices = pos_set.GetVertices();
        for (auto& v : vertices) {
            pt.AddCircleFilled(trans3d(v), gs::NODE_QUERY_RADIUS, 0x88888888);
        }
    }
    else if (type == rttr::type::get<gs::Polyline3D>())
    {
        auto& polyline = static_cast<const gs::Polyline3D&>(shape);
        auto& vertices = polyline.GetVertices();
        pt.AddPolyline3D(&vertices[0], vertices.size(), trans3d, 0xffffffff);
    }
}

}