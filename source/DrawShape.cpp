#include "painting3/DrawShape.h"
#include "painting3/Viewport.h"

#include <geoshape/Box.h>
#include <geoshape/Point3D.h>
#include <geoshape/Line3D.h>
#include <geoshape/Polyline3D.h>
#include <geoshape/Polygon3D.h>
#include <tessellation/Painter.h>

namespace pt3
{

void DrawShape::Draw(tess::Painter& pt, const gs::Shape3D& shape,
                     const Viewport& vp, const sm::mat4& cam_mat,
                     float radius, uint32_t col, bool draw_ctrl_node)
{
    auto trans3d = [&](const sm::vec3& pos3)->sm::vec2 {
        return vp.TransPosProj3ToProj2(pos3, cam_mat);
    };

    auto type = shape.get_type();
    if (type == rttr::type::get<gs::Box>())
    {
        auto& box = static_cast<const gs::Box&>(shape);
        pt.AddCube(box.GetCube(), trans3d, col);
    }
    else if (type == rttr::type::get<gs::Point3D>())
    {
        auto& p = static_cast<const gs::Point3D&>(shape);
        pt.AddCircleFilled(trans3d(p.GetPos()), radius, col);
    }
    else if (type == rttr::type::get<gs::Line3D>())
    {
        auto& line = static_cast<const gs::Line3D&>(shape);
        pt.AddLine(trans3d(line.GetStart()), trans3d(line.GetEnd()), col);
    }
    else if (type == rttr::type::get<gs::Polyline3D>())
    {
        auto& polyline = static_cast<const gs::Polyline3D&>(shape);
        auto& vertices = polyline.GetVertices();
        if (!vertices.empty())
        {
            pt.AddPolyline3D(&vertices[0], vertices.size(), trans3d, col,
                tess::DEFAULT_LINE_WIDTH, polyline.GetClosed());
            if (draw_ctrl_node) {
                for (auto& v : vertices) {
                    pt.AddCircleFilled(trans3d(v), radius, col);
                }
            }
        }
    }
    else if (type == rttr::type::get<gs::Polygon3D>())
    {
        auto& polygon = static_cast<const gs::Polygon3D&>(shape);
        auto& vertices = polygon.GetVertices();
        if (!vertices.empty()) {
            pt.AddPolygonFilled3D(&vertices[0], vertices.size(), trans3d, col);
        }
    }}

}