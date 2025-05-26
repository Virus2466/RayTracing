#pragma once
#include "Walnut/Image.h"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

#include<memory>
#include<glm/glm.hpp>



class Renderer {
public:

	struct Settings {
		bool Accumulation = true;
	};

	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene,const Camera& camera);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

	void ResetFrameIndex() { m_FrameIndex = 1; }
	Settings& GetSettings() { return m_Settings; }

private:
	struct HitPayload {
		float hitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};

	glm::vec4 PerPixel(uint32_t x, uint32_t y); // RayGen Shader as in Vulkan Pipeline
	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray , float hitDistance , int objectIndex);
	HitPayload Miss(const Ray& ray);

private:
	std::shared_ptr<Walnut::Image>m_FinalImage;
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;
	 Settings m_Settings;

	 std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_AccumulationData = nullptr;
	//glm::vec3 changeColor;
     
	uint32_t m_FrameIndex = 1;

};