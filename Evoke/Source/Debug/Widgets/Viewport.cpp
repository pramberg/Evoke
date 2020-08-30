#include "PCH.h"
#include "Viewport.h"
#include "Core\Application.h"

#include <imgui.h>

namespace Evoke
{
	void Viewport::Render()
	{
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		auto rt = Application::Instance().RenderTarget();
		ImGui::Image(rt->RawHandle(0), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	void Viewport::PreRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	}

	void Viewport::PostRender()
	{
		ImGui::PopStyleVar();
	}

	void Viewport::Resized()
	{
		auto& app = Application::Instance();
		app.CameraController()->Camera().Resize(mWidth, mHeight);
		app.RenderTarget()->Resize(mWidth, mHeight);
	}
}