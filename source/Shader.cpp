#include "painting3/Shader.h"
#include "painting3/WindowContext.h"

namespace pt3
{

Shader::Shader(ur::RenderContext* rc, const pt0::Shader::Params& params)
	: pt0::Shader(rc, params)
{
}

Shader::~Shader()
{
    ClearNotifies();
}

void Shader::AddNotify(std::shared_ptr<WindowContext>& wc)
{
    if (m_notifies.find(wc) == m_notifies.end()) {
        m_notifies.insert({ wc, {
            wc->DoOnView(boost::bind(&Shader::UpdateViewMat, this, _1)),
            wc->DoOnProj(boost::bind(&Shader::UpdateProjMat, this, _1))
        } });
    }
}

void Shader::RemoveNotify(std::shared_ptr<WindowContext>& wc)
{
    auto itr = m_notifies.find(wc);
    if (itr != m_notifies.end())
    {
        itr->second.view.disconnect();
        itr->second.proj.disconnect();
        m_notifies.erase(wc);
    }
}

void Shader::ClearNotifies()
{
    for (auto& n : m_notifies) {
        n.second.view.disconnect();
        n.second.proj.disconnect();
    }
    m_notifies.clear();
}

void Shader::UpdateViewMat(const sm::mat4& view_mat)
{
	if (view_mat == m_view_mat) {
		return;
	}
	m_view_mat = view_mat;

	Use();

    assert(!m_uniform_names[pt0::U_VIEW_MAT].empty());
	SetMat4(m_uniform_names[pt0::U_VIEW_MAT], view_mat.x);
}

void Shader::UpdateProjMat(const sm::mat4& proj_mat)
{
	if (proj_mat == m_proj_mat) {
		return;
	}
	m_proj_mat = proj_mat;

	Use();

    assert(!m_uniform_names[pt0::U_PROJ_MAT].empty());
	SetMat4(m_uniform_names[pt0::U_PROJ_MAT], proj_mat.x);
}

}