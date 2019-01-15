#pragma once

#include <painting0/Camera.h>

#include <SM_Vector.h>
#include <SM_Matrix.h>

namespace pt3
{

class PerspCam : public pt0::Camera
{
public:
	PerspCam();
	PerspCam(const sm::vec3& pos, const sm::vec3& target, const sm::vec3& up);

	virtual pt0::CamTypeID TypeID() const override {
		return pt0::GetCamTypeID<PerspCam>();
	}

	virtual void OnSize(float w, float h) override;

	virtual void Bind() const override;

	virtual sm::mat4 GetViewMat() const override;
	virtual sm::mat4 GetProjectionMat() const override;

	virtual void Reset() override;

	void Slide(float du, float dv, float dn);

	// rotate
	void Roll(float angle);
	void Yaw(float angle);
	void Pitch(float angle);
	void SetUpDir(const sm::vec3& up);

	// translate
	void Translate(float dx, float dy);
	void MoveToward(float offset);
	void Move(const sm::vec3& dir, float offset);
	void AimAtTarget();

	const sm::vec3& GetPos() const { return m_pos; }
	const sm::vec3& GetTarget() const { return m_target; }
	float GetDistance() const { return m_distance; }
	const sm::vec3& GetUpDir() const { return m_v; }

    void SetPosAndAngle(const sm::vec3& pos, const sm::vec3& target, const sm::vec3& up);

	sm::mat4 GetRotateMat() const;

	float GetNear() const { return m_znear; }
	float GetFar() const { return m_zfar; }
	float GetAspect() const { return m_aspect; }
	float GetAngleOfView() const { return m_angle_of_view; }

	const sm::vec3 GetToward() const { return m_n; }

	void SetAspect(float aspect) { m_aspect = aspect; }

	void Reset(const sm::vec3& pos, const sm::vec3& target, const sm::vec3& up);

private:
	void CalcUVN(const sm::vec3& up);

	void UpdateRender() const;

private:
	// pos
	sm::vec3 m_pos, m_target;
	float m_distance;

	// angle
	sm::vec3 m_u, m_v, m_n;

	// store for reset
	sm::vec3 m_init_pos;
	sm::vec3 m_init_target;
	sm::vec3 m_init_up;

	// projection
	float m_znear, m_zfar;
	float m_aspect;
	float m_angle_of_view;

}; // PerspCam

}