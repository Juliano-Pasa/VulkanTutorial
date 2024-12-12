#pragma once

#include "LveCamera.h"
#include "LveGameObject.h"

#include <vulkan/vulkan.h>

namespace lve
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		LveCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		LveGameObject::Map& gameObjects;
	};
}