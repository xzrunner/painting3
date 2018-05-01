#pragma once

namespace pt3
{

inline AABB::AABB()
	: m_position(0, 0, 0)
{
}

inline AABB::AABB(const sm::cube& cube)
	: m_cube(cube)
	, m_position(0, 0, 0)
{ 
	MakeEmpty(); 
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

inline void AABB::Translate(const sm::vec3& offset)
{
	m_position += offset;
}

}