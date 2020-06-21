#include "painting3/PerspCam.h"
#include "painting3/Blackboard.h"
#include "painting3/WindowContext.h"

#include <sm_const.h>

namespace pt3
{

static const float ZNEAR  = 0.01f;
static const float ZFAR   = 1000;
static const float ASPECT = 45;

PerspCam::PerspCam()
	: m_distance(0)
	, m_znear(ZNEAR)
	, m_zfar(ZFAR)
	, m_aspect(1)
	, m_angle_of_view(ASPECT)
{
}

PerspCam::PerspCam(const sm::vec3& pos, const sm::vec3& target, const sm::vec3& up)
	: m_pos(pos)
	, m_target(target)
	, m_distance(0)
	, m_init_pos(pos)
	, m_init_target(target)
	, m_init_up(up)
	, m_znear(ZNEAR)
	, m_zfar(ZFAR)
	, m_aspect(1)
	, m_angle_of_view(ASPECT)
{
	m_distance = (pos - target).Length();
	CalcUVN(up);

	UpdateViewMat();
}

void PerspCam::OnSize(float w, float h)
{
    pt0::Camera::OnSize(w, h);

	SetAspect(w / h);
}

void PerspCam::Bind() const
{
	UpdateViewMat();
}

sm::mat4 PerspCam::GetViewMat() const
{
	sm::mat4 mat;
	float* m = mat.x;
	m[0] =  m_u.x; m[4] =  m_u.y; m[8] =  m_u.z; m[12] = -m_pos.Dot(m_u);
	m[1] =  m_v.x; m[5] =  m_v.y; m[9] =  m_v.z; m[13] = -m_pos.Dot(m_v);
	m[2] = -m_n.x; m[6] = -m_n.y; m[10]= -m_n.z; m[14] =  m_pos.Dot(m_n);
	m[3] =      0; m[7] =      0; m[11]=      0; m[15] =             1.0;
	return mat;
}

sm::mat4 PerspCam::GetProjectionMat() const
{
	return sm::mat4::Perspective(m_angle_of_view, m_aspect, m_znear, m_zfar);
}

void PerspCam::Reset()
{
	m_pos    = m_init_pos;
	m_target = m_init_target;

	m_distance = (m_target - m_pos).Length();

	CalcUVN(m_init_up);

	UpdateViewMat();
}

void PerspCam::Slide(float du, float dv, float dn)
{
	m_pos.x    = m_pos.x + du * m_u.x + dv * m_v.x + dn * m_n.x;
	m_pos.y    = m_pos.y + du * m_u.y + dv * m_v.y + dn * m_n.y;
	m_pos.z    = m_pos.z + du * m_u.z + dv * m_v.z + dn * m_n.z;
	m_target.x = m_target.x + du * m_u.x + dv * m_v.x + dn * m_n.x;
	m_target.y = m_target.x + du * m_u.y + dv * m_v.y + dn * m_n.y;
	m_target.z = m_target.x + du * m_u.z + dv * m_v.z + dn * m_n.z;

	UpdateViewMat();
}

void PerspCam::Roll(float angle)
{
	float cs = cos(angle * SM_DEG_TO_RAD);
	float sn = sin(angle * SM_DEG_TO_RAD);
	sm::vec3 t(m_u);
	sm::vec3 s(m_v);

	m_u.x = cs * t.x - sn * s.x;
	m_u.y = cs * t.y - sn * s.y;
	m_u.z = cs * t.z - sn * s.z;

	m_v.x = sn * t.x + cs * s.x;
	m_v.y = sn * t.y + cs * s.y;
	m_v.z = sn * t.z + cs * s.z;

	UpdateViewMat();
}

void PerspCam::Yaw(float angle)
{
	float cs = cos(angle * SM_DEG_TO_RAD);
	float sn = sin(angle * SM_DEG_TO_RAD);
	sm::vec3 t(m_n);
	sm::vec3 s(m_u);

	m_n.x = cs * t.x - sn * s.x;
	m_n.y = cs * t.y - sn * s.y;
	m_n.z = cs * t.z - sn * s.z;

	m_u.x = sn * t.x + cs * s.x;
	m_u.y = sn * t.y + cs * s.y;
	m_u.z = sn * t.z + cs * s.z;

	UpdateViewMat();
}

void PerspCam::Pitch(float angle)
{
	float cs = cos(angle * SM_DEG_TO_RAD);
	float sn = sin(angle * SM_DEG_TO_RAD);
	sm::vec3 t(m_v);
	sm::vec3 s(m_n);

	m_v.x = cs * t.x - sn * s.x;
	m_v.y = cs * t.y - sn * s.y;
	m_v.z = cs * t.z - sn * s.z;

	m_n.x = sn * t.x + cs * s.x;
	m_n.y = sn * t.y + cs * s.y;
	m_n.z = sn * t.z + cs * s.z;

	UpdateViewMat();
}

void PerspCam::SetUpDir(const sm::vec3& up)
{
	m_u = m_n.Cross(up).Normalized();
	m_v = m_u.Cross(m_n);

	UpdateViewMat();
}

void PerspCam::Translate(float dx, float dy)
{
	sm::vec3 tx = - m_u * dx,
		     ty = m_v * dy;
	m_pos += tx;
	m_pos += ty;
	m_target += tx;
	m_target += ty;

	UpdateViewMat();
}

void PerspCam::MoveToward(float offset)
{
	m_pos += m_n * offset;
	m_distance = (m_target - m_pos).Length();

	UpdateViewMat();
}

void PerspCam::Move(const sm::vec3& dir, float offset)
{
	m_pos += dir * offset;
	m_distance = (m_target - m_pos).Length();
	m_target = m_pos + m_n * m_distance;

	CalcUVN(m_init_up);
	UpdateViewMat();
}

void PerspCam::AimAtTarget()
{
	m_pos = m_target - m_n * m_distance;

	CalcUVN(m_init_up);
	UpdateViewMat();
}

void PerspCam::SetPosAndAngle(const sm::vec3& pos, const sm::vec3& target, const sm::vec3& up)
{
    m_pos = pos;
    m_target = target;

    m_distance = (m_target - m_pos).Length();

    CalcUVN(up);

    UpdateViewMat();
}

sm::mat4 PerspCam::GetRotateMat() const
{
	sm::mat4 mat;
	float* m = mat.x;
	m[0] = m_u.x; m[4] = m_u.y; m[8] = m_u.z; m[12] = 0;
	m[1] = m_v.x; m[5] = m_v.y; m[9] = m_v.z; m[13] = 0;
	m[2] = m_n.x; m[6] = m_n.y; m[10] = m_n.z; m[14] = 0;
	m[3] = 0;     m[7] = 0;     m[11] = 0;     m[15] = 1.0;
	return mat;
}

void PerspCam::SetAspect(float aspect)
{
    m_aspect = aspect;
    UpdateProjMat();
}

void PerspCam::SetAngleOfView(float aov)
{
    m_angle_of_view = aov;
    UpdateProjMat();
}

void PerspCam::Reset(const sm::vec3& pos, const sm::vec3& target, const sm::vec3& up)
{
	m_init_pos    = pos;
	m_init_target = target;
	m_init_up     = up;

	m_pos    = pos;
	m_target = target;

	m_distance = (m_target - m_pos).Length();

	CalcUVN(up);

	UpdateViewMat();
}

void PerspCam::CalcUVN(const sm::vec3& up)
{
	m_n = (m_target - m_pos).Normalized();
	m_u = m_n.Cross(up).Normalized();
	m_v = m_u.Cross(m_n);
}

void PerspCam::UpdateViewMat() const
{
	//auto& wc = Blackboard::Instance()->GetWindowContext();
	//if (!wc) {
	//	return;
	//}

	//wc->SetView(GetViewMat());

    if (m_wc) {
        m_wc->SetView(GetViewMat());
    }
}

void PerspCam::UpdateProjMat() const
{
    //auto& wc = Blackboard::Instance()->GetWindowContext();
    //if (!wc) {
    //    return;
    //}

    //wc->SetProjection(GetProjectionMat());

    if (m_wc) {
        m_wc->SetProjection(GetProjectionMat());
    }
}

}