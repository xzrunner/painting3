#include "painting3/ProjectMatUpdater.h"
#include "painting3/WindowContext.h"

#include <SM_Matrix.h>
#include <unirender/Uniform.h>
#include <unirender/ShaderProgram.h>

namespace pt3
{

ProjectMatUpdater::ProjectMatUpdater(const ur::ShaderProgram& shader,
                                     const std::string& name)
{
    m_uniform = shader.QueryUniform(name);

    m_mat.Scale(0, 0, 0);
}

void ProjectMatUpdater::Update(const ur::Context& ctx, const ur::DrawState& draw,
                               const void* scene)
{
    if (!m_uniform || !scene) {
        return;
    }

    auto& wc = *static_cast<const WindowContext*>(scene);
    if (wc.GetProjMat() == m_mat) {
        return;
    }

    m_mat = wc.GetProjMat();
    m_uniform->SetValue(m_mat.x, 4 * 4);
}

}