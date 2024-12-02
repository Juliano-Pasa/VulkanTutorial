#pragma once

#include "LvePipeline.h"
#include "LveDevice.h"
#include "LveGameObject.h"
#include "LveCamera.h"
#include "LveFrameInfo.h"

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

		void RenderGameObjects(FrameInfo& frameInfo, std::vector<LveGameObject>& gameObjects);


	private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);

		LveDevice& lveDevice;
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;

	};
}