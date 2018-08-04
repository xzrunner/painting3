#pragma once

#include <painting0/Camera.h>

namespace pt3
{

	class OrthoCam : public pt0::Camera
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

		virtual pt0::CamTypeID TypeID() const override {
			return pt0::GetCamTypeID<OrthoCam>();
		}

		virtual void OnSize(float w, float h) override;

		virtual void Bind() const override {}

		virtual sm::mat4 GetModelViewMat() const override;
		virtual sm::mat4 GetProjectionMat() const override;

		virtual void Reset() override;

		void Translate(const sm::vec2& offset);
		void Scale(float scale, float x, float y, float width, float height);

		ViewPlaneType GetViewPlaneType() const { return m_view_plane; }

		sm::vec2 TransPosScreenToProject(int x, int y) const;

private:
	// modelview
	float    m_scale;
	sm::vec2 m_position;
	ViewPlaneType m_view_plane;

	// projection
	float m_width, m_height;

}; // OrthoCam

}