// from "Focus On 3D Terrain Programming"

#pragma once

namespace pt3
{

class Camera;

class ViewFrustum
{
public:
	ViewFrustum();

	void CalculateViewFrustum(const Camera& cam) const;

	bool VertexFrustumTest(float x, float y, float z,
		bool test_lr = true, bool test_tb = true, bool test_nf = true) const;
	bool CubeFrustumTest(float x, float y, float z, float size) const;
	bool SphereInFrustum(float x, float y, float z, float radius) const;

private:
	mutable float m_view_frustum[6][4];

}; // ViewFrustum

}