#include "NormalModel.h"

NormalModel::NormalModel(std::vector<Face<3>> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, float normalLength) :
	Model(GeometryType::LINES),
	normalLength(normalLength)
{
	modelVertices.reserve(2 * vertices.size());
	for (int i = 0; i < normals.size(); i++)
	{
		glm::vec3 scaled_normal = normals[i] * normalLength;
		modelVertices.push_back(vertices[i]);
		modelVertices.push_back(vertices[i] + scaled_normal);
	}

	for (int i = 0; i < modelVertices.size(); i++)
	{
		modelNormals.push_back(glm::vec3(0, 0, 0));
	}
}

NormalModel::~NormalModel()
{

}