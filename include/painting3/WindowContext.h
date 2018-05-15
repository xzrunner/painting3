#pragma once

#include <SM_Matrix.h>

namespace pt3
{

class WindowContext
{
public:
	WindowContext();

	void SetModelView(const sm::mat4& mt);
	void SetProjection(const sm::mat4& mt);
	void SetScreen(int width, int height);

	void UpdateMVP() const;
	void UpdateModelView() const;
	void UpdateProjection() const;
	void UpdateViewport() const;

	void Bind() {
		UpdateMVP();
		UpdateViewport();
	}

	const sm::mat4& GetModelViewMat() const { return m_mv_mat; }
	const sm::mat4& GetProjMat() const { return m_proj_mat; }

private:
	sm::mat4 m_mv_mat;
	sm::mat4 m_proj_mat;

	int m_screen_width;
	int m_screen_height;

}; // WindowContext

}