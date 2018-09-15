#include "painting3/Shader.h"
#include "painting3/WindowContext.h"

// todo: rm dependence of shaderlab
#include <shaderlab/Blackboard.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/ShaderMgr.h>

namespace pt3
{

Shader::Shader(WindowContext& wc, ur::RenderContext* rc, const char* vs, const char* fs,
	           const std::vector<std::string>& textures, const CU_VEC<ur::VertexAttrib>& va_list,
               const std::string& view_name, const std::string& proj_name)
	: ur::Shader(rc, vs, fs, textures, va_list)
	, m_view_name(view_name)
	, m_proj_name(proj_name)
{
	m_conn_view = wc.DoOnView(boost::bind(&Shader::UpdateViewMat, this, _1));
	m_conn_proj = wc.DoOnProj(boost::bind(&Shader::UpdateProjMat, this, _1));
}

Shader::~Shader()
{
	m_conn_view.disconnect();
	m_conn_proj.disconnect();
}

void Shader::UpdateViewMat(const sm::mat4& view_mat)
{
	if (view_mat == m_view_mat) {
		return;
	}
	m_view_mat = view_mat;

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::EXTERN_SHADER);
	shader_mgr.BindRenderShader(nullptr, sl::EXTERN_SHADER);

	Use();

	SetMat4(m_view_name.c_str(), view_mat.x);
}

void Shader::UpdateProjMat(const sm::mat4& proj_mat)
{
	if (proj_mat == m_proj_mat) {
		return;
	}
	m_proj_mat = proj_mat;

	auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
	shader_mgr.SetShader(sl::EXTERN_SHADER);
	shader_mgr.BindRenderShader(nullptr, sl::EXTERN_SHADER);

	Use();

	SetMat4(m_proj_name.c_str(), proj_mat.x);
}

}