#include "painting3/ViewMatUpdater.h"
#include "painting3/WindowContext.h"

#include <SM_Matrix.h>
#include <unirender2/Uniform.h>
#include <unirender2/ShaderProgram.h>

namespace pt3
{

ViewMatUpdater::ViewMatUpdater(const ur2::ShaderProgram& shader, const std::string& name)
{
    m_uniform = shader.QueryUniform(name);

    m_mat.Scale(0, 0, 0);
}

void ViewMatUpdater::Update(const ur2::Context& ctx, const ur2::DrawState& draw,
                            const void* scene)
{
    if (!m_uniform || !scene) {
        return;
    }

    auto& wc = *static_cast<const WindowContext*>(scene);
    if (wc.GetViewMat() == m_mat) {
        return;
    }

    m_mat = wc.GetViewMat();
    m_uniform->SetValue(m_mat.x, 4 * 4);
}

}