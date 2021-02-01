#pragma once

#include "Utils.h"
#include <glm/glm.hpp>

enum ProjectionType {
	ORTHOGRAPHIC = 0,
	PRESPECTIVE
};

class Projection
{
protected:
	float aspectRatio;
	float zNear;
	float zFar;
	ProjectionType projectionType;

public:
	Projection(const float aspectRatio, const float near, const float far, ProjectionType projectionType);
	virtual glm::mat4x4 GenerateProjectionMatrix() = 0;

	void SetAspectRatio(float aspectRatio);
	void SetNear(const float near);
	void SetFar(const float far);

	float GetAspectRatio();
	float GetNear();
	float GetFar();

	ProjectionType GetProjectionType();
};