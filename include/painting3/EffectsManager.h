#pragma once

#include <cu/cu_macro.h>
#include <SM_Vector.h>
#include <SM_Matrix.h>
#include <model/EffectType.h>

#include <vector>
#include <memory>

namespace ur { class Shader; }

namespace pt3
{

class EffectsManager
{
public:
	std::shared_ptr<ur::Shader> Use(model::EffectType effect);

	void SetMaterial(model::EffectType effect, const sm::vec3& ambient,
		const sm::vec3& diffuse, const sm::vec3& specular, float shininess);

	void SetLightPosition(model::EffectType effect, const sm::vec3& pos);

	void SetProjMat(model::EffectType effect, const sm::mat4& mat);
	void SetModelViewMat(model::EffectType effect, const sm::mat4& mat);
	void SetNormalMat(model::EffectType effect, const sm::mat4& mat);

	void SetBoneMatrixes(model::EffectType effect, const sm::mat4* mat, int n);

	void SetMorphAnimBlend(float blend);

	void SetUserEffect(std::shared_ptr<ur::Shader>& user_et) {
		m_effects[model::EFFECT_USER] = user_et;
	}

private:
	std::shared_ptr<ur::Shader> m_effects[model::EFFECT_MAX_COUNT];

	CU_SINGLETON_DECLARATION(EffectsManager);

}; // EffectsManager

}