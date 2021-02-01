#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "Model.h"
#include "Light.h"
#include "Camera.h"

class Scene {
private:
	std::vector<std::shared_ptr<Model>> models;
	std::vector< std::shared_ptr<Light>> lights;
	std::vector<Camera> cameras;

	int activeCameraIndex;
	int activeModelIndex;

public:
	Scene();

	void AddModel(const std::shared_ptr<Model>& model);
	int GetModelCount() const;
	std::shared_ptr<Model> GetModel(int index) const;

	void AddCamera(const Camera& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	const Camera& GetCamera(int index) const;

	void AddLight(const std::shared_ptr<Light>& light);
	int GetLightCount() const;
	std::shared_ptr<Light> GetLight(int index) const;
	std::vector<std::shared_ptr<Light>> GetActiveLights() const;

	const Camera& GetActiveCamera() const;
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	const std::shared_ptr<Model>& GetActiveModel() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;
};