#include "painting3/RenderSystem.h"
#include "painting3/EffectsManager.h"
#include "painting3/MaterialMgr.h"

#include <model/typedef.h>
#include <model/BspModel.h>
#include <model/Model.h>
#include <model/ModelInstance.h>
#include <model/ParametricEquations.h>
#include <model/MorphTargetAnim.h>
#include <model/QuakeMapEntity.h>
#include <tessellation/Painter.h>
#include <unirender/Shader.h>
#include <unirender/RenderContext.h>
#include <unirender/Blackboard.h>
#include <unirender/Texture3D.h>
#include <painting0/Shader.h>
#include <painting0/Material.h>
#include <painting0/RenderPass.h>
#include <painting2/RenderSystem.h>
#include <quake/Lightmaps.h>
#include <rendergraph/RenderMgr.h>
#include <rendergraph/VolumeRenderer.h>
#include <rendergraph/MeshRenderer.h>

namespace
{

#define MAX_BATCH_SIZE 4096

static unsigned int vbo_indices[MAX_BATCH_SIZE];
static unsigned int num_vbo_indices;

void FlushBatch(ur::DRAW_MODE mode)
{
	if (num_vbo_indices > 0)
	{
		auto& rc = ur::Blackboard::Instance()->GetRenderContext();
		rc.DrawElements(mode, num_vbo_indices, vbo_indices);

		num_vbo_indices = 0;
	}
}

}

namespace pt3
{

CU_SINGLETON_DEFINITION(RenderSystem);

RenderSystem::RenderSystem()
{
}

void RenderSystem::DrawMaterial(const pt0::Material& material, const RenderParams& params, const RenderContext& ctx) const
{
	if (!m_mat_sphere) {
		CreateMaterialSphere();
	}

	//auto& dst = m_mat_sphere->materials[0];
	//dst->ambient   = material.ambient;
	//dst->diffuse   = material.diffuse;
	//dst->specular  = material.specular;
	//dst->shininess = material.shininess;
	//dst->diffuse_tex = -1;
	//if (material.diffuse_tex)
	//{
	//	dst->diffuse_tex = 0;
	//	m_mat_sphere->textures.clear();
	//	m_mat_sphere->textures.push_back({ "unknown", material.diffuse_tex });
	//	for (auto& mesh : m_mat_sphere->meshes) {
	//		mesh->effect = model::EFFECT_DEFAULT;
	//	}
	//}
	//else
	//{
		for (auto& mesh : m_mat_sphere->meshes) {
			mesh->effect = model::EFFECT_DEFAULT_NO_TEX;
		}
	//}

    DrawMesh(*m_mat_sphere, { material }, params, ctx);
}

void RenderSystem::DrawMesh(const model::MeshGeometry& mesh, const pt0::Material& material,
                            const RenderParams& params, const RenderContext& ctx)
{
    DrawMesh(mesh, material, model::EFFECT_DEFAULT_NO_TEX, nullptr, params, ctx);
}

void RenderSystem::DrawModel(const model::ModelInstance& model_inst, const std::vector<pt0::Material>& materials,
                             const RenderParams& params, const RenderContext& ctx)
{
	auto& model = model_inst.GetModel();
	auto& ext = model->ext;
	if (ext)
	{
		switch (ext->Type())
		{
		case model::EXT_MORPH_TARGET:
			DrawMorphAnim(*model, materials, params, ctx);
			break;
		case model::EXT_SKELETAL:
			DrawSkeletalNode(model_inst, materials, 0, params, ctx);
			//DrawSkeletalNodeDebug(model, 0, params.mt);
			break;
		case model::EXT_QUAKE_BSP:
			DrawQuakeBSP(*model, params);
			break;
		case model::EXT_QUAKE_MAP:
			DrawMesh(*model, materials, params, ctx);
			//// debug draw, brush's border
			//DrawHalfEdgeMesh(*static_cast<model::QuakeMapEntity*>(model->ext.get()), params);
			break;
		}
	}
	else
	{
		DrawMesh(*model, materials, params, ctx);
	}
}

void RenderSystem::DrawTex3D(const ur::Texture3D& t3d, const RenderParams& params)
{
	float hw = static_cast<float>(t3d.Width());
	float hh = static_cast<float>(t3d.Height());
	float hd = static_cast<float>(t3d.Depth());
	hh /= hw * 2;
	hd /= hw * 2;
	hw = 0.5f;

	const int slice_n = 200;
	for (int i = 0; i < slice_n + 1; ++i)
	{
		// calc vertices
		sm::vec3 vertices[4], texcoords[4];

		const float z = -hd + hd * 2 / slice_n * i;
		vertices[0] = sm::vec3(-hw, -hh, z);
		vertices[1] = sm::vec3( hw, -hh, z);
		vertices[2] = sm::vec3( hw,  hh, z);
		vertices[3] = sm::vec3(-hw,  hh, z);
		// trans
		for (auto& v : vertices) {
			v = params.model_local * v;
		}

		const float slice = 1.0f / slice_n * i;
		texcoords[0] = sm::vec3(0, 0, slice);
		texcoords[1] = sm::vec3(1, 0, slice);
		texcoords[2] = sm::vec3(1, 1, slice);
		texcoords[3] = sm::vec3(0, 1, slice);

		auto sr = rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::TEX3D);
		std::static_pointer_cast<rg::VolumeRenderer>(sr)->
			DrawCube(vertices[0].xyz, texcoords[0].xyz, t3d.TexID(), 0xffffffff);
	}
}

void RenderSystem::DrawLines3D(size_t num, const float* positions, uint32_t color)
{
    auto mr = rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::MESH);
    std::static_pointer_cast<rg::MeshRenderer>(mr)->DrawLines(num, positions, color);
}

//void RenderSystem::DrawPasses(const std::vector<pt0::RenderPass>& passes)
//{
//    for (auto& pass : passes)
//    {
//
//    }
//}

void RenderSystem::CreateMaterialSphere() const
{
	m_mat_sphere = std::make_unique<model::Model>();

	auto sphere = std::make_unique<model::Sphere>(1.0f);

	const int vertex_type = model::VERTEX_FLAG_NORMALS | model::VERTEX_FLAG_TEXCOORDS;
	const int stride = 8;

	ur::RenderContext::VertexInfo vi;

	std::vector<float> vertices;
	sphere->GenerateVertices(vertex_type, vertices);
	vi.vn = vertices.size() / stride;
	vi.vertices = &vertices[0];
	vi.stride = stride * sizeof(float);

	std::vector<unsigned short> indices;
	sphere->GenerateTriangleIndices(indices);
	vi.in = indices.size();
	vi.indices = &indices[0];

	vi.va_list.push_back(ur::VertexAttrib("pos",      3, 4, 32, 0));   // pos
	vi.va_list.push_back(ur::VertexAttrib("normal",   3, 4, 32, 12));  // normal
	vi.va_list.push_back(ur::VertexAttrib("texcoord", 2, 4, 32, 24));  // texcoord

	// material
	m_mat_sphere->materials.emplace_back(std::make_unique<model::Model::Material>());

	// mesh
	auto mesh = std::make_unique<model::Model::Mesh>();
	ur::Blackboard::Instance()->GetRenderContext().CreateVAO(
		vi, mesh->geometry.vao, mesh->geometry.vbo, mesh->geometry.ebo);
	mesh->geometry.sub_geometries.push_back(model::SubmeshGeometry(true, vi.in, 0));
	mesh->geometry.sub_geometry_materials.push_back(0);
	mesh->geometry.vertex_type |= model::VERTEX_FLAG_NORMALS;
	mesh->geometry.vertex_type |= model::VERTEX_FLAG_TEXCOORDS;
	mesh->material = 0;
	mesh->effect = model::EFFECT_DEFAULT;
	m_mat_sphere->meshes.push_back(std::move(mesh));
}

void RenderSystem::DrawMesh(const model::MeshGeometry& mesh, const pt0::Material& material,
                            model::EffectType effect_type, const ur::TexturePtr& diffuse_tex,
                            const RenderParams& params, const RenderContext& ctx)
{
    auto& rc = ur::Blackboard::Instance()->GetRenderContext();
    auto mgr = EffectsManager::Instance();

	if (params.user_effect) {
		effect_type = model::EFFECT_USER;
	}
	auto effect = mgr->Use(effect_type);
	if (!effect) {
		return;
	}

    if (ctx.resolution.IsValid()) {
        std::static_pointer_cast<pt0::Shader>(effect)->SetResolution(
            static_cast<float>(ctx.resolution.x), static_cast<float>(ctx.resolution.y)
        );
    }
    if (ctx.cam_pos.IsValid()) {
        std::static_pointer_cast<pt0::Shader>(effect)->SetCamraPos(ctx.cam_pos);
    }

	effect->DrawBefore(diffuse_tex);

    material.Bind(*effect);
    ctx.uniforms.Bind(*effect);

	auto& geo = mesh;
	auto mode = effect->GetDrawMode();
	for (auto& sub : geo.sub_geometries)
	{
		if (geo.vao > 0)
		{
			if (sub.index) {
				ur::Blackboard::Instance()->GetRenderContext().DrawElementsVAO(
					mode, sub.offset, sub.count, geo.vao);
			} else {
				ur::Blackboard::Instance()->GetRenderContext().DrawArraysVAO(
					mode, sub.offset, sub.count, geo.vao);
			}
		}
		else
		{
			auto& sub = geo.sub_geometries[0];
			if (geo.ebo) {
				rc.BindBuffer(ur::INDEXBUFFER, geo.ebo);
				rc.BindBuffer(ur::VERTEXBUFFER, geo.vbo);
				rc.DrawElements(mode, sub.offset, sub.count);
			} else {
				rc.BindBuffer(ur::VERTEXBUFFER, geo.vbo);
				rc.DrawArrays(mode, sub.offset, sub.count);
			}
		}
	}
}

void RenderSystem::DrawMesh(const model::Model& model, const std::vector<pt0::Material>& materials,
                            const RenderParams& params, const RenderContext& ctx)
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	auto mgr = EffectsManager::Instance();
	auto& meshes = params.type == RenderParams::DRAW_MESH ? model.meshes : model.border_meshes;
	for (auto& mesh : meshes)
	{
		ur::TexturePtr diffuse_tex = nullptr;

		auto& material = model.materials[mesh->material];
		if (material->diffuse_tex != -1) {
            diffuse_tex = model.textures[material->diffuse_tex].second;
			if (diffuse_tex) {
				ur::Blackboard::Instance()->GetRenderContext().BindTexture(diffuse_tex->TexID(), 0);
			}
		}

		auto effect_type = model::EffectType(mesh->effect);
        DrawMesh(mesh->geometry, materials[mesh->material], effect_type, diffuse_tex, params, ctx);
	}
}

void RenderSystem::DrawMorphAnim(const model::Model& model, const std::vector<pt0::Material>& materials,
                                 const RenderParams& params, const RenderContext& ctx)
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	auto anim = static_cast<model::MorphTargetAnim*>(model.ext.get());
	int frame = anim->GetFrame();
	assert(frame < anim->GetNumFrames());
	int stride = anim->GetNumVertices() * (4 * 3 * 2);

	auto mgr = EffectsManager::Instance();
	for (auto& mesh : model.meshes)
	{
		auto& material = model.materials[mesh->material];
		if (material->diffuse_tex != -1) {
			int tex_id = model.textures[material->diffuse_tex].second->TexID();
			rc.BindTexture(tex_id, 0);
		}

		auto effect_type = model::EffectType(mesh->effect);
		auto effect = mgr->Use(effect_type);

        // update anim blend
        const_cast<RenderContext&>(ctx).uniforms.AddVar(
            MaterialMgr::AnimUniforms::blend.name,
            pt0::RenderVariant(anim->GetBlend())
        );

        materials[mesh->material].Bind(*effect);
        ctx.uniforms.Bind(*effect);

		auto& geo = mesh->geometry;
//		assert(frame >= 0 && frame < geo.sub_geometries.size());
		//if (geo.vao > 0)
		//{
		//	auto& sub = geo.sub_geometries[frame];
		//	if (sub.index) {
		//		rc.DrawElementsVAO(
		//			mode, sub.offset, sub.count, geo.vao);
		//	} else {
		//		rc.DrawArraysVAO(
		//			mode, sub.offset, sub.count, geo.vao);
		//	}
		//}
		//else
		{
			// pose1_vert, pose1_normal
			int offset = stride * frame;
			geo.vertex_layout[0].offset = offset;
			geo.vertex_layout[1].offset = offset;
			// pose2_vert, pose2_normal
			offset += stride;
			geo.vertex_layout[2].offset = offset;
			geo.vertex_layout[3].offset = offset;
			// update vertex layout
			rc.UpdateVertexLayout(geo.vertex_layout);

			auto& sub = geo.sub_geometries[0];
			rc.BindBuffer(ur::VERTEXBUFFER, geo.vbo);
			rc.DrawArrays(effect->GetDrawMode(), sub.offset, sub.count);
		}
	}
}

void RenderSystem::DrawSkeletalNode(const model::ModelInstance& model_inst, const std::vector<pt0::Material>& materials,
                                    int node_idx, const RenderParams& params, const RenderContext& ctx)
{
	auto& model = *model_inst.GetModel();
	auto& g_trans = model_inst.GetGlobalTrans();
	auto& nodes = static_cast<model::SkeletalAnim*>(model.ext.get())->GetNodes();
	auto& node = *nodes[node_idx];
	if (!node.children.empty())
	{
		assert(node.meshes.empty());
		for (auto& child : node.children) {
			DrawSkeletalNode(model_inst, materials, child, params, ctx);
		}
	}
	else
	{
		auto mgr = EffectsManager::Instance();
		auto child_mat = g_trans[node_idx] * params.model_world;
		assert(node.children.empty());
		for (auto& mesh_idx : node.meshes)
		{
			auto& mesh = model.meshes[mesh_idx];

			auto& material = model.materials[mesh->material];
			if (material->diffuse_tex != -1) {
				int tex_id = model.textures[material->diffuse_tex].second->TexID();
				ur::Blackboard::Instance()->GetRenderContext().BindTexture(tex_id, 0);
			}

			auto effect_type = model::EffectType(mesh->effect);
			auto effect = mgr->Use(effect_type);
			auto mode = effect->GetDrawMode();

			auto& bone_trans = model_inst.CalcBoneMatrices(node_idx, mesh_idx);
			if (!bone_trans.empty()) {
                const_cast<RenderContext&>(ctx).uniforms.AddVar(
                    MaterialMgr::AnimUniforms::bone_matrix.name,
                    pt0::RenderVariant(&bone_trans[0], bone_trans.size())
                );
			} else {
				sm::mat4 mat;
                const_cast<RenderContext&>(ctx).uniforms.AddVar(
                    MaterialMgr::AnimUniforms::bone_matrix.name,
                    pt0::RenderVariant(&mat, 1)
                );
			}

            const_cast<RenderContext&>(ctx).uniforms.AddVar(
                MaterialMgr::PosTransUniforms::model.name,
                pt0::RenderVariant(child_mat)
            );
            auto normal_mat = child_mat.Inverted().Transposed();
            const_cast<RenderContext&>(ctx).uniforms.AddVar(
                MaterialMgr::PositionUniforms::normal_mat.name,
                pt0::RenderVariant(sm::mat3(normal_mat))
            );
            materials[mesh->material].Bind(*effect);
            ctx.uniforms.Bind(*effect);

			auto& geo = mesh->geometry;
			for (auto& sub : geo.sub_geometries)
			{
				if (sub.index) {
					ur::Blackboard::Instance()->GetRenderContext().DrawElementsVAO(
						mode, sub.offset, sub.count, geo.vao);
				} else {
					ur::Blackboard::Instance()->GetRenderContext().DrawArraysVAO(
						mode, sub.offset, sub.count, geo.vao);
				}
			}
		}
	}
}

//void RenderSystem::DrawSkeletalNodeDebug(const model::ModelInstance& model_inst, int node_idx, const RenderParams& params)
//{
//	auto& model = *model_inst.GetModel();
//	auto& g_trans = model_inst.GetGlobalTrans();
//
//	auto& nodes = static_cast<model::SkeletalAnim*>(model.ext.get())->GetNodes();
//	auto& node = *nodes[node_idx];
//	for (auto& child : node.children)
//	{
//		auto& ptrans = g_trans[node_idx];
//		auto& ctrans = g_trans[child];
//		PrimitiveDraw::Line(params.mt * ptrans.GetTranslate(), params.mt * ctrans.GetTranslate());
//
//		assert(node.meshes.empty());
//		for (auto& child : node.children) {
//			DrawSkeletalNodeDebug(model_inst, child, params);
//		}
//	}
//}

void RenderSystem::DrawQuakeBSP(const model::Model& model, const RenderParams& params)
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();
	rc.SetCull(ur::CULL_DISABLE);

	auto mgr = EffectsManager::Instance();
	auto effect_type = model::EFFECT_BSP;
	auto effect = mgr->Use(effect_type);
	auto mode = effect->GetDrawMode();

	num_vbo_indices = 0;

	rc.BindBuffer(ur::VERTEXBUFFER, model.meshes[0]->geometry.vbo);

	int last_lightmap = -1;

	auto bsp = static_cast<model::BspModel*>(model.ext.get());
	for (auto& tex : bsp->textures)
	{
		if (!tex.surfaces_chain || !tex.tex) {
			continue;
		}

		rc.BindTexture(tex.tex->TexID(), 0);

		auto s = tex.surfaces_chain;
		while (s)
		{
			if (s->lightmaptexturenum != last_lightmap)
			{
				FlushBatch(mode);
				last_lightmap = s->lightmaptexturenum;
				int texid = quake::Lightmaps::Instance()->GetTexID(s->lightmaptexturenum);
				rc.BindTexture(texid, 1);
			}

			int num_surf_indices = 3 * (s->numedges - 2);
			if (num_vbo_indices + num_surf_indices > MAX_BATCH_SIZE) {
				FlushBatch(mode);
			}

			for (int i = 2; i < s->numedges; ++i)
			{
				vbo_indices[num_vbo_indices++] = s->vbo_firstvert;
				vbo_indices[num_vbo_indices++] = s->vbo_firstvert + i - 1;
				vbo_indices[num_vbo_indices++] = s->vbo_firstvert + i;
			}

			s = s->next;
		}

		FlushBatch(mode);
	}
}

//void RenderSystem::DrawQuakeMapEntity(const model::QuakeMapEntity& entity,
//	                                  const RenderParams& params)
//{
//	tess::Painter pt;
//	for (auto& brush : entity.GetMapEntity()->brushes) {
//		pt.AddCube(brush.geometry->GetAABB(), [&](const sm::vec3& pos3)->sm::vec2 {
//			return .TransPosProj3ToProj2(params.mt_trans * pos3, cam_mat);
//		}, 0xff0000ff);
//	}
//	pt2::RenderSystem::DrawPainter(pt);
//}

}