#pragma once

#include <cu/cu_macro.h>
#include <SM_Matrix.h>
#include <painting0/RenderVariant.h>
#include <painting0/ShaderUniforms.h>
#include <painting0/Material.h>
#include <painting0/RenderContext.h>
#include <painting0/UniformNames.h>

#include <memory>
#include <vector>
#include <bitset>

namespace tess { class Painter; }
namespace model { struct Model; class ModelInstance; class QuakeMapEntity; struct MeshGeometry; }
namespace gs { class Shape3D; }
namespace pt0 { class RenderPass; }
namespace ur2 { class Device; class Context; class ShaderProgram; }

namespace pt3
{

class Viewport;

class RenderParams
{
public:
    enum MaskType
    {
        DrawMeshBorder = 0,
        UserEffect,
        DrawCtrlNode,

        NotDrawShape,
        NotDrawModel,
        NotDrawImage,
        NotDrawCloth,
    };

public:
	sm::mat4 model_world;
	sm::mat4 model_local;

    std::bitset<32> mask;

    tess::Painter*  painter  = nullptr;
    const Viewport* viewport = nullptr;
    const sm::mat4* cam_mat  = nullptr;

    // shape
    float    radius = 10.0f;
    uint32_t color  = 0xffffffff;

}; // RenderParams

class RenderSystem
{
public:
    void DrawMaterial(const ur2::Device& dev, ur2::Context& ur_ctx, const pt0::Material& material, const RenderParams& params,
        const pt0::RenderContext& ctx, const std::shared_ptr<ur2::ShaderProgram>& shader = nullptr) const;

    static void DrawShape(const gs::Shape3D& shape, const RenderParams& rp);

    static void DrawMesh(const ur2::Device& dev, ur2::Context& ur_ctx, const model::MeshGeometry& mesh, const pt0::Material& material,
        const pt0::RenderContext& ctx, const std::shared_ptr<ur2::ShaderProgram>& shader = nullptr, bool face = true);

	static void DrawModel(const ur2::Device& dev, ur2::Context& ur_ctx, const model::ModelInstance& model, const std::vector<pt0::Material>& materials,
        const RenderParams& params, const pt0::RenderContext& ctx, const std::shared_ptr<ur2::ShaderProgram>& shader = nullptr);

	static void DrawTex3D(const ur2::Device& dev, ur2::Context& ctx,
        const ur2::Texture& t3d, const RenderParams& params);

    static void DrawLines3D(const ur2::Device& dev, ur2::Context& ctx,
        size_t num, const float* positions, uint32_t color);

    static void DrawSkybox(const ur2::Device& dev, ur2::Context& ctx,
        const ur2::Texture& cube_map);

private:
	void CreateMaterialSphere(const ur2::Device& dev) const;

	static void DrawMesh(const ur2::Device& dev, ur2::Context& ur_ctx, const model::Model& model, const std::vector<pt0::Material>& materials,
        const RenderParams& params, const pt0::RenderContext& ctx, const std::shared_ptr<ur2::ShaderProgram>& shader, bool face);

	static void DrawMorphAnim(const ur2::Device& dev, ur2::Context& ur_ctx, const model::Model& model,
        const std::vector<pt0::Material>& materials, const RenderParams& params, const pt0::RenderContext& ctx);

	static void DrawSkeletalNode(const ur2::Device& dev, ur2::Context& ur_ctx, const model::ModelInstance& model, const std::vector<pt0::Material>& materials,
        int node_idx, const RenderParams& params, const pt0::RenderContext& ctx, const std::shared_ptr<ur2::ShaderProgram>& shader);
	//static void DrawSkeletalNodeDebug(const model::ModelInstance& model, int node_idx, const RenderParams& params);

//	static void DrawQuakeBSP(const model::Model& model, const RenderParams& params);

	//static void DrawQuakeMapEntity(const model::QuakeMapEntity& entity, const RenderParams& params);

private:
	mutable std::unique_ptr<model::Model> m_mat_sphere = nullptr;

	CU_SINGLETON_DECLARATION(RenderSystem)

}; // RenderSystem

}