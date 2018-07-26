#include "painting3/PrimitiveDraw.h"
#include "painting3/AABB.h"

#include <rvg.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Shape3Shader.h>
#include <unirender/typedef.h>

namespace pt3
{

void PrimitiveDraw::Init()
{
	rvg_style_init();
}

void PrimitiveDraw::SetColor(uint32_t abgr)
{
	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	auto shader = static_cast<sl::Shape3Shader*>(shader_mgr.GetShader(sl::SHAPE3));
	shader->SetColor(abgr);
}

void PrimitiveDraw::Line(const sm::vec3& p0, const sm::vec3& p1)
{
	SetShader(sl::SHAPE3);
	rvg_line3(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);
}

void PrimitiveDraw::TriLine(const sm::vec3& p0, const sm::vec3& p1, const sm::vec3& p2)
{
	sm::vec3 vertices[] = {p0, p1, p1, p2, p2, p0};
	SetShader(sl::SHAPE3);
	rvg_lines3(&vertices[0].x, 6);
}

void PrimitiveDraw::Rect(const sm::vec3& p0, const sm::vec3& p1)
{
	SetShader(sl::SHAPE3);
	assert(p0.y == p1.y);
	rvg_rect3_on_y(p0.x, p0.z, p1.x, p1.z, p0.y, true);
}

void PrimitiveDraw::Cube(const AABB& aabb)
{
	PrimitiveDraw::Cube(aabb.Cube());
}

void PrimitiveDraw::Cube(const sm::mat4& mat, const AABB& aabb)
{
	PrimitiveDraw::Cube(mat, aabb.Cube());
}

void PrimitiveDraw::Cube(const sm::cube& cube)
{
	Cube(sm::mat4(), cube);
}

void PrimitiveDraw::Cube(const sm::mat4& mat, const sm::cube& cube)
{
	auto& min = cube.min;
	auto& max = cube.max;
	sm::vec3 vertices[] = {
		mat * sm::vec3(min[0], min[1], min[2]),
		mat * sm::vec3(max[0], min[1], min[2]),
		mat * sm::vec3(max[0], max[1], min[2]),
		mat * sm::vec3(min[0], max[1], min[2]),
		mat * sm::vec3(min[0], min[1], max[2]),
		mat * sm::vec3(max[0], min[1], max[2]),
		mat * sm::vec3(max[0], max[1], max[2]),
		mat * sm::vec3(min[0], max[1], max[2])
	};

	// bottom
	Line(vertices[0], vertices[1]);
	Line(vertices[1], vertices[2]);
	Line(vertices[2], vertices[3]);
	Line(vertices[3], vertices[0]);
	// top
	Line(vertices[4], vertices[5]);
	Line(vertices[5], vertices[6]);
	Line(vertices[6], vertices[7]);
	Line(vertices[7], vertices[4]);
	// middle
	Line(vertices[0], vertices[4]);
	Line(vertices[1], vertices[5]);
	Line(vertices[2], vertices[6]);
	Line(vertices[3], vertices[7]);
}

void PrimitiveDraw::Cube(const sm::cube& cube, int texid)
{
	//float vertices[2 * 6 * 3 * 3];
	//int idx_vert = 0;

	//float texcoords[2 * 6 * 3 * 2];
	//int idx_tex = 0;

	//// front
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//// back
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//// left
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//// right
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//// bottom
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = min.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//// top
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 1;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//vertices[idx_vert++] = min.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 0; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = min.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 0;
	//vertices[idx_vert++] = max.x; vertices[idx_vert++] = max.y; vertices[idx_vert++] = max.z;
	//texcoords[idx_tex++] = 1; texcoords[idx_tex++] = 1;

	//enode3d::ShaderMgr* mgr = enode3d::ShaderMgr::Instance();
	//mgr->Sprite();

	//mgr->DrawTri(vertices, texcoords, 2 * 6 * 3, texid);
}

void PrimitiveDraw::Arc(const sm::vec3& center, float radius, const sm::vec3& axis,
	                    const sm::vec3& normal, float start_angle, float end_angle)
{
	static const int NUM = 10;

	std::vector<sm::vec3> vertices;
	vertices.resize(NUM);
	for (int i = 0; i < NUM; ++i)
	{
		float angle = start_angle + (end_angle - start_angle) * (static_cast<float>(i) / (NUM - 1));
		auto mat = sm::mat4::RotatedAxis(normal, angle);
		vertices[i] = center + mat * (axis * radius);
	}

	SetShader(sl::SHAPE3);
	rvg_polyline3(&vertices[0].x, NUM, false);
}

void PrimitiveDraw::Circle(const sm::vec3& center, const sm::vec3& normal, float radius, bool filling)
{
	static const int NUM = 16;

	// rotate normal 90 degree
	sm::vec3 r_vec;
	if (normal.x != 0 || normal.y != 0) {
		r_vec.x = -normal.y;
		r_vec.y = normal.x;
		r_vec.z = 0;
	} else {
		r_vec.x = 1;
		r_vec.y = 0;
		r_vec.z = 0;
	}

	if (!filling)
	{
		std::vector<sm::vec3> vertices;
		vertices.resize(NUM);
		for (int i = 0; i < NUM; ++i) {
			float angle = SM_PI * 2 * (static_cast<float>(i) / (NUM - 1));
			auto mat = sm::mat4::RotatedAxis(normal, angle);
			vertices[i] = center + mat * (r_vec * radius);
		}

		SetShader(sl::SHAPE3);
		rvg_polyline3(&vertices[0].x, vertices.size(), true);
	}
	else
	{
		std::vector<sm::vec3> vertices;
		vertices.resize(NUM * 2 + 2);
		int ptr = 0;
		bool empty = true;
		for (int i = 0; i < NUM; ++i)
		{
			float angle = SM_PI * 2 * (static_cast<float>(i) / (NUM - 1));
			auto mat = sm::mat4::RotatedAxis(normal, angle);
			vertices[ptr++] = center + mat * (r_vec * radius);
			if (empty) {
				vertices[ptr++] = vertices[0];
				empty = false;
			}
			vertices[ptr++] = center;
		}
		vertices[ptr++] = vertices[NUM * 2];

		SetShader(sl::SHAPE3);
		rvg_triangle_strip3(&vertices[0].x, vertices.size());
	}
}

void PrimitiveDraw::Cross(const sm::vec3& center, const sm::vec3& size)
{
	float vertices[18];
	int idx = 0;

	vertices[idx++] = center.x - size.x;
	vertices[idx++] = center.y;
	vertices[idx++] = center.z;
	vertices[idx++] = center.x + size.x;
	vertices[idx++] = center.y;
	vertices[idx++] = center.z;

	vertices[idx++] = center.x;
	vertices[idx++] = center.y - size.y;
	vertices[idx++] = center.z;
	vertices[idx++] = center.x;
	vertices[idx++] = center.y + size.y;
	vertices[idx++] = center.z;

	vertices[idx++] = center.x;
	vertices[idx++] = center.y;
	vertices[idx++] = center.z - size.x;
	vertices[idx++] = center.x;
	vertices[idx++] = center.y;
	vertices[idx++] = center.z + size.z;

	SetShader(sl::SHAPE3);
	rvg_lines3(vertices, 6);
}

void PrimitiveDraw::Grids(const sm::cube& cube, const sm::vec3& size)
{
	std::vector<sm::vec3> lines;
	int cx = size.x ? static_cast<int>(ceil(cube.Width())  / size.x) : 1;
	int cy = size.y ? static_cast<int>(ceil(cube.Height()) / size.y) : 1;
	int cz = size.z ? static_cast<int>(ceil(cube.Depth())  / size.z) : 1;
	lines.reserve(cx * cy * cz * 2);

	for (float z = cube.zmin; z <= cube.zmax; z += size.z) {
		for (float x = cube.xmin; x <= cube.xmax; x += size.x) {
			lines.push_back(sm::vec3(x, cube.ymin, z));
			lines.push_back(sm::vec3(x, cube.ymax, z));
			if (size.x == 0) {
				break;
			}
		}
		for (float y = cube.ymin; y <= cube.ymax; y += size.y) {
			lines.push_back(sm::vec3(cube.xmin, y, z));
			lines.push_back(sm::vec3(cube.xmax, y, z));
			if (size.x == 0) {
				break;
			}
		}
		if (size.x == 0) {
			break;
		}
	}

	SetShader(sl::SHAPE3);
	rvg_lines3(&lines[0].x, lines.size());
}

void PrimitiveDraw::Point(const sm::vec3& p)
{
	SetShader(sl::SHAPE3);
	rvg_point3(p.x, p.y, p.z);
}

void PrimitiveDraw::Points(const std::vector<sm::vec3>& points)
{
	SetShader(sl::SHAPE3);
	for (auto& p : points) {
		rvg_point3(p.x, p.y, p.z);
	}
}

void PrimitiveDraw::Polyline(const std::vector<sm::vec3>& polyline, bool loop)
{
	if (polyline.size() < 2) {
		return;
	}

	SetShader(sl::SHAPE3);
	rvg_polyline3(&polyline[0].x, polyline.size(), loop);
}

void PrimitiveDraw::Polygon(const std::vector<sm::vec3>& polygon)
{
	if (polygon.size() < 2) {
		return;
	}

	SetShader(sl::SHAPE3);
	auto& mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	sl::Shape3Shader* shader = static_cast<sl::Shape3Shader*>(mgr.GetShader(sl::SHAPE3));
	shader->SetType(ur::DRAW_TRIANGLES);
	for (int i = 1, n = polygon.size(); i < n - 1; ++i) {
		shader->Draw(&polygon[0].x, 1);
		shader->Draw(&polygon[i].x, 1);
		shader->Draw(&polygon[i+1].x, 1);
	}
}

void PrimitiveDraw::SetShader(int shader_type)
{
	sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr().SetShader(
		static_cast<sl::ShaderType>(shader_type));
}

}