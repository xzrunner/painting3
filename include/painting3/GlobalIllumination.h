#pragma once

#include <unirender2/typedef.h>

namespace pt3
{

struct GlobalIllumination
{
    ur2::TexturePtr irradiance_map = nullptr;
    ur2::TexturePtr prefilter_map  = nullptr;
    ur2::TexturePtr brdf_lut       = nullptr;

}; // GlobalIllumination

}