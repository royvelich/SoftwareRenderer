#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Triangle
{
private:
	std::vector<glm::vec3> vertices;

public:
	Triangle(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3);
	const glm::vec3& GetVertex(int index) const;
};