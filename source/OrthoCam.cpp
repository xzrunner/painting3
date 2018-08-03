#include "painting3/OrthoCam.h"

namespace pt3
{

OrthoCam::OrthoCam(ViewPlaneType vp, float scale)
	: m_scale(scale)
	, m_view_plane(vp)
{
}

void OrthoCam::OnSize(float w, float h)
{
	m_scale  = 1 / (h * 0.5f);
	m_width  = w;
	m_height = h;
}

sm::mat4 OrthoCam::GetModelViewMat() const
{
	sm::vec2 mv_pos = -m_position;
	float mv_scale = 1 / m_scale;

	// scale
	auto scale_mat = sm::mat4::Scaled(mv_scale, mv_scale, mv_scale);

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
	auto trans_mat = sm::mat4::Translated(mv_pos.x / m_scale, mv_pos.y / m_scale, 0);

	return scale_mat * rot_mat * trans_mat;
}

sm::mat4 OrthoCam::GetProjectionMat() const
{
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	const float max = std::numeric_limits<float>::max();
	return sm::mat4::Orthographic(-hw, hw, -hh, hh, max, -max);
}

void OrthoCam::Reset()
{
	m_position.Set(0, 0);
	m_scale = 1;
}

void OrthoCam::Translate(const sm::vec2& offset)
{
	m_position += offset * m_scale;
}

void OrthoCam::Scale(float scale, float x, float y, float width, float height)
{
	float new_scale = m_scale * scale;
	float hw = width * 0.5f,
		  hh = height * 0.5f;
	m_position.x = (x - hw) * m_scale + m_position.x - (x - hw) * new_scale;
	m_position.y = (y - hh) * m_scale + m_position.y - (y - hh) * new_scale;
	m_scale = new_scale;
}

}