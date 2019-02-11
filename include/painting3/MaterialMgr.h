#pragma once

#include <painting0/RenderVariant.h>

#include <string>

namespace pt3
{

class MaterialMgr
{
public:
    struct Uniform
    {
        pt0::RenderVarType type;
        std::string        name;
    };

    struct PosTransUniforms
    {
        static Uniform projection;
        static Uniform view;
        static Uniform model;
    };

    struct PhongUniforms
    {
        static Uniform ambient;
        static Uniform diffuse;
        static Uniform specular;
        static Uniform shininess;

        static Uniform diffuse_tex;
    };

    struct PositionUniforms
    {
        static Uniform normal_mat;
        static Uniform light_pos;
    };

    struct AnimUniforms
    {
        static Uniform bone_matrix;
        static Uniform blend;
    };

}; // MaterialMgr

}
