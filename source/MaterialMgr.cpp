#include "painting3/MaterialMgr.h"

namespace pt3
{

// CoordsUniforms
MaterialMgr::Uniform MaterialMgr::CoordsUniforms::projection
    = { pt0::RenderVarType::MAT4, "u_projection" };
MaterialMgr::Uniform MaterialMgr::CoordsUniforms::modelview
    = { pt0::RenderVarType::MAT4, "u_modelview" };

// PhongUniforms
MaterialMgr::Uniform MaterialMgr::PhongUniforms::ambient
    = { pt0::RenderVarType::VEC3, "u_ambient_material" };
MaterialMgr::Uniform MaterialMgr::PhongUniforms::diffuse
    = { pt0::RenderVarType::VEC3, "u_diffuse_material" };
MaterialMgr::Uniform MaterialMgr::PhongUniforms::specular
    = { pt0::RenderVarType::VEC3, "u_specular_material" };
MaterialMgr::Uniform MaterialMgr::PhongUniforms::shininess
    = { pt0::RenderVarType::FLOAT, "u_shininess" };
MaterialMgr::Uniform MaterialMgr::PhongUniforms::diffuse_tex
    = { pt0::RenderVarType::SAMPLER2D, "u_texture0" };

// PositionUniforms
MaterialMgr::Uniform MaterialMgr::PositionUniforms::normal_mat
    = { pt0::RenderVarType::MAT3, "u_normal_matrix" };
MaterialMgr::Uniform MaterialMgr::PositionUniforms::light_pos
    = { pt0::RenderVarType::VEC3, "u_light_position" };

}