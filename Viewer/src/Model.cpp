#include "Model.h"
#include "Utils.h"
#include <random>

Model::Model(GeometryType geometryType, const std::string& modelName) :
	geometryType(geometryType),
	position(glm::vec3(0,0,0)),
	modelName(modelName)
{
	worldTransform = glm::mat4x4(1);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	color = glm::vec4(dist(mt), dist(mt), dist(mt), 1);
}

Model::~Model()
{

}

const glm::vec3& Model::GetModelVertex(int index) const
{
	return modelVertices[index];
}

const glm::vec3& Model::GetModelNormal(int index) const
{
	return modelNormals[index];
}

const int Model::GetModelVertexCount() const
{
	return modelVertices.size();
}

const GeometryType Model::GetGeometryType() const
{
	return geometryType;
}

const glm::mat4x4& Model::GetWorldTransformation() const
{
	return worldTransform;
}

void Model::Translate(glm::vec3 translationVector)
{
	position += translationVector;
	worldTransform = Utils::TranslationMatrix(translationVector) * worldTransform;
	ModelTransformed();
}

void Model::Scale(glm::vec3 scalingVector)
{
	worldTransform = Utils::ScalingMatrix(scalingVector) * worldTransform;
	ModelTransformed();
}

void Model::RotateX(double angle)
{
	worldTransform = Utils::XRotationMatrix(angle) * worldTransform;
	ModelTransformed();
}

void Model::RotateY(double angle)
{
	worldTransform = Utils::YRotationMatrix(angle) * worldTransform;
	ModelTransformed();
}

void Model::RotateZ(double angle)
{
	worldTransform = Utils::ZRotationMatrix(angle) * worldTransform;
	ModelTransformed();
}

void Model::ScaleX(double factor)
{
	worldTransform = Utils::XScalingMatrix(factor) * worldTransform;
	ModelTransformed();
}

void Model::ScaleY(double factor)
{
	worldTransform = Utils::YScalingMatrix(factor) * worldTransform;
	ModelTransformed();
}

void Model::ScaleZ(double factor)
{
	worldTransform = Utils::ZScalingMatrix(factor) * worldTransform;
	ModelTransformed();
}

const glm::vec4& Model::GetColor() const
{
	return color;
}

void Model::SetColor(const glm::vec4& color)
{
	this->color = color;
}

void Model::ModelTransformed()
{

}

void Model::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

void Model::SelfRotateX(double angle)
{
	worldTransform = Utils::TranslationMatrix(-position) * worldTransform;
	worldTransform = Utils::XRotationMatrix(angle) * worldTransform;
	worldTransform = Utils::TranslationMatrix(position) * worldTransform;
	ModelTransformed();
}

void Model::SelfRotateY(double angle)
{
	worldTransform = Utils::TranslationMatrix(-position) * worldTransform;
	worldTransform = Utils::YRotationMatrix(angle) * worldTransform;
	worldTransform = Utils::TranslationMatrix(position) * worldTransform;
	ModelTransformed();
}

void Model::SelfRotateZ(double angle)
{
	worldTransform = Utils::TranslationMatrix(-position) * worldTransform;
	worldTransform = Utils::ZRotationMatrix(angle) * worldTransform;
	worldTransform = Utils::TranslationMatrix(position) * worldTransform;
	ModelTransformed();
}

void Model::SelfScaleX(double factor)
{
	worldTransform = Utils::TranslationMatrix(-position) * worldTransform;
	worldTransform = Utils::XScalingMatrix(factor) * worldTransform;
	worldTransform = Utils::TranslationMatrix(position) * worldTransform;
	ModelTransformed();
}

void Model::SelfScaleY(double factor)
{
	worldTransform = Utils::TranslationMatrix(-position) * worldTransform;
	worldTransform = Utils::YScalingMatrix(factor) * worldTransform;
	worldTransform = Utils::TranslationMatrix(position) * worldTransform;
	ModelTransformed();
}

void Model::SelfScaleZ(double factor)
{
	worldTransform = Utils::TranslationMatrix(-position) * worldTransform;
	worldTransform = Utils::ZScalingMatrix(factor) * worldTransform;
	worldTransform = Utils::TranslationMatrix(position) * worldTransform;
	ModelTransformed();
}

void Model::SelfScale(glm::vec3 scalingVector)
{
	worldTransform = Utils::TranslationMatrix(-position) * worldTransform;
	worldTransform = Utils::ScalingMatrix(scalingVector) * worldTransform;
	worldTransform = Utils::TranslationMatrix(position) * worldTransform;
	ModelTransformed();
}

void Model::Scale(double factor)
{
	worldTransform = Utils::ScalingMatrix(glm::vec3(factor, factor, factor)) * worldTransform;
	ModelTransformed();
}

void Model::SelfScale(double factor)
{
	worldTransform = Utils::TranslationMatrix(-position) * worldTransform;
	worldTransform = Utils::ScalingMatrix(glm::vec3(factor, factor, factor)) * worldTransform;
	worldTransform = Utils::TranslationMatrix(position) * worldTransform;
	ModelTransformed();
}

void Model::TranslateCenterToOrigin()
{
	glm::vec3 center = glm::vec3(0,0,0);
	for (int i = 0; i < modelVertices.size(); i++)
	{
		glm::vec4 vertex = glm::vec4(modelVertices[i], 1);
		center += glm::vec3(worldTransform * vertex);
	}

	center /= modelVertices.size();
	Translate(-center);
	position = glm::vec3(0, 0, 0);
}

const std::string& Model::GetModelName()
{
	return modelName;
}