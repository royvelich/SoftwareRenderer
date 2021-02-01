#pragma once

#include <glm/glm.hpp>
#include "Light.h"

class AmbientLight : public Light 
{
public:
	AmbientLight();
	virtual ~AmbientLight();
	const glm::vec3 CalculateIllumination(const glm::vec3& point, const glm::vec3& normal);
};
