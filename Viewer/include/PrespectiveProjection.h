#pragma once

#include "Projection.h"
#include "Utils.h"
#include <glm/glm.hpp>

class PrespectiveProjection : public Projection
{
private:
	float fovy;

public:
	PrespectiveProjection(const float fovy, const float aspectRatio, const float near, const float far);
	void SetFovy(const float fovy);
	const float GetFovy();
	glm::mat4x4 GenerateProjectionMatrix();
};
