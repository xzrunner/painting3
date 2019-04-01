#pragma once

#include <memory>

namespace ur { class TextureCube; }

namespace pt3
{

struct GlobalIllumination
{
    std::shared_ptr<ur::TextureCube> irradiance_map = nullptr;

}; // GlobalIllumination

}