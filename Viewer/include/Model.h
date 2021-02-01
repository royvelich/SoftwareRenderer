#pragma once

#include "GeometryType.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <string>

class Model
{
protected:
	glm::vec4 color;
	std::vector<glm::vec3> modelVertices;
	std::vector<glm::vec3> modelNormals;
	glm::vec3 position;
	glm::mat4x4 worldTransform;
	GeometryType geometryType;
	std::string modelName;

	Model(GeometryType geometryType, const std::string& modelName = "");
	virtual ~Model();
	virtual void ModelTransformed();

public:
	const glm::mat4x4& GetWorldTransformation() const;
	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	const glm::vec3& GetModelVertex(int index) const;
	const glm::vec3& GetModelNormal(int index) const;
	const int GetModelVertexCount() const;
	const GeometryType GetGeometryType() const;
	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);
	void TranslateCenterToOrigin();
	const std::string& GetModelName();

	void Translate(glm::vec3 translationVector);
	void SelfScale(glm::vec3 scalingVector);
	void SelfScale(double factor);
	void Scale(glm::vec3 scalingVector);
	void Scale(double factor);
	void RotateX(double angle);
	void RotateY(double angle);
	void RotateZ(double angle);
	void ScaleX(double factor);
	void ScaleY(double factor);
	void ScaleZ(double factor);

	void SelfRotateX(double angle);
	void SelfRotateY(double angle);
	void SelfRotateZ(double angle);
	void SelfScaleX(double factor);
	void SelfScaleY(double factor);
	void SelfScaleZ(double factor);
};
