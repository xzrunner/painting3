// from "Focus On 3D Terrain Programming"

#pragma once

namespace pt0 { class Camera; }

namespace pt3
{

class ViewFrustum
{
public:
	ViewFrustum();

	void CalculateViewFrustum(const pt0::Camera& cam) const;

	bool VertexFrustumTest(float x, float y, float z,
		bool test_lr = true, bool test_tb = true, bool test_nf = true) const;
	bool CubeFrustumTest(float x, float y, float z, float size) const;
	bool SphereInFrustum(float x, float y, float z, float radius) const;

	const auto& GetViewFrustum() const { return m_view_frustum; }

private:
	mutable float m_view_frustum[6][4];

}; // ViewFrustum

}