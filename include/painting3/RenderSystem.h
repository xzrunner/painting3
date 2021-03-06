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
namespace ur { class Device; class Context; class ShaderProgram; struct DrawState; }

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
    void DrawMaterial(const ur::Device& dev, ur::Context& ur_ctx, const ur::DrawState& ds,
        const pt0::Material& material, const RenderParams& params, const pt0::RenderContext& ctx) const;

    static void DrawShape(const gs::Shape3D& shape, const RenderParams& rp);

    static void DrawMesh(const ur::Device& dev, ur::Context& ur_ctx, const ur::DrawState& ds,
        const model::MeshGeometry& mesh, const pt0::Material& material, const pt0::RenderContext& ctx, bool face = true);

	static void DrawModel(const ur::Device& dev, ur::Context& ur_ctx, const ur::DrawState& ds,
        const model::ModelInstance& model, const std::vector<pt0::Material>& materials,
        const RenderParams& params, const pt0::RenderContext& ctx);

	static void DrawTex3D(const ur::Device& dev, ur::Context& ctx,
        const ur::Texture& t3d, const RenderParams& params);

    static void DrawLines3D(const ur::Device& dev, ur::Context& ctx,
        size_t num, const float* positions, uint32_t color);

    static void DrawSkybox(const ur::Device& dev, ur::Context& ctx,
        const ur::Texture& cube_map);

private:
	void CreateMaterialSphere(const ur::Device& dev) const;

	static void DrawMesh(const ur::Device& dev, ur::Context& ur_ctx, const ur::DrawState& ds,
        const model::Model& model, const std::vector<pt0::Material>& materials, const RenderParams& params,
        const pt0::RenderContext& ctx, bool face);

	static void DrawMorphAnim(const ur::Device& dev, ur::Context& ur_ctx, const model::Model& model,
        const std::vector<pt0::Material>& materials, const RenderParams& params, const pt0::RenderContext& ctx);

	static void DrawSkeletalNode(const ur::Device& dev, ur::Context& ur_ctx, const ur::DrawState& ds,
        const model::ModelInstance& model, const std::vector<pt0::Material>& materials,
        int node_idx, const RenderParams& params, const pt0::RenderContext& ctx);
	//static void DrawSkeletalNodeDebug(const model::ModelInstance& model, int node_idx, const RenderParams& params);

//	static void DrawQuakeBSP(const model::Model& model, const RenderParams& params);

	//static void DrawQuakeMapEntity(const model::QuakeMapEntity& entity, const RenderParams& params);

private:
	mutable std::unique_ptr<model::Model> m_mat_sphere = nullptr;

	CU_SINGLETON_DECLARATION(RenderSystem)

}; // RenderSystem

}