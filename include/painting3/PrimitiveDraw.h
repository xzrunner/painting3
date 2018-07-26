#pragma once

#include <SM_Vector.h>
#include <SM_Matrix.h>
#include <SM_Cube.h>

#include <vector>

namespace pt3
{

class AABB;

class PrimitiveDraw
{
public:
	static void Init();

	static void SetColor(uint32_t abgr);

	static void Line(const sm::vec3& p0, const sm::vec3& p1);

	static void TriLine(const sm::vec3& p0, const sm::vec3& p1, const sm::vec3& p2);

	static void Rect(const sm::vec3& p0, const sm::vec3& p1);

	static void Cube(const AABB& aabb);
	static void Cube(const sm::mat4& mat, const AABB& aabb);
	static void Cube(const sm::cube& cube);
	static void Cube(const sm::mat4& mat, const sm::cube& cube);
	static void Cube(const sm::cube& cube, int texid);

	static void Arc(const sm::vec3& center, float radius, const sm::vec3& axis,
		const sm::vec3& normal, float start_angle, float end_angle);
	static void Circle(const sm::vec3& center, const sm::vec3& normal, float radius,
		bool filling = true);

	static void Cross(const sm::vec3& center, const sm::vec3& size);

	static void Grids(const sm::cube& cube, const sm::vec3& size);

	static void Point(const sm::vec3& point);
	static void Points(const std::vector<sm::vec3>& points);
	static void Polyline(const std::vector<sm::vec3>& polyline, bool loop);
	static void Polygon(const std::vector<sm::vec3>& polygon);

private:
	static void SetShader(int shader_type);

}; // PrimitiveDraw

}