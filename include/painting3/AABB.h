#pragma once

#include <SM_Cube.h>

namespace pt3
{

class AABB
{
public:
	AABB();
	AABB(const sm::cube& cube);

	void MakeEmpty();

	void Combine(const sm::vec3& pos);

	void Translate(const sm::vec3& offset);

	const float* Max() const { return m_cube.max; }
	const float* Min() const { return m_cube.min; }

	auto& Cube() const { return m_cube; }

private:
	sm::cube m_cube;
	sm::vec3 m_position;

}; // AABB

}

#include "AABB.inl"