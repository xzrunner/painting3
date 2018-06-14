#include "painting3/EffectsManager.h"

#include <cu/cu_stl.h>
#include <unirender/Blackboard.h>
#include <unirender/Shader.h>
#include <unirender/VertexAttrib.h>

namespace
{

const char* default_vs = R"(

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform vec3 u_diffuse_material;
uniform vec3 u_ambient_material;
uniform vec3 u_specular_material;
uniform float u_shininess;
uniform mat3 u_normal_matrix;
uniform vec3 u_light_position;

varying vec4 v_gouraud_dst;
varying vec2 v_texcoord;

void main()
{
	gl_Position = u_projection * u_modelview * position;

	vec3 eye_normal = u_normal_matrix * normal;
 	vec4 pos4 = u_modelview * position;
 	vec3 pos3 = pos4.xyz / pos4.w;
 	vec3 light_dir = normalize(u_light_position - pos3);
 	float diff = max(0.0, dot(eye_normal, light_dir));

 	vec4 gouraud_col = vec4(diff * u_diffuse_material, 1);
 	gouraud_col.rgb += u_ambient_material;
 	vec3 reflection = normalize(reflect(-light_dir, eye_normal));
 	float spec = max(0.0, dot(eye_normal, reflection));
 	spec = pow(spec, u_shininess);
 	gouraud_col.rgb += spec * u_specular_material;
	v_gouraud_dst = gouraud_col;

	v_texcoord = texcoord;
}

)";

const char* default_fs = R"(

uniform sampler2D u_texture0;

varying vec4 v_gouraud_dst;
varying vec2 v_texcoord;

void main()
{
	vec4 tex_map = texture2D(u_texture0, v_texcoord);
	vec4 tmp = v_gouraud_dst * tex_map;
	gl_FragColor = tmp;
}

)";

const char* no_tex_fs = R"(

uniform sampler2D u_texture0;

varying vec4 v_gouraud_dst;

void main()
{
	gl_FragColor = v_gouraud_dst;
}

)";

const char* skinned_vs = R"(

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec4 blend_indices;
attribute vec4 blend_weights;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform vec3 u_diffuse_material;
uniform vec3 u_ambient_material;
uniform vec3 u_specular_material;
uniform float u_shininess;
uniform mat3 u_normal_matrix;
uniform vec3 u_light_position;

uniform mat4 u_bone_matrix[60];

varying vec4 v_gouraud_dst;
varying vec2 v_texcoord;

int round2int(float f)
{
	return int(floor(f * 255.0 + 0.5));
}

void main()
{
	vec4 obj_pos = u_bone_matrix[round2int(blend_indices.x)] * position * blend_weights.x;
	obj_pos += u_bone_matrix[round2int(blend_indices.y)] * position * blend_weights.y;
	obj_pos += u_bone_matrix[round2int(blend_indices.z)] * position * blend_weights.z;
	obj_pos += u_bone_matrix[round2int(blend_indices.w)] * position * blend_weights.w;

	gl_Position = u_projection * u_modelview * obj_pos;

	vec3 eye_normal = u_normal_matrix * normal;
 	vec4 pos4 = u_modelview * position;
 	vec3 pos3 = pos4.xyz / pos4.w;
 	vec3 light_dir = normalize(u_light_position - pos3);
 	float diff = max(0.0, dot(eye_normal, light_dir));

 	vec4 gouraud_col = vec4(diff * u_diffuse_material, 1);
 	gouraud_col.rgb += u_ambient_material;
 	vec3 reflection = normalize(reflect(-light_dir, eye_normal));
 	float spec = max(0.0, dot(eye_normal, reflection));
 	spec = pow(spec, u_shininess);
 	gouraud_col.rgb += spec * u_specular_material;
	v_gouraud_dst = gouraud_col;

	v_texcoord = texcoord;
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
	default_layout.push_back(ur::VertexAttrib("position", 3, 4));
	default_layout.push_back(ur::VertexAttrib("normal",   3, 4));
	default_layout.push_back(ur::VertexAttrib("texcoord", 2, 4));

	CU_VEC<ur::VertexAttrib> skinned_layout;
	skinned_layout.push_back(ur::VertexAttrib("position",      3, 4));
	skinned_layout.push_back(ur::VertexAttrib("normal",        3, 4));
	skinned_layout.push_back(ur::VertexAttrib("texcoord",      2, 4));
	skinned_layout.push_back(ur::VertexAttrib("blend_indices", 4, 1));
	skinned_layout.push_back(ur::VertexAttrib("blend_weights", 4, 1));

	std::vector<std::string> textures;
	m_effects[EFFECT_DEFAULT] = std::make_unique<ur::Shader>(
		&rc, default_vs, default_fs, textures, default_layout);
	m_effects[EFFECT_DEFAULT_NO_TEX] = std::make_unique<ur::Shader>(
		&rc, default_vs, no_tex_fs, textures, default_layout);
	m_effects[EFFECT_SKINNED] = std::make_unique<ur::Shader>(
		&rc, skinned_vs, default_fs, textures, skinned_layout);
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

void EffectsManager::SetUserEffect(std::unique_ptr<ur::Shader>& user_et)
{
	m_effects[EFFECT_USER] = std::move(user_et);
}

}