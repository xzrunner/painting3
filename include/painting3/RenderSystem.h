#pragma once

#include <cu/cu_macro.h>
#include <SM_Matrix.h>
#include <unirender/Texture.h>
#include <painting0/RenderVariant.h>
#include <painting0/ShaderUniforms.h>
#include <painting0/Material.h>
#include <painting0/RenderContext.h>
#include <painting0/UniformNames.h>

#include <memory>
#include <vector>

namespace tess { class Painter; }
namespace model { struct Model; class ModelInstance; class QuakeMapEntity; struct MeshGeometry; }
namespace ur { class Texture3D; class TextureCube; class Shader; }
namespace gs { class Shape3D; }
namespace pt0 { class RenderPass; }

namespace pt3
{

class Viewport;

class RenderParams
{
public:
	enum DrawType
	{
		DRAW_MESH = 0,
		DRAW_BORDER_MESH,
	};

public:
	sm::mat4 model_world;
	sm::mat4 model_local;

	DrawType type = DRAW_MESH;

	bool user_effect = false;

    tess::Painter*  painter  = nullptr;
    const Viewport* viewport = nullptr;
    const sm::mat4* cam_mat  = nullptr;

}; // RenderParams

class RenderSystem
{
public:
    void DrawMaterial(const pt0::Material& material, const RenderParams& params,
        const pt0::RenderContext& ctx, const std::shared_ptr<ur::Shader>& shader = nullptr) const;

    static void DrawShape(const gs::Shape3D& shape, const RenderParams& rp);

    static void DrawMesh(const model::MeshGeometry& mesh, const pt0::Material& material,
        const pt0::RenderContext& ctx, const std::shared_ptr<ur::Shader>& shader = nullptr, bool face = true);

	static void DrawModel(const model::ModelInstance& model, const std::vector<pt0::Material>& materials,
        const RenderParams& params, const pt0::RenderContext& ctx, const std::shared_ptr<ur::Shader>& shader = nullptr,
        const pt0::UniformNames& uniforms = pt0::UniformNames());

	static void DrawTex3D(const ur::Texture3D& t3d, const RenderParams& params);

    static void DrawLines3D(size_t num, const float* positions, uint32_t color);

    static void DrawSkybox(const ur::TextureCube& tcube);

private:
	void CreateMaterialSphere() const;

	static void DrawMesh(const model::Model& model, const std::vector<pt0::Material>& materials,
        const RenderParams& params, const pt0::RenderContext& ctx, const std::shared_ptr<ur::Shader>& shader, bool face);

	static void DrawMorphAnim(const model::Model& model, const std::vector<pt0::Material>& materials,
        const RenderParams& params, const pt0::RenderContext& ctx);

	static void DrawSkeletalNode(const model::ModelInstance& model, const std::vector<pt0::Material>& materials,
        int node_idx, const RenderParams& params, const pt0::RenderContext& ctx, const std::shared_ptr<ur::Shader>& shader,
        const pt0::UniformNames& uniforms);
	//static void DrawSkeletalNodeDebug(const model::ModelInstance& model, int node_idx, const RenderParams& params);

	static void DrawQuakeBSP(const model::Model& model, const RenderParams& params);

	//static void DrawQuakeMapEntity(const model::QuakeMapEntity& entity, const RenderParams& params);

private:
	mutable std::unique_ptr<model::Model> m_mat_sphere = nullptr;

	CU_SINGLETON_DECLARATION(RenderSystem)

}; // RenderSystem

}