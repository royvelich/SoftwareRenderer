#include "Triangle.h"

Triangle::Triangle(const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3)
{
	vertices.push_back(vertex1);
	vertices.push_back(vertex2);
	vertices.push_back(vertex3);
}

const glm::vec3& Triangle::GetVertex(int index) const
{
	return vertices[index];
}