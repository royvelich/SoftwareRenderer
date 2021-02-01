#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Utils.h"
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
// open file dialog cross platform https://github.com/mlabbe/nativefiledialog
#include <nfd.h>
#include <random>


bool showDemoWindow = false;
bool showAnotherWindow = false;
bool showFile = false;
glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

const glm::vec4& GetClearColor()
{
	return clearColor;
}

void DrawImguiMenus(double aspectRatio, ImGuiIO& io, std::shared_ptr<Scene> scene)
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		ImGui::Begin("Scene Menu");
		//static float f = 0.0f;
		//static int counter = 0;
		//ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("Clear Color", (float*)&clearColor); // Edit 3 floats representing a color

		//ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our windows open/close state
		//ImGui::Checkbox("Another Window", &showAnotherWindow);

		if (ImGui::CollapsingHeader("Cameras"))
		{
			if (ImGui::Button("Add Camera"))
			{

				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> dist(0, 2 * M_PI);
				std::uniform_real_distribution<double> dist2(2, 10);
				double angle = dist(mt);
				double radius = dist2(mt);

				glm::vec4 eye = glm::vec4(radius * glm::cos(angle), 0, radius * glm::sin(angle), 1);
				glm::vec4 at = glm::vec4(0, 0, 0, 1);
				glm::vec4 up = glm::vec4(0, 1, 0, 1);
				scene->AddCamera(Camera(-scene->GetActiveCamera().GetEye(), at, up, std::make_shared<PrespectiveProjection>(M_PI / 4, aspectRatio, 1, 10)));
			}

			const char** items;
			std::vector<std::string> cameraStrings;
			items = new const char*[scene->GetCameraCount()];
			for (int i = 0; i < scene->GetCameraCount(); i++)
			{
				std::ostringstream s;
				s << "Camera " << i;
				std::string mystring = s.str();
				cameraStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetCameraCount(); i++)
			{
				items[i] = cameraStrings[i].c_str();
			}

			int currentCamera = scene->GetActiveCameraIndex();
			ImGui::Combo("Active Camera", &currentCamera, items, scene->GetCameraCount());

			if (currentCamera != scene->GetActiveCameraIndex())
			{
				scene->SetActiveCameraIndex(currentCamera);
				scene->GetActiveCamera().SetProjectionAspectRatio(aspectRatio);
			}

			delete items;

			std::shared_ptr<Projection> projection = scene->GetActiveCamera().GetProjection();
			ProjectionType projectionType = projection->GetProjectionType();
			int newProjectionType = projectionType == ProjectionType::PRESPECTIVE ? 0 : 1;
			ImGui::RadioButton("Prespective", &newProjectionType, 0);
			ImGui::RadioButton("Orthographic", &newProjectionType, 1);

			if (newProjectionType == 0)
			{
				float fovy;
				float zNear;
				float zFar;
				std::shared_ptr<PrespectiveProjection> prespectiveProjection;

				if (projectionType != ProjectionType::PRESPECTIVE)
				{
					fovy = M_PI / 4;
					zNear = 1;
					zFar = 10;
					prespectiveProjection = std::make_shared<PrespectiveProjection>(fovy, aspectRatio, zNear, zFar);
					scene->GetActiveCamera().SetProjection(prespectiveProjection);
				}
				else
				{
					prespectiveProjection = std::dynamic_pointer_cast<PrespectiveProjection>(projection);
					fovy = prespectiveProjection->GetFovy();
					zNear = prespectiveProjection->GetNear();
					zFar = prespectiveProjection->GetFar();
				}

				if (ImGui::SliderFloat("Fovy", &fovy, 0.0f, M_PI))
				{
					prespectiveProjection->SetFovy(fovy);
					scene->GetActiveCamera().SetProjection(prespectiveProjection);
				}

				if (ImGui::SliderFloat("Near", &zNear, 1.0f, 10.0f))
				{
					prespectiveProjection->SetNear(zNear);
					scene->GetActiveCamera().SetProjection(prespectiveProjection);
				}

				if (ImGui::SliderFloat("Far", &zFar, 1.0f, 10.0f))
				{
					prespectiveProjection->SetFar(zFar);
					scene->GetActiveCamera().SetProjection(prespectiveProjection);
				}
			}
			else if (newProjectionType == 1)
			{
				float height;
				float zNear;
				float zFar;
				std::shared_ptr<OrthographicProjection> orthographicProjection;

				if (projectionType != ProjectionType::ORTHOGRAPHIC)
				{
					height = 3;
					zNear = 1;
					zFar = 10;
					orthographicProjection = std::make_shared<OrthographicProjection>(height, aspectRatio, zNear, zFar);
					scene->GetActiveCamera().SetProjection(orthographicProjection);
				}
				else
				{
					orthographicProjection = std::dynamic_pointer_cast<OrthographicProjection>(projection);
					height = orthographicProjection->GetHeight();
					zNear = orthographicProjection->GetNear();
					zFar = orthographicProjection->GetFar();
				}

				if (ImGui::SliderFloat("Height", &height, 0.0f, M_PI))
				{
					orthographicProjection->SetHeight(height);
					scene->GetActiveCamera().SetProjection(orthographicProjection);
				}

				if (ImGui::SliderFloat("Near", &zNear, 1.0f, 10.0f))
				{
					orthographicProjection->SetNear(zNear);
					scene->GetActiveCamera().SetProjection(orthographicProjection);
				}

				if (ImGui::SliderFloat("Far", &zFar, 1.0f, 10.0f))
				{
					orthographicProjection->SetFar(zFar);
					scene->GetActiveCamera().SetProjection(orthographicProjection);
				}
			}
		}

		if (ImGui::CollapsingHeader("Models"))
		{
			const char** items;
			std::vector<std::string> modelStrings;
			items = new const char*[scene->GetModelCount()];
			for (int i = 0; i < scene->GetModelCount(); i++)
			{
				std::ostringstream s;
				s << scene->GetModel(i)->GetModelName();
				std::string mystring = s.str();
				modelStrings.push_back(mystring);
			}

			for (int i = 0; i < scene->GetModelCount(); i++)
			{
				items[i] = modelStrings[i].c_str();
			}

			int currentModelIndex = scene->GetActiveModelIndex();
			ImGui::Combo("Active Model", &currentModelIndex, items, scene->GetModelCount());

			if (currentModelIndex != scene->GetActiveModelIndex())
			{
				scene->SetActiveModelIndex(currentModelIndex);
			}

			delete items;

			glm::vec4 modelColor = scene->GetActiveModel()->GetColor();
			if (ImGui::ColorEdit3("Model Color", (float*)&modelColor))
			{
				scene->GetActiveModel()->SetColor(modelColor);
			}

			std::shared_ptr<MeshModel> meshModel = std::dynamic_pointer_cast<MeshModel>(scene->GetActiveModel());
			if (meshModel)
			{
				glm::vec4 normalColor = meshModel->GetNormalModel().GetColor();
				if (ImGui::ColorEdit3("Normal Color", (float*)&normalColor))
				{
					meshModel->GetNormalModel().SetColor(normalColor);
				}
			}

			if (ImGui::Button("Snap To Origin"))
			{
				scene->GetActiveModel()->TranslateCenterToOrigin();
			}
		}

		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if (showAnotherWindow)
	{
		int val[2]; val[0] = io.MousePos.x; val[1] = io.MousePos.y;
		ImGui::Begin("Another Window", &showAnotherWindow);
		ImGui::InputInt2("(x,y)", val, 3);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			showAnotherWindow = false;
		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if (showDemoWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene->AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath)));
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Demo Menu")) { showDemoWindow = true; }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}