#pragma once
#include <imgui/imgui.h>
#include <memory>
#include "Scene.h"

void DrawImguiMenus(double aspectRatio, ImGuiIO& io, std::shared_ptr<Scene> scene);
const glm::vec4& GetClearColor();
