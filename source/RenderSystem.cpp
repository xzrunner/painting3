#include "painting3/RenderSystem.h"
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
#include <renderpipeline/RenderMgr.h>
#include <renderpipeline/VolumeRenderer.h>
#include <renderpipeline/Shape3Renderer.h>
#include <renderpipeline/MeshRenderer.h>
#include <renderpipeline/SkinRenderer.h>
#include <renderpipeline/BSPRenderer.h>
#include <renderpipeline/MorphRenderer.h>
#include <renderpipeline/SkyboxRenderer.h>

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

void RenderSystem::DrawMaterial(const pt0::Material& material,
                                const RenderParams& params,
                                const pt0::RenderContext& ctx,
                                const std::shared_ptr<ur::Shader>& shader) const
{
	if (!m_mat_sphere) {
		CreateMaterialSphere();
	}

    DrawMesh(*m_mat_sphere, { material }, params, ctx, shader);
}

void RenderSystem::DrawMesh(const model::MeshGeometry& mesh, const pt0::Material& material,
                            const pt0::RenderContext& ctx, const std::shared_ptr<ur::Shader>& shader)
{
    auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::MESH);
    std::static_pointer_cast<rp::MeshRenderer>(rd)->Draw(mesh, material, ctx, shader);
}

void RenderSystem::DrawModel(const model::ModelInstance& model_inst, const std::vector<pt0::Material>& materials,
                             const RenderParams& params, const pt0::RenderContext& ctx, const std::shared_ptr<ur::Shader>& shader,
                             const pt0::UniformNames& uniforms)
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
			DrawSkeletalNode(model_inst, materials, 0, params, ctx, shader, uniforms);
			//DrawSkeletalNodeDebug(model, 0, params.mt);
			break;
		case model::EXT_QUAKE_BSP:
			DrawQuakeBSP(*model, params);
			break;
		case model::EXT_QUAKE_MAP:
			DrawMesh(*model, materials, params, ctx, shader);
			//// debug draw, brush's border
			//DrawHalfEdgeMesh(*static_cast<model::QuakeMapEntity*>(model->ext.get()), params);
			break;
		}
	}
	else
	{
		DrawMesh(*model, materials, params, ctx, shader);
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

		auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::TEX3D);
		std::static_pointer_cast<rp::VolumeRenderer>(rd)->
			DrawCube(vertices[0].xyz, texcoords[0].xyz, t3d.TexID(), 0xffffffff);
	}
}

void RenderSystem::DrawLines3D(size_t num, const float* positions, uint32_t color)
{
    auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::SHAPE3D);
    std::static_pointer_cast<rp::Shape3Renderer>(rd)->DrawLines(num, positions, color);
}

void RenderSystem::DrawSkybox(const ur::TextureCube& tcube)
{
    auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::SKYBOX);
    std::static_pointer_cast<rp::SkyboxRenderer>(rd)->Draw(tcube);
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
	m_mat_sphere->meshes.push_back(std::move(mesh));
}

void RenderSystem::DrawMesh(const model::Model& model, const std::vector<pt0::Material>& materials,
                            const RenderParams& params, const pt0::RenderContext& ctx,
                            const std::shared_ptr<ur::Shader>& shader)
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

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

        DrawMesh(mesh->geometry, materials[mesh->material], ctx, shader);
	}
}

void RenderSystem::DrawMorphAnim(const model::Model& model, const std::vector<pt0::Material>& materials,
                                 const RenderParams& params, const pt0::RenderContext& ctx)
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	auto anim = static_cast<model::MorphTargetAnim*>(model.ext.get());
	int frame = anim->GetFrame();
	assert(frame < anim->GetNumFrames());
	int stride = anim->GetNumVertices() * (4 * 3 * 2);

    auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::MORPH);
    auto mrd = std::static_pointer_cast<rp::MorphRenderer>(rd);
    mrd->Draw();

	for (auto& mesh : model.meshes)
	{
		auto& material = model.materials[mesh->material];
		if (material->diffuse_tex != -1) {
			int tex_id = model.textures[material->diffuse_tex].second->TexID();
			rc.BindTexture(tex_id, 0);
		}

        // update anim blend
        const_cast<pt0::RenderContext&>(ctx).AddVar(
            MaterialMgr::AnimUniforms::blend.name,
            pt0::RenderVariant(anim->GetBlend())
        );

        auto& shader = mrd->GetAllShaders()[0];
        materials[mesh->material].Bind(*shader);
        ctx.Bind(*shader);

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
			rc.DrawArrays(mrd->GetAllShaders()[0]->GetDrawMode(), sub.offset, sub.count);
		}
	}
}

void RenderSystem::DrawSkeletalNode(const model::ModelInstance& model_inst, const std::vector<pt0::Material>& materials,
                                    int node_idx, const RenderParams& params, const pt0::RenderContext& ctx,
                                    const std::shared_ptr<ur::Shader>& shader, const pt0::UniformNames& uniforms)
{
    std::string model_mat_name;
    auto name = uniforms.Query(pt0::UniformTypes::ModelMat);
    if (!name.empty()) {
        model_mat_name = name;
    } else {
        model_mat_name = MaterialMgr::PosTransUniforms::model.name;
    }

	auto& model = *model_inst.GetModel();
	auto& g_trans = model_inst.GetGlobalTrans();
	auto& nodes = static_cast<model::SkeletalAnim*>(model.ext.get())->GetNodes();
	auto& node = *nodes[node_idx];
	if (!node.children.empty())
	{
		assert(node.meshes.empty());
		for (auto& child : node.children) {
			DrawSkeletalNode(model_inst, materials, child, params, ctx, shader, uniforms);
		}
	}
	else
	{
		auto child_mat = params.model_world * g_trans[node_idx];
		assert(node.children.empty());
		for (auto& mesh_idx : node.meshes)
		{
            const_cast<pt0::RenderContext&>(ctx).AddVar(
                model_mat_name, pt0::RenderVariant(child_mat)
            );
            auto normal_mat = child_mat.Inverted().Transposed();
            const_cast<pt0::RenderContext&>(ctx).AddVar(
                MaterialMgr::PositionUniforms::normal_mat.name,
                pt0::RenderVariant(sm::mat3(normal_mat))
            );

            auto& mesh = model.meshes[mesh_idx];
            if (mesh->geometry.vertex_type & model::VERTEX_FLAG_SKINNED)
            {
			    auto& bone_trans = model_inst.CalcBoneMatrices(node_idx, mesh_idx);
			    if (!bone_trans.empty()) {
                    const_cast<pt0::RenderContext&>(ctx).AddVar(
                        MaterialMgr::AnimUniforms::bone_matrix.name,
                        pt0::RenderVariant(&bone_trans[0], bone_trans.size())
                    );
			    } else {
				    sm::mat4 mat;
                    const_cast<pt0::RenderContext&>(ctx).AddVar(
                        MaterialMgr::AnimUniforms::bone_matrix.name,
                        pt0::RenderVariant(&mat, 1)
                    );
			    }
                auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::SKIN);
                std::static_pointer_cast<rp::SkinRenderer>(rd)->Draw(model, *mesh, materials[mesh->material], ctx);
            }
            else
            {
                auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::MESH);
                std::static_pointer_cast<rp::MeshRenderer>(rd)->Draw(mesh->geometry, materials[mesh->material], ctx, shader);
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

    auto rd = rp::RenderMgr::Instance()->SetRenderer(rp::RenderType::BSP);
    std::static_pointer_cast<rp::BSPRenderer>(rd)->Draw();

	auto mode = rd->GetAllShaders()[0]->GetDrawMode();

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