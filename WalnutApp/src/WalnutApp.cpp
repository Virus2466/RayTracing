#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() : m_Camera(45.0f,0.1f,100.0f)
	{
		Material& pinkSphere = m_Scene.Materials.emplace_back();
		pinkSphere.Albedo = { 1.0f,0.0f,1.0f };
		pinkSphere.Roughness = 0.0f;

		Material& blueSphere = m_Scene.Materials.emplace_back();
		blueSphere.Albedo = { 0.3f,0.3f,1.0f };
		blueSphere.Roughness = 0.1f;

		Material& orangeSphere = m_Scene.Materials.emplace_back();
		orangeSphere.Albedo = { 0.8f,0.5f,0.2f };
		orangeSphere.Roughness = 0.1f;
		orangeSphere.EmissionColor = orangeSphere.Albedo;
		orangeSphere.EmissionPower = 2.0f;

		{

		Sphere sphere;
		sphere.Position = { 0.0f,0.0f,0.0f };
		sphere.radius = 1.0f;
		sphere.MaterialIndex = 0;
		m_Scene.Spheres.push_back(sphere);
		}


		{

			Sphere sphere;
			sphere.Position = { 2.0f,0.0f,0.0f };
			sphere.radius = 1.0f;
			sphere.MaterialIndex = 2;
			m_Scene.Spheres.push_back(sphere);
		}

		{

		Sphere sphere;
		sphere.Position = {0.0f,-101.0f,-5.0f};
		sphere.radius = 100.0f;
		sphere.MaterialIndex = 1;
		m_Scene.Spheres.push_back(sphere);
		}


	}

	virtual void OnUpdate(float ts)override 
	{
		if (m_Camera.OnUpdate(ts))
			m_Renderer.ResetFrameIndex();
	}
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Render Options");
		ImGui::Text("Last Render : %.3fms ", m_TimeElasped);
		ImGui::Button("Change Color ");
		ImGui::Spacing();
		if (ImGui::Button("Render")) {
			Render();
		}

		ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulation);

		if (ImGui::Button("Reset"))
			m_Renderer.ResetFrameIndex();

		ImGui::End();

		ImGui::Begin("Scene");
		for (size_t i = 0; i < m_Scene.Spheres.size(); i++) {
			
			ImGui::PushID(i);
			
			Sphere& sphere = m_Scene.Spheres[i];

			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.radius, 0.1f);
			ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, (int)m_Scene.Materials.size() - 1);

		
			ImGui::Separator();


			ImGui::PopID();
		}

		for (size_t i = 0; i < m_Scene.Materials.size(); i++) {
			

				ImGui::PushID(i);


				Material& material = m_Scene.Materials[i];

				ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
				ImGui::DragFloat("Roughness", &material.Roughness,0.5f , 0.0f , 1.0f);
				ImGui::DragFloat("Metallic", &material.Metallic,0.5f, 0.0f, 1.0f);
				ImGui::ColorEdit3("Emission Color ", glm::value_ptr(material.EmissionColor));
				ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);

				ImGui::Separator();


				ImGui::PopID();
			
		}




		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

			
		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;


		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth() , (float)image->GetHeight() },ImVec2(0,1),ImVec2(1,0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}


	void Render() {
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene,m_Camera);

		m_TimeElasped = timer.ElapsedMillis();
	}

	/*void changeColor() {
		
	}*/


private:
	// 32 bytes -> RGBA channel is calculate with it.
	/*uint32_t* m_ImageData = nullptr;*/
	Renderer m_Renderer;
	Camera  m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeight = 0;
	float m_TimeElasped = 0;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "RayTracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}



