#pragma once

#include <SM_Matrix.h>
#include <unirender/UniformUpdater.h>

#include <string>

namespace ur { class ShaderProgram; class Uniform; }

namespace pt3
{

class ProjectMatUpdater : public ur::UniformUpdater
{
public:
    ProjectMatUpdater(const ur::ShaderProgram& shader,
        const std::string& name);

    virtual ur::UpdaterID UpdaterTypeID() const override {
        return ur::GetUpdaterTypeID<ProjectMatUpdater>();
    }

    virtual void Update(const ur::Context& ctx,
        const ur::DrawState& draw, const void* scene = nullptr) override;

private:
    std::shared_ptr<ur::Uniform> m_uniform = nullptr;

    sm::mat4 m_mat;

}; // ProjectMatUpdater

}