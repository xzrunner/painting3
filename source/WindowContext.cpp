#include "painting3/WindowContext.h"

#include <shaderlab/Blackboard.h>
#include <shaderlab/RenderContext.h>
#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>

namespace pt3
{

WindowContext::WindowContext()
	: m_screen_width(0)
	, m_screen_height(0)
{
}

boost::signals2::connection WindowContext::DoOnView(const OnView::slot_type& slot)
{
	return m_on_view.connect(slot);
}

boost::signals2::connection WindowContext::DoOnProj(const OnProj::slot_type& slot)
{
	return m_on_proj.connect(slot);
}

void WindowContext::SetModelView(const sm::mat4& mt)
{
	if (m_mv_mat == mt) {
		return;
	}

	m_mv_mat = mt;

	UpdateModelView();
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

void WindowContext::UpdateMVP() const
{
	UpdateModelView();
	UpdateProjection();
}

void WindowContext::UpdateModelView() const
{
	sl::Blackboard::Instance()->GetRenderContext().GetSubMVP3().NotifyModelview(m_mv_mat);

	m_on_view(m_mv_mat);
}

void WindowContext::UpdateProjection() const
{
	sl::Blackboard::Instance()->GetRenderContext().GetSubMVP3().NotifyProjection(m_proj_mat);

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

}