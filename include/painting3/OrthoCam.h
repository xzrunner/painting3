#pragma once

#include "painting3/ICamera.h"

namespace pt3
{

class OrthoCam : public ICamera
{
public:
	enum ViewPlaneType
	{
		VP_ZY = 0,
		VP_XZ,
		VP_XY,
	};

public:
	OrthoCam(ViewPlaneType vp, float scale = 1);

	virtual CamType Type() const { return CAM_ORTHO; }

	virtual void OnSize(float w, float h) override;

	virtual sm::mat4 GetModelViewMat() const override;
	virtual sm::mat4 GetProjectionMat() const override;

	virtual void Reset() override;

	void Translate(const sm::vec2& offset);
	void Scale(float scale, float x, float y, float width, float height);

private:
	// modelview
	float    m_scale;
	sm::vec2 m_position;
	ViewPlaneType m_view_plane;

	// projection
	float m_width, m_height;

}; // OrthoCam

}