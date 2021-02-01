#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Model.h"
#include "Face.h"
#include "NormalModel.h"

class MeshModel : public Model
{
private:
	std::vector<Face<3>> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	NormalModel normalModel;

	virtual void ModelTransformed();

public:
	MeshModel(std::vector<Face<3>> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& modelName = "");
	virtual ~MeshModel();
	const Face<3>& GetFaceInfo(int index);
	const int GetFaceInfoCount();
	const NormalModel& GetNormalModel() const;
	NormalModel& GetNormalModel();
};
