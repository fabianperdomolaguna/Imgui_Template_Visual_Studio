module;

#include <unordered_map>
#include <string>
#include <any>

#include "GLFW/glfw3.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

export module ImguiContext;

import Window;
import SettingsConfig;
import ColorStyles;

std::unordered_map <std::string, std::any> color_styles{ {"Dark", SetDarkTheme}, {"Light", SetLightTheme} };

export class ImguiContext
{
public:
	ImguiContext(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Load default font
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("fonts/Roboto_Regular.ttf", 18.0f, &fontConfig);
		ImFont* italic = io.Fonts->AddFontFromFileTTF("fonts/Roboto_Italic.ttf", 18.0f, &fontConfig);
		ImFont* bold = io.Fonts->AddFontFromFileTTF("fonts/Roboto_Bold.ttf", 18.0f, &fontConfig);

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.WindowBorderSize = 0.0f;
			style.WindowPadding = ImVec2(0.0f, 0.0f);
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		std::string current_style = GetConfigVariable("GuiStyle");
		std::any_cast <void (*) ()> (color_styles[current_style]) ();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 400 core");
	}

	~ImguiContext()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void PreRender()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::Begin("InvisibleWindow", nullptr, window_flags);
		ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
		ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::End();
	}

	void PostRender()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
};

export void UpdateTheme()
{
	std::string current_style = GetConfigVariable("GuiStyle");
	std::any_cast <void (*) ()> (color_styles[current_style]) ();
}