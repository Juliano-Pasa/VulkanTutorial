#pragma once

#include "LvePipeline.h"
#include "LveDevice.h"
#include "LveGameObject.h"
#include "LveCamera.h"

#include <memory>
#include <vector>

namespace lve
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<LveGameObject>& gameObjects, const LveCamera &camera);


	private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);

		LveDevice& lveDevice;
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;

	};
}