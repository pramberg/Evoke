#include "PCH.h"
#include "ImGuiLayer.h"
#include "Widgets/Viewport.h"
#include "Core/Application.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Widgets/Outliner.h"
#include "Widgets/DetailsWidget.h"

#include "ECS/Components.h"
#include "Assets/AssetRegistry.h"
#include "ImGuiStyles.h"
#include "imgui_internal.h"
#include "Widgets/ConsoleWidget.h"

namespace Evoke
{
	ImGuiLayer::ImGuiLayer(b8 inDockable) : Layer("ImGuiLayer"), mDockable(inDockable)
	{
		REGISTER_WIDGET(Viewport);
		REGISTER_WIDGET(Outliner);
		REGISTER_WIDGET_NAME(DetailsWidget, "Details");
		REGISTER_WIDGET_NAME(ConsoleWidget, "Console");

		REGISTER_DETAIL_PANEL_RENDERER_FN(TransformComponent, 126, [](TransformComponent& inComponent, entt::registry& inRegistry, entt::entity inEntity)
		{
			glm::vec3 rotation = glm::degrees(inComponent.RotationE);
			ImGui::DragFloat3("Translation", &inComponent.Location.x, 0.05f);
			ImGui::DragFloat3("Rotation", &rotation.x, 1.0f, 0.0f, 0.0f, (c8*)u8"%.3f\u00B0");
			ImGui::DragFloat3("Scale", &inComponent.Scale.x, 0.05f);
			inComponent.RotationE = glm::radians(rotation);
		});

		REGISTER_DETAIL_PANEL_RENDERER_FN(StaticMeshComponent, 0, [](StaticMeshComponent& inComponent, entt::registry& inRegistry, entt::entity inEntity)
		{
			AssetRegistry* ar = Application::Get().GetAssetRegistry();
			auto meshIdentifiers = ar->GetLoadedMeshIdentifiers();
			ImGui::Combo("Mesh", &inComponent.CurrentMeshSelection, meshIdentifiers);

			if (inComponent.CurrentMeshSelection < 0)
				return;

			inComponent.MeshId = entt::hashed_string::value(meshIdentifiers[inComponent.CurrentMeshSelection].c_str());
		});

		//REGISTER_DETAIL_PANEL_RENDERER(SelectionComponent, -127);
		//REGISTER_DETAIL_PANEL_RENDERER(NameComponent, 127);
		//REGISTER_DETAIL_PANEL_RENDERER(EditorCameraComponent, 127);
	}

	void ImGuiLayer::Attached()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		/*io.ConfigViewportsNoDecoration = false;
		io.ConfigViewportsNoDefaultParent = true;*/

		// Setup Dear ImGui style
		ImGui::Styles::DarkTheme_01_Custom();

		constexpr ImWchar ranges[]
		{
			0x00B0 // Degree sign
		};

		ImFontConfig cfg{};
		cfg.SizePixels = 14;

		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf", 14, &cfg);
		cfg.MergeMode = true;
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf", 14, &cfg, ranges);
		cfg.MergeMode = false;
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Bold.ttf", 14, &cfg);
		cfg.MergeMode = true;
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Bold.ttf", 14, &cfg, ranges);
		cfg.MergeMode = false;
		cfg.SizePixels = 15;
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/SourceCodePro-Regular.ttf", 15, &cfg);
		
		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.MainWindow().NativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		mWidgets.push_back(MakeUnique<Viewport>("Viewport"));
		mWidgets.push_back(MakeUnique<Outliner>());
		mWidgets.push_back(MakeUnique<DetailsWidget>());
		mWidgets.push_back(MakeUnique<ConsoleWidget>());

		names.reserve(9);
		names.emplace_back("Test1");
		names.emplace_back("Test2");
		names.emplace_back("Test3");
		names.emplace_back("Test4");
		names.emplace_back("Test5");
		names.emplace_back("Test6");
		names.emplace_back("Test7");
		names.emplace_back("Test8");
		names.emplace_back("Test9");

		EV_LOG(LogTemp, ELogLevel::Trace, "Trace");
		EV_LOG(LogTemp, ELogLevel::Info, "Info");
		EV_LOG(LogTemp, ELogLevel::Warning, "Warning");
		EV_LOG(LogTemp, ELogLevel::Error, "Error");
		EV_LOG(LogTemp, ELogLevel::Critical, "Critical");
		EV_LOG(LogTemp, ELogLevel::Critical, "Critical");
	}

	void ImGuiLayer::Detached()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Update(f32 inDeltaTime, GraphicsContext* inOutGraphicsContext)
	{
		inOutGraphicsContext->BeginEvent("ImGui");
		Begin();

		if (mShowDemoWindow)
			ImGui::ShowDemoWindow(&mShowDemoWindow);

		OnRender.Broadcast();

		ImGui::Begin("Reorderable Selectable");
		
		for (i32 i = 0; i < names.size(); i++)
		{
			ImGui::PushID(i);

			ImGui::Selectable(names[i].Name.c_str(), &names[i].Selected, ImGuiSelectableFlags_AllowDoubleClick);
			
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				// Set payload to carry the index of our item (could be anything)
				ImGui::SetDragDropPayload("SomeTestXD", &i, sizeof(i32));

				// Display preview (could be anything, e.g. when dragging an image we could decide to display
				// the filename and a small preview of the image, etc.)
				ImGui::Text("Moving %s", names[i].Name.c_str());
				ImGui::EndDragDropSource();
			}

			/*if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SomeTestXD"))
				{
					EV_ASSERT(payload->DataSize == sizeof(i32));
					i32 payload_i = *(const i32*)payload->Data;
					std::swap(names[i], names[payload_i]);
				}
				ImGui::EndDragDropTarget();
			}*/

			ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			ImVec2 p0 = cursorPos;
			p0.y -= 4;
			ImVec2 p1 = ImVec2(p0.x + ImGui::GetWindowWidth(), p0.y + 8);
			if (ImGui::BeginDragDropTargetCustom(ImRect{ p0, p1 }, ImGui::GetID("CustomRect")))
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SomeTestXD", ImGuiDragDropFlags_AcceptPeekOnly))
				{
					auto* drawList = ImGui::GetWindowDrawList();
					ImVec2 p25141 = ImVec2(p0.x + ImGui::GetWindowWidth(), p0.y);

					const ImU32 color = ImGui::GetColorU32(ImGuiCol_Text);
					drawList->AddLine(p0, p25141, color, 1.0f);
					if (payload && payload->Delivery)
					{
						EV_ASSERT(payload->DataSize == sizeof(i32));
						i32 source_i = *(const i32*)payload->Data;
						//std::swap(names[i], names[source_i]);
						EV_LOG(LogTemp, ELogLevel::Info, "Source: {}", source_i);
						EV_LOG(LogTemp, ELogLevel::Info, "Target: {}", i);
						auto tmp = names[source_i];
						names.erase(names.begin() + source_i);
						names.insert(names.begin() + i + 1 * (source_i > i), tmp);
						
					}
				}
				ImGui::EndDragDropTarget();
			}
			

			
			/*if (ImGui::BeginDragDropTargetCustom(ImRect{ p0, p1 }, ImGui::GetID("CustomRect")))
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SomeTestXD"))
				{
					EV_ASSERT(payload->DataSize == sizeof(i32));
					i32 payload_i = *(const i32*)payload->Data;
					std::swap(names[i], names[payload_i]);
				}
				ImGui::EndDragDropTarget();
			}*/

			/*ImGui::Button("###2", { ImGui::GetWindowWidth(), 2 });
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SomeTestXD"))
				{
					EV_ASSERT(payload->DataSize == sizeof(i32));
					i32 payload_i = *(const i32*)payload->Data;
					std::swap(names[i], names[payload_i]);
				}
				ImGui::EndDragDropTarget();
			}*/
			ImGui::PopID();
		}

		ImGui::End();

		End();
		inOutGraphicsContext->EndEvent();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (!mDockable)
			return;

		static b8 dockspaceOpen = true;

		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		{
			windowFlags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Evoke Editor", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar(3);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspaceId = ImGui::GetID("Evoke Editor Dockspace");
			ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Windows"))
			{
				WidgetManager::Get().Each([&](StringView inName, const WidgetManager::WidgetFactoryFn& inFactory, i32 inIndex)
				{
					ImGui::PushID(inIndex);
					if (ImGui::MenuItem(inName.data()))
					{
						mWidgets.push_back(inFactory());
					}
					ImGui::PopID();
				});

				if (ImGui::MenuItem("Show Demo Window")) { mShowDemoWindow = true; }
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		for (auto& widget : mWidgets)
			widget->RenderInternal();
	}

	void ImGuiLayer::End()
	{
		ImGui::End();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((f32)app.MainWindow().Width(), (f32)app.MainWindow().Height());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* currentContextBackup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(currentContextBackup);
		}

		auto removedIt = std::remove_if(mWidgets.begin(), mWidgets.end(), [](auto& inWidget) { return inWidget->WasClosed(); });
		mWidgets.erase(removedIt, mWidgets.end());
	}

}

