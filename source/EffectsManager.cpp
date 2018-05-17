#include "painting3/EffectsManager.h"

#include <unirender/Blackboard.h>
#include <unirender/Shader.h>

namespace pt3
{

CU_SINGLETON_DEFINITION(EffectsManager);

EffectsManager::EffectsManager()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	m_effects[EFFECT_DEFAULT] = std::make_unique<ur::Shader>(
		&rc, "shaders/default.vs", "shaders/default.fs");
	m_effects[EFFECT_DEFAULT_NO_TEX] = std::make_unique<ur::Shader>(
		&rc, "shaders/default.vs", "shaders/default_no_tex.fs");
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

}