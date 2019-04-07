#include "painting3/WindowContext.h"

#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

namespace pt3
{

WindowContext::WindowContext()
	: m_screen_width(0)
	, m_screen_height(0)
{
}

WindowContext::~WindowContext()
{
    m_on_view.disconnect_all_slots();
    m_on_proj.disconnect_all_slots();
}

boost::signals2::connection WindowContext::DoOnView(const OnView::slot_type& slot)
{
	return m_on_view.connect(slot);
}

boost::signals2::connection WindowContext::DoOnProj(const OnProj::slot_type& slot)
{
	return m_on_proj.connect(slot);
}

void WindowContext::SetView(const sm::mat4& mt)
{
	if (m_mv_mat == mt) {
		return;
	}

	m_mv_mat = mt;

	UpdateView();
}

void WindowContext::SetProjection(const sm::mat4& mt)
{
	if (m_proj_mat == mt) {
		return;
	}

	m_proj_mat = mt;

	UpdateProjection();
}

void WindowContext::SetScreen(int width, int height)
{
	m_screen_width = width;
	m_screen_height = height;

	UpdateViewport();
}

void WindowContext::UpdateView() const
{
	m_on_view(m_mv_mat);
}

void WindowContext::UpdateProjection() const
{
	m_on_proj(m_proj_mat);
}

void WindowContext::UpdateViewport() const
{
	if (m_screen_width == 0 && m_screen_height == 0) {
		return;
	}

	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	ur_rc.SetViewport(0, 0, m_screen_width, m_screen_height);
}

void WindowContext::Bind()
{
	UpdateView();
	UpdateProjection();
	UpdateViewport();
}

}