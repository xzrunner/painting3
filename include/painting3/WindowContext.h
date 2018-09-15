#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include <SM_Matrix.h>

#include <boost/signals2.hpp>

namespace pt3
{

class WindowContext
{
public:
	typedef boost::signals2::signal<void(const sm::mat4& mt)> OnView;
	typedef boost::signals2::signal<void(const sm::mat4& mt)> OnProj;

public:
	WindowContext();

	boost::signals2::connection DoOnView(const OnView::slot_type& slot);
	boost::signals2::connection DoOnProj(const OnProj::slot_type& slot);

	void SetView(const sm::mat4& mt);
	void SetProjection(const sm::mat4& mt);
	void SetScreen(int width, int height);

	void UpdateView() const;
	void UpdateProjection() const;
	void UpdateViewport() const;

	void Bind();

	const sm::mat4& GetViewMat() const { return m_mv_mat; }
	const sm::mat4& GetProjMat() const { return m_proj_mat; }

	sm::ivec2 GetScreenSize() { return sm::ivec2(m_screen_width, m_screen_height); }

private:
	OnView m_on_view;
	OnProj m_on_proj;

	sm::mat4 m_mv_mat;
	sm::mat4 m_proj_mat;

	int m_screen_width;
	int m_screen_height;

}; // WindowContext

}

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(pop)
#endif