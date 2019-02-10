#include "painting3/EffectsManager.h"

#include "painting3/default.glsl"
#include "painting3/color.glsl"
#include "painting3/skinned.glsl"
#include "painting3/morph.glsl"
#include "painting3/bsp.glsl"

#include <cu/cu_stl.h>
#include <unirender/Blackboard.h>
#include <unirender/Shader.h>
#include <unirender/VertexAttrib.h>
#include <unirender/typedef.h>
#include <unirender/RenderContext.h>

namespace
{

const char* no_tex_fs = R"(

uniform sampler2D u_texture0;

varying vec4 v_gouraud_dst;

void main()
{
	gl_FragColor = v_gouraud_dst;
}

)";

}

namespace pt3
{

CU_SINGLETON_DEFINITION(EffectsManager);

EffectsManager::EffectsManager()
{
	auto& rc = ur::Blackboard::Instance()->GetRenderContext();

	CU_VEC<ur::VertexAttrib> default_layout;
	default_layout.emplace_back("position", 3, 4, 32, 0);
	default_layout.emplace_back("normal",   3, 4, 32, 12);
	default_layout.emplace_back("texcoord", 2, 4, 32, 24);

	CU_VEC<ur::VertexAttrib> color_layout;
	color_layout.emplace_back("position", 3, 4, 28, 0);
	color_layout.emplace_back("normal",   3, 4, 28, 12);
	color_layout.emplace_back("color",    4, 1, 28, 24);

	CU_VEC<ur::VertexAttrib> skinned_layout;
	skinned_layout.emplace_back("position",      3, 4, 40, 0);
	skinned_layout.emplace_back("normal",        3, 4, 40, 12);
	skinned_layout.emplace_back("texcoord",      2, 4, 40, 24);
	skinned_layout.emplace_back("blend_indices", 4, 1, 40, 32);
	skinned_layout.emplace_back("blend_weights", 4, 1, 40, 36);

	CU_VEC<ur::VertexAttrib> morph_layout;
	morph_layout.emplace_back("pose1_vert",   3, 4, 24, 0);
	morph_layout.emplace_back("pose1_normal", 3, 4, 24, 12);
	morph_layout.emplace_back("pose2_vert",   3, 4, 24, 0);
	morph_layout.emplace_back("pose2_normal", 3, 4, 24, 12);
	morph_layout.emplace_back("texcoord",     2, 4, 0, 0);

	CU_VEC<ur::VertexAttrib> bsp_layout;
	bsp_layout.emplace_back("pos",            3, 4, 28, 0);
	bsp_layout.emplace_back("texcoord",       2, 4, 28, 12);
	bsp_layout.emplace_back("texcoord_light", 2, 4, 28, 20);

	std::vector<std::string> default_textures;

	std::vector<std::string> bsp_textures;
	bsp_textures.push_back("u_base_tex");
	bsp_textures.push_back("u_light_tex");

	m_effects[model::EFFECT_DEFAULT] = std::make_shared<ur::Shader>(
		&rc, default_vs, default_fs, default_textures, default_layout);
	m_effects[model::EFFECT_DEFAULT_NO_TEX] = std::make_shared<ur::Shader>(
		&rc, default_vs, no_tex_fs, default_textures, default_layout);
	m_effects[model::EFFECT_COLOR] = std::make_shared<ur::Shader>(
		&rc, color_vs, no_tex_fs, default_textures, color_layout);
	m_effects[model::EFFECT_SKINNED] = std::make_shared<ur::Shader>(
		&rc, skinned_vs, default_fs, default_textures, skinned_layout);
	m_effects[model::EFFECT_SKINNED_NO_TEX] = std::make_shared<ur::Shader>(
		&rc, skinned_vs, no_tex_fs, default_textures, skinned_layout);
	m_effects[model::EFFECT_MORPH_TARGET] = std::make_shared<ur::Shader>(
		&rc, morph_vs, morph_fs, default_textures, morph_layout);
	m_effects[model::EFFECT_BSP] = std::make_shared<ur::Shader>(
		&rc, bsp_vs, bsp_fs, bsp_textures, bsp_layout);
}

std::shared_ptr<ur::Shader> EffectsManager::Use(model::EffectType effect)
{
	if (effect < model::EFFECT_MAX_COUNT)
	{
		if (m_effects[effect]) {
			m_effects[effect]->Use();
		}

//		ur::Blackboard::Instance()->GetRenderContext().SetDepthTest(ur::DEPTH_LESS_EQUAL);

		return m_effects[effect];
	}
	else
	{
		return nullptr;
	}
}

void EffectsManager::SetProjMat(model::EffectType effect, const sm::mat4& mat)
{
	if (effect < model::EFFECT_MAX_COUNT) {
		m_effects[effect]->SetMat4("u_projection", mat.x);
	}
}

void EffectsManager::SetViewMat(model::EffectType effect, const sm::mat4& mat)
{
    if (effect < model::EFFECT_MAX_COUNT) {
        m_effects[effect]->SetMat4("u_view", mat.x);
    }
}

void EffectsManager::SetModelMat(model::EffectType effect, const sm::mat4& mat)
{
    if (effect < model::EFFECT_MAX_COUNT) {
        m_effects[effect]->SetMat4("u_model", mat.x);
    }
}

void EffectsManager::SetNormalMat(model::EffectType effect, const sm::mat4& model_mat)
{
	if (effect < model::EFFECT_MAX_COUNT) {
        auto normal_mat = model_mat.Inverted().Transposed();
		m_effects[effect]->SetMat3("u_normal_matrix", sm::mat3(normal_mat).x);
	}
}

void EffectsManager::SetBoneMatrixes(model::EffectType effect, const sm::mat4* mat, int n)
{
	if (effect < model::EFFECT_MAX_COUNT) {
		m_effects[effect]->SetMultiMat4("u_bone_matrix", &mat->x[0], n);
	}
}

void EffectsManager::SetMorphAnimBlend(float blend)
{
	m_effects[model::EFFECT_MORPH_TARGET]->SetFloat("u_blend", blend);
}

}