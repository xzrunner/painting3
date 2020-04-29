#pragma once

#include <unirender/typedef.h>

namespace pt3
{

struct GlobalIllumination
{
    ur::TexturePtr irradiance_map = nullptr;
    ur::TexturePtr prefilter_map  = nullptr;
    ur::TexturePtr brdf_lut       = nullptr;

}; // GlobalIllumination

}