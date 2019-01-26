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
    for (auto& n : m_notifies) {
        n.first.disconnect();
        n.second.disconnect();
    }
}

void Shader::AddNotify(WindowContext& wc)
{
    m_notifies.push_back({
        wc.DoOnView(boost::bind(&Shader::UpdateViewMat, this, _1)),
        wc.DoOnProj(boost::bind(&Shader::UpdateProjMat, this, _1))
    });
}

void Shader::UpdateViewMat(const sm::mat4& view_mat)
{
	if (view_mat == m_view_mat) {
		return;
	}
	m_view_mat = view_mat;

	Use();

	SetMat4(m_uniform_names.view_mat.c_str(), view_mat.x);
}

void Shader::UpdateProjMat(const sm::mat4& proj_mat)
{
	if (proj_mat == m_proj_mat) {
		return;
	}
	m_proj_mat = proj_mat;

	Use();

	SetMat4(m_uniform_names.proj_mat.c_str(), proj_mat.x);
}

}