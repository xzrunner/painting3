#pragma once

#include <cu/cu_macro.h>
#include <SM_Matrix.h>

#include <memory>

namespace model { struct Model; class ModelInstance; class QuakeMapEntity; }
namespace ur { class Texture3D; }

namespace pt3
{

class Material;

class RenderParams
{
public:
	enum DrawType
	{
		DRAW_MESH = 0,
		DRAW_BORDER_MESH,
	};

public:
	sm::mat4 mt;
	sm::mat4 mt_trans;

	DrawType type = DRAW_MESH;

	bool user_effect = false;

}; // RenderParams

class RenderSystem
{
public:
	void DrawMaterial(const Material& material, const RenderParams& params) const;

	static void DrawModel(const model::ModelInstance& model, const RenderParams& params);

	static void DrawTex3D(const ur::Texture3D& t3d, const RenderParams& params);

private:
	void CreateMaterialSphere() const;

	static void DrawMesh(const model::Model& model, const RenderParams& params);

	static void DrawMorphAnim(const model::Model& model, const RenderParams& params);

	static void DrawSkeletalNode(const model::ModelInstance& model, int node_idx, const RenderParams& params);
	//static void DrawSkeletalNodeDebug(const model::ModelInstance& model, int node_idx, const RenderParams& params);

	static void DrawQuakeBSP(const model::Model& model, const RenderParams& params);

	//static void DrawQuakeMapEntity(const model::QuakeMapEntity& entity, const RenderParams& params);

private:
	mutable std::unique_ptr<model::Model> m_mat_sphere = nullptr;

	CU_SINGLETON_DECLARATION(RenderSystem)

}; // RenderSystem

}