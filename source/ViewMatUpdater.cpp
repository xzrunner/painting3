#include "painting3/ViewMatUpdater.h"
#include "painting3/WindowContext.h"

#include <SM_Matrix.h>
#include <unirender/Uniform.h>
#include <unirender/ShaderProgram.h>

namespace pt3
{

ViewMatUpdater::ViewMatUpdater(const ur::ShaderProgram& shader, const std::string& name)
{
    m_uniform = shader.QueryUniform(name);

    m_mat.Scale(0, 0, 0);
}

void ViewMatUpdater::Update(const ur::Context& ctx, const ur::DrawState& draw,
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