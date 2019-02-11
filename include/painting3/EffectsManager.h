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

	void SetUserEffect(std::shared_ptr<ur::Shader>& user_et) {
		m_effects[model::EFFECT_USER] = user_et;
	}

private:
	std::shared_ptr<ur::Shader> m_effects[model::EFFECT_MAX_COUNT];

	CU_SINGLETON_DECLARATION(EffectsManager);

}; // EffectsManager

}