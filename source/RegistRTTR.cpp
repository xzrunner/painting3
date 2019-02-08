#include "painting3/PointLight.h"
#include "painting3/DirectionalLight.h"

#include <rttr/registration.h>

RTTR_REGISTRATION
{

rttr::registration::class_<pt3::Light>("pt3_light")
    .property("color", &pt3::Light::GetColor, &pt3::Light::SetColor)
    .property("intensity", &pt3::Light::GetIntensity, &pt3::Light::SetIntensity)
;

rttr::registration::class_<pt3::PointLight>("pt3_point_light")
	.property("position", &pt3::PointLight::GetPosition, &pt3::PointLight::SetPosition)
    .property("radius", &pt3::PointLight::GetRadius, &pt3::PointLight::SetRadius)
;

rttr::registration::class_<pt3::DirectionalLight>("pt3_dir_light")
	.property("direction", &pt3::DirectionalLight::GetDirection, &pt3::DirectionalLight::SetDirection)
;

}

namespace pt3
{

void regist_rttr()
{
}

}
