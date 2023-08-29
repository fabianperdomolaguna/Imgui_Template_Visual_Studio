module;

#include <memory>

#include "imgui.h"

export module RenderScene;

import Application;
import Layer;
import Texture;
import Image;
import Window;
import Framebuffer;
import Vertex;

export class SimpleRender : public Layer
{
	Application* m_app;

	std::unique_ptr<GlVertex> m_vertex;
	std::unique_ptr<GlFramebuffer> m_framebuffer;

	std::unique_ptr<Texture> image_texture;

public:
	SimpleRender(Application* app) : m_app(app) {}

	void OnAttach()
	{
		m_vertex = std::make_unique<GlVertex>("Shaders_Files/vs.shader", "Shaders_Files/fs.shader");
		m_framebuffer = std::make_unique<GlFramebuffer>(m_app->m_window->m_width, m_app->m_window->m_height);
		m_vertex->CreateBuffers();

		auto image = ReadImage("cpp_logo.jpg");
		image_texture = std::make_unique<Texture>(image.data, image.width, image.height, image.format);
	}

	void OnRender()
	{
		static float clear_color[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
		static float fraction = 0.0f;
		static int counter = 0;

		ImGui::Begin("Render - Slider - Color - FPS");
		ImGui::SliderFloat("Range 0 to 1", &fraction, 0.0f, 1.0f);
		ImGui::ColorEdit3("Color selection", clear_color);

		if (ImGui::Button("Counter"))
			counter++;
		ImGui::SameLine();
		ImGui::Text("You have clicked the button %d times", counter);

		ImGui::Text("Application average - %.3f ms/frame - (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::ShowDemoWindow();

		ImGui::Begin("Image Texture");
		ImGui::Image(reinterpret_cast<void*>((uint64_t)image_texture->get_texture()),
			{ (float)image_texture->m_width, (float)image_texture->m_height });
		ImGui::End();

		m_framebuffer->Bind();
		m_vertex->Draw();
		m_framebuffer->Unbind();

		ImGui::Begin("Scene");
		uint64_t textureID = m_framebuffer->get_texture();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
	}
};