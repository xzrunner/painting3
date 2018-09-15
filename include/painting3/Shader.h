#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1400 
#pragma warning(push) 
#pragma warning(disable:4996) 
#endif 

#include <SM_Matrix.h>
#include <unirender/Shader.h>

#include <boost/signals2.hpp>

namespace pt3
{

class WindowContext;

class Shader : public ur::Shader
{
public:
	Shader(WindowContext& wc, ur::RenderContext* rc, const char* vs, const char* fs,
		const std::vector<std::string>& textures, const CU_VEC<ur::VertexAttrib>& va_list,
		const std::string& view_name, const std::string& proj_name);
	virtual ~Shader();

private:
	void UpdateViewMat(const sm::mat4& view_mat);
	void UpdateProjMat(const sm::mat4& proj_mat);

private:
	std::string m_view_name;
	std::string m_proj_name;

	sm::mat4 m_view_mat;
	sm::mat4 m_proj_mat;

	boost::signals2::connection m_conn_view;
	boost::signals2::connection m_conn_proj;

}; // Shader

}

#if defined(_MSC_VER) && _MSC_VER >= 1400 
#pragma warning(pop) 
#endif 