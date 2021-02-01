#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MeshModel::MeshModel(std::vector<Face<3>> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& modelName) :
	Model(GeometryType::TRIANGLES, modelName),
	normalModel(faces, vertices, normals, 0.2)
{
	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face<3> currentFace = faces.at(i);
		modelVertices.push_back(vertices.at(currentFace.GetVertexIndex(0) - 1));
		modelVertices.push_back(vertices.at(currentFace.GetVertexIndex(1) - 1));
		modelVertices.push_back(vertices.at(currentFace.GetVertexIndex(2) - 1));
	}

	modelNormals.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face<3> currentFace = faces.at(i);
		modelNormals.push_back(normals.at(currentFace.GetVertexIndex(0) - 1));
		modelNormals.push_back(normals.at(currentFace.GetVertexIndex(1) - 1));
		modelNormals.push_back(normals.at(currentFace.GetVertexIndex(2) - 1));
	}
}

MeshModel::~MeshModel()
{
}

const Face<3>& MeshModel::GetFaceInfo(int index)
{
	return faces[index];
}

const int MeshModel::GetFaceInfoCount()
{
	return faces.size();
}

const NormalModel& MeshModel::GetNormalModel() const
{
	return normalModel;
}

NormalModel& MeshModel::GetNormalModel()
{
	return normalModel;
}

void MeshModel::ModelTransformed()
{
	normalModel.SetWorldTransformation(worldTransform);
}