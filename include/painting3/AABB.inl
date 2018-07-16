#pragma once

namespace pt3
{

inline AABB::AABB()
{
	MakeEmpty();
}

inline AABB::AABB(const sm::cube& cube)
	: m_cube(cube)
	, m_position(0, 0, 0)
{
}

inline void AABB::MakeEmpty()
{
	m_cube.MakeEmpty();
	m_position.Set(0, 0, 0);
}

inline void AABB::Combine(const sm::vec3& pos)
{
	m_cube.Combine(pos);
}

inline void AABB::Combine(const sm::cube& cube, const sm::mat4& mat)
{
	m_cube.Combine(mat * sm::vec3(cube.xmin, cube.ymin, cube.zmin));
	m_cube.Combine(mat * sm::vec3(cube.xmin, cube.ymax, cube.zmin));
	m_cube.Combine(mat * sm::vec3(cube.xmax, cube.ymax, cube.zmin));
	m_cube.Combine(mat * sm::vec3(cube.xmax, cube.ymin, cube.zmin));
	m_cube.Combine(mat * sm::vec3(cube.xmin, cube.ymin, cube.zmax));
	m_cube.Combine(mat * sm::vec3(cube.xmin, cube.ymax, cube.zmax));
	m_cube.Combine(mat * sm::vec3(cube.xmax, cube.ymax, cube.zmax));
	m_cube.Combine(mat * sm::vec3(cube.xmax, cube.ymin, cube.zmax));
}

inline void AABB::Translate(const sm::vec3& offset)
{
	m_position += offset;
}

inline void AABB::Scale(const sm::vec3& scale)
{
	m_cube.Scale(scale);

	m_position.x *= scale.x;
	m_position.y *= scale.y;
	m_position.z *= scale.z;
}

}