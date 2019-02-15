#pragma once

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include <painting0/Shader.h>

#include <SM_Matrix.h>

#include <boost/signals2.hpp>

namespace pt3
{

class WindowContext;

class Shader : public pt0::Shader
{
public:
	Shader(ur::RenderContext* rc, const pt0::Shader::Params& params);
	virtual ~Shader();

    void AddNotify(WindowContext& wc);

private:
	void UpdateViewMat(const sm::mat4& view_mat);
	void UpdateProjMat(const sm::mat4& proj_mat);

private:
	sm::mat4 m_view_mat;
	sm::mat4 m_proj_mat;

    std::vector<std::pair<boost::signals2::connection, boost::signals2::connection>> m_notifies;

    RTTR_ENABLE(pt0::Shader)

}; // Shader

}

#if defined(_MSC_VER) && _MSC_VER >= 1400
#pragma warning(pop)
#endif