#include "painting3/EffectsManager.h"
#include "painting3/default.glsl"
#include "painting3/skinned.glsl"
#include "painting3/morph.glsl"
#include "painting3/bsp.glsl"

#include <cu/cu_stl.h>
#include <unirender/Blackboard.h>
#include <unirender/Shader.h>
#include <unirender/VertexAttrib.h>
#include <unirender/typedef.h>
#include <unirender/RenderContext.h>
#include <shaderlab/Blackboard.h>
#include <shaderlab/RenderContext.h>

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
	default_layout.push_back(ur::VertexAttrib("position", 3, 4, 32, 0));
	default_layout.push_back(ur::VertexAttrib("normal",   3, 4, 32, 12));
	default_layout.push_back(ur::VertexAttrib("texcoord", 2, 4, 32, 24));

	CU_VEC<ur::VertexAttrib> skinned_layout;
	skinned_layout.push_back(ur::VertexAttrib("position",      3, 4, 40, 0));
	skinned_layout.push_back(ur::VertexAttrib("normal",        3, 4, 40, 12));
	skinned_layout.push_back(ur::VertexAttrib("texcoord",      2, 4, 40, 24));
	skinned_layout.push_back(ur::VertexAttrib("blend_indices", 4, 1, 40, 32));
	skinned_layout.push_back(ur::VertexAttrib("blend_weights", 4, 1, 40, 36));

	CU_VEC<ur::VertexAttrib> morph_layout;
	morph_layout.push_back(ur::VertexAttrib("pose1_vert",   3, 4, 24, 0));
	morph_layout.push_back(ur::VertexAttrib("pose1_normal", 3, 4, 24, 12));
	morph_layout.push_back(ur::VertexAttrib("pose2_vert",   3, 4, 24, 0));
	morph_layout.push_back(ur::VertexAttrib("pose2_normal", 3, 4, 24, 12));
	morph_layout.push_back(ur::VertexAttrib("texcoord",     2, 4, 0, 0));

	CU_VEC<ur::VertexAttrib> bsp_layout;
	bsp_layout.push_back(ur::VertexAttrib("pos",            3, 4, 28, 0));
	bsp_layout.push_back(ur::VertexAttrib("texcoord",       2, 4, 28, 12));
	bsp_layout.push_back(ur::VertexAttrib("texcoord_light", 2, 4, 28, 20));

	std::vector<std::string> default_textures;

	std::vector<std::string> bsp_textures;
	bsp_textures.push_back("u_base_tex");
	bsp_textures.push_back("u_light_tex");

	m_effects[EFFECT_DEFAULT] = std::make_shared<ur::Shader>(
		&rc, default_vs, default_fs, default_textures, default_layout);
	m_effects[EFFECT_DEFAULT_NO_TEX] = std::make_shared<ur::Shader>(
		&rc, default_vs, no_tex_fs, default_textures, default_layout);
	m_effects[EFFECT_SKINNED] = std::make_shared<ur::Shader>(
		&rc, skinned_vs, default_fs, default_textures, skinned_layout);
	m_effects[EFFECT_MORPH_TARGET] = std::make_shared<ur::Shader>(
		&rc, morph_vs, morph_fs, default_textures, morph_layout);
	m_effects[EFFECT_BSP] = std::make_shared<ur::Shader>(
		&rc, bsp_vs, bsp_fs, bsp_textures, bsp_layout);
}

std::shared_ptr<ur::Shader> EffectsManager::Use(EffectType effect)
{
	if (effect < EFFECT_MAX_COUNT)
	{
		m_effects[effect]->Use();

		auto& shader_mgr = sl::Blackboard::Instance()->GetRenderContext().GetShaderMgr();
		shader_mgr.SetShader(sl::EXTERN_SHADER);

		// flush shader status
		shader_mgr.BindRenderShader(nullptr, sl::EXTERN_SHADER);

		ur::Blackboard::Instance()->GetRenderContext().SetDepthFormat(ur::DEPTH_LESS_EQUAL);

		return m_effects[effect];
	}
	else
	{
		return nullptr;
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

void EffectsManager::SetMorphAnimBlend(float blend)
{
	m_effects[EFFECT_MORPH_TARGET]->SetFloat("u_blend", blend);
}

}