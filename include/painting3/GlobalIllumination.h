#pragma once

#include <memory>

namespace ur { class TextureCube; class Texture; }

namespace pt3
{

struct GlobalIllumination
{
    std::shared_ptr<ur::TextureCube> irradiance_map = nullptr;
    std::shared_ptr<ur::TextureCube> prefilter_map = nullptr;
    std::shared_ptr<ur::Texture>     brdf_lut = nullptr;

}; // GlobalIllumination

}