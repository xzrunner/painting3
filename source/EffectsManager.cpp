#include "painting3/EffectsManager.h"

#include <cu/cu_stl.h>
#include <unirender/Blackboard.h>
#include <unirender/Shader.h>
#include <unirender/VertexAttrib.h>

namespace pt3
{

CU_SINGLETON_DEFINITION(EffectsManager);

EffectsManager::EffectsManager()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	CU_VEC<ur::VertexAttrib> default_layout;
	default_layout.push_back(ur::VertexAttrib("position", 3, 4));
	default_layout.push_back(ur::VertexAttrib("normal",   3, 4));
	default_layout.push_back(ur::VertexAttrib("texcoord", 2, 4));

	CU_VEC<ur::VertexAttrib> skinned_layout;
	skinned_layout.push_back(ur::VertexAttrib("position",      3, 4));
	skinned_layout.push_back(ur::VertexAttrib("normal",        3, 4));
	skinned_layout.push_back(ur::VertexAttrib("texcoord",      2, 4));
	skinned_layout.push_back(ur::VertexAttrib("blend_indices", 4, 1));
	skinned_layout.push_back(ur::VertexAttrib("blend_weights", 4, 1));

	m_effects[EFFECT_DEFAULT] = std::make_unique<ur::Shader>(
		&rc, "shaders/default.vs", "shaders/default.fs", default_layout);
	m_effects[EFFECT_DEFAULT_NO_TEX] = std::make_unique<ur::Shader>(
		&rc, "shaders/default.vs", "shaders/default_no_tex.fs", default_layout);
	m_effects[EFFECT_SKINNED] = std::make_unique<ur::Shader>(
		&rc, "shaders/skinned.vs", "shaders/default.fs", skinned_layout);
}

void EffectsManager::Use(EffectType effect)
{
	if (effect < EFFECT_MAX_COUNT) {
		m_effects[effect]->Use();
	}
}

void EffectsManager::SetMaterial(EffectType effect, const sm::vec3& ambient, const sm::vec3& diffuse, const sm::vec3& specular, float shininess)
{
	if (effect >= EFFECT_MAX_COUNT) {
		return;
	}

	auto& e = m_effects[effect];
	e->SetVec3("u_ambient_material", ambient.xyz);
	e->SetVec3("u_diffuse_material", diffuse.xyz);
	e->SetVec3("u_specular_material", specular.xyz);
	e->SetFloat("u_shininess", shininess);
}

void EffectsManager::SetLightPosition(EffectType effect, const sm::vec3& pos)
{
	if (effect < EFFECT_MAX_COUNT) {
		m_effects[effect]->SetVec3("u_light_position", pos.xyz);
	}
}

void EffectsManager::SetProjMat(EffectType effect, const sm::mat4& mat)
{
	if (effect < EFFECT_MAX_COUNT) {
		m_effects[effect]->SetMat4("u_projection", mat.x);
	}
}

void EffectsManager::SetModelViewMat(EffectType effect, const sm::mat4& mat)
{
	if (effect < EFFECT_MAX_COUNT) {
		m_effects[effect]->SetMat4("u_modelview", mat.x);
	}
}

void EffectsManager::SetNormalMat(EffectType effect, const sm::mat4& mat)
{

	if (effect < EFFECT_MAX_COUNT) {
		m_effects[effect]->SetMat3("u_normal_matrix", sm::mat3(mat).x);
	}
}

void EffectsManager::SetBoneMatrixes(EffectType effect, const sm::mat4* mat, int n)
{
	if (effect < EFFECT_MAX_COUNT) {
		m_effects[effect]->SetMultiMat4("u_bone_matrix", &mat->x[0], n);
	}
}

}