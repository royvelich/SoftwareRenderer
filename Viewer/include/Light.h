#pragma once

#include <glm/glm.hpp>

/*
 * Light class. Holds light source information and data.
 */
class Light
{
public:
	Light();
	Light(const glm::vec3& color);
	virtual ~Light();

	const glm::vec3 GetColor();
	void SetColor(const glm::vec3& color);

	virtual const glm::vec3 CalculateIllumination(const glm::vec3& point, const glm::vec3& normal) = 0;

protected:
	glm::vec3 color;
};
