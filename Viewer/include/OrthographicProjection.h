#pragma once

#include "Projection.h"
#include "Utils.h"
#include <glm/glm.hpp>

class OrthographicProjection : public Projection
{
private:
	float height;

public:
	OrthographicProjection(const float height, const float aspectRatio, const float near, const float far);
	void SetHeight(const float height);
	const float GetHeight();
	glm::mat4x4 GenerateProjectionMatrix();
};