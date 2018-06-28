#pragma once

#include <cu/cu_macro.h>
#include <SM_Vector.h>
#include <SM_Matrix.h>

#include <vector>
#include <memory>

namespace ur { class Shader; }

namespace pt3
{

class EffectsManager
{
public:
	enum EffectType
	{
		EFFECT_NULL = 0,

		EFFECT_DEFAULT,
		EFFECT_DEFAULT_NO_TEX,
		EFFECT_SKINNED,
		EFFECT_MORPH_TARGET,

		EFFECT_USER,

		EFFECT_MAX_COUNT,
	};

	void Use(EffectType effect);

	void SetMaterial(EffectType effect, const sm::vec3& ambient,
		const sm::vec3& diffuse, const sm::vec3& specular, float shininess);

	void SetLightPosition(EffectType effect, const sm::vec3& pos);

	void SetProjMat(EffectType effect, const sm::mat4& mat);
	void SetModelViewMat(EffectType effect, const sm::mat4& mat);
	void SetNormalMat(EffectType effect, const sm::mat4& mat);

	void SetBoneMatrixes(EffectType effect, const sm::mat4* mat, int n);

	void SetUserEffect(std::shared_ptr<ur::Shader>& user_et) {
		m_effects[EFFECT_USER] = user_et;
	}

private:
	std::shared_ptr<ur::Shader> m_effects[EFFECT_MAX_COUNT];

	CU_SINGLETON_DECLARATION(EffectsManager);

}; // EffectsManager

}