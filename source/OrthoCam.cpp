#include "painting3/OrthoCam.h"

namespace pt3
{

OrthoCam::OrthoCam(ViewPlaneType vp, float scale)
	: m_view_plane(vp)
	, m_aspect(1)
{
	m_scale.Set(scale, scale);
}

void OrthoCam::OnSize(float w, float h)
{
	m_aspect = w / h;
}

sm::mat4 OrthoCam::GetModelViewMat() const 
{
	// scale
	auto scale_mat = sm::mat4::Scaled(m_scale.x, m_scale.y, m_scale.y);

	// rotate
	sm::mat4 rot_mat;
	switch (m_view_plane)
	{
	case VP_ZY:
		rot_mat = sm::Quaternion::CreateFromAxisAngle(sm::vec3(0, 1, 0), SM_PI * 0.5f);
		break;
	case VP_XZ:
		rot_mat = sm::Quaternion::CreateFromAxisAngle(sm::vec3(1, 0, 0), -SM_PI * 0.5f);
		break;
	case VP_XY:
		break;
	}

	// translate
	auto trans_mat = sm::mat4::Translated(m_pos.x, m_pos.y, 0);

	return scale_mat * rot_mat * trans_mat;
}

sm::mat4 OrthoCam::GetProjectionMat() const 
{
	float hh = 1;
	float hw = hh * m_aspect;
	return sm::mat4::Orthographic(-hw, hw, -hh, hh, 1, -1);
}

}