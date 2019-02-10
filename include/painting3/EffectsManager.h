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

	void SetProjMat(model::EffectType effect, const sm::mat4& mat);
    void SetViewMat(model::EffectType effect, const sm::mat4& mat);
    void SetModelMat(model::EffectType effect, const sm::mat4& mat);
	void SetNormalMat(model::EffectType effect, const sm::mat4& model_mat);

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