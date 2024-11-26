#pragma once

#include "LveWindow.h"
#include "LveDevice.h"
#include "LveSwapChain.h"

#include <memory>
#include <cassert>
#include <vector>

namespace lve
{
	class LveRenderer
	{
	public:
		LveRenderer(LveWindow& window, LveDevice& device);
		~LveRenderer();

		LveRenderer(const LveRenderer&) = delete;
		LveRenderer& operator=(const LveRenderer&) = delete;

		VkRenderPass GetSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); }
		bool isFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer GetCurrentCommandBuffer() const
		{
			assert(isFrameStarted && "Cannot get command buffer when frame is not in progress.");
			return commandBuffers[currentFrameIndex];
		}

		int GetFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame is not in progress.");
			return currentFrameIndex;
		}

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();

		LveWindow& lveWindow;
		LveDevice& lveDevice;
		std::unique_ptr<LveSwapChain> lveSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted;
	};
}