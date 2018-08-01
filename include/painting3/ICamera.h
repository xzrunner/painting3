#pragma once

#include "painting3/CamType.h"

#include <SM_Matrix.h>

#include <memory>

namespace pt3
{

class ICamera
{
public:
	virtual ~ICamera() {}

	virtual CamType Type() const = 0;

	virtual void OnSize(float w, float h) = 0;

	virtual sm::mat4 GetModelViewMat() const = 0;
	virtual sm::mat4 GetProjectionMat() const = 0;

	virtual void Reset() = 0;

}; // ICamera

using ICameraPtr = std::shared_ptr<ICamera>;

}