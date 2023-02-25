#include "pnpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "Pine/Core/Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <ImGuizmo.h>

namespace Pine
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		PN_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); static_cast<void>(io);
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		float fontSize = 18.0f;

		io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Bold.ttf", fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Regular.ttf", fontSize);
		io.FontGlobalScale = 1.0f;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.GrabRounding = style.FrameRounding = 2.5f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{
		PN_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		PN_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		PN_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().GetWidth()), static_cast<float>(app.GetWindow().GetHeight()));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& style = ImGui::GetStyle();
		
		style.Colors[ImGuiCol_Text] = ImVec4(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
		
		// Headers
		style.Colors[ImGuiCol_Header] = ImVec4(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 255.0f / 255.0f);
		
		// Buttons
		style.Colors[ImGuiCol_Button] = ImVec4(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(87.0f / 255.0f, 87.0f / 255.0f, 87.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0f / 255.0f, 112.0f / 255.0f, 224.0f / 255.0f, 255.0f / 255.0f);

		// Frame BG
		style.Colors[ImGuiCol_FrameBg] = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 255.0f / 255.0f);

		// Tabs
		style.Colors[ImGuiCol_Tab] = ImVec4(21.0f / 255.0f, 21.0f / 255.0f, 21.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(21.0f / 255.0f, 21.0f / 255.0f, 21.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 255.0f / 255.0f);
		
		// Title
		style.Colors[ImGuiCol_TitleBg] = ImVec4(21.0f / 255.0f, 21.0f / 255.0f, 21.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(21.0f / 255.0f, 21.0f / 255.0f, 21.0f / 255.0f, 255.0f / 255.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(21.0f / 255.0f, 21.0f / 255.0f, 21.0f / 255.0f, 255.0f / 255.0f);
	}
}
