#include "FirstApp.h"

#include <stdexcept>
#include <array>

namespace lve
{
	FirstApp::FirstApp()
	{
		//LoadModels();
		LoadSierpinski(7);
		CreatePipelineLayout();
		CreatePipeline();
		CreateCommandBuffers();
	}

	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApp::run()
	{
		while (!lveWindow.ShouldClose())
		{
			glfwPollEvents();
			DrawFrame();
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::CreatePipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipeline layout!");
	}

	void FirstApp::CreatePipeline()
	{
		auto pipelineConfig = LvePipeline::DefaultPipelineConfigInfo(lveSwapChain.width(), lveSwapChain.height());
		pipelineConfig.renderPass = lveSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeline>(lveDevice, "SimpleShader.vert.spv", "SimpleShader.frag.spv", pipelineConfig);
	}

	void FirstApp::CreateCommandBuffers()
	{
		commandBuffers.resize(lveSwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = lveDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers.");

		for (int i = 0; i < commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) 
				throw std::runtime_error("Failed to begin recording command buffer.");

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = lveSwapChain.getRenderPass();
			renderPassInfo.framebuffer = lveSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = lveSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.25f, 0.25f, 0.26f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0};
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			lvePipeline->Bind(commandBuffers[i]);
			lveModel->Bind(commandBuffers[i]);
			lveModel->Draw(commandBuffers[i]);

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to record command buffer.");
		}
	}

	void FirstApp::DrawFrame()
	{
		uint32_t imageIndex;
		auto result = lveSwapChain.acquireNextImage(&imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire swap chain image.");

		result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to present swap chain image.");
	}

	void FirstApp::LoadModels()
	{
		std::vector<LveModel::Vertex> vertices
		{
			{{0.0f, -0.8f}},
			{{0.8f, 0.8f}},
			{{-0.8f, 0.8f}}
		};

		lveModel = std::make_unique<LveModel>(lveDevice, vertices);
	}

	void FirstApp::LoadSierpinski(int sierpinskiDivisions)
	{
		if (sierpinskiDivisions < 1)
		{
			LoadModels();
			return;
		}

		size_t trianglesCount = static_cast<size_t>((pow(3, sierpinskiDivisions + 1) - 1.0) * 0.5);
		std::vector<std::vector<LveModel::Vertex>> triangles(trianglesCount);

		triangles[0] = std::vector<LveModel::Vertex>
		{
			{{0.0f, -0.8f}},
			{{0.8f, 0.8f}},
			{{-0.8f, 0.8f}}
		};

		int currentPosition = 0, currentPower = 1;
		for (int i = 0; i < sierpinskiDivisions; i++)
		{
			for (int j = 0; j < currentPower; j++)
			{
				LveModel::Vertex leftVertex = LveModel::Vertex::GetMidPoint(triangles[currentPosition][0], triangles[currentPosition][1]);
				LveModel::Vertex bottomVertex = LveModel::Vertex::GetMidPoint(triangles[currentPosition][1], triangles[currentPosition][2]);
				LveModel::Vertex rightVertex = LveModel::Vertex::GetMidPoint(triangles[currentPosition][2], triangles[currentPosition][0]);

				int childIndex = (currentPosition + 1) * 3 - 2;
				triangles[childIndex] = std::vector<LveModel::Vertex>
				{
					triangles[currentPosition][0],
					leftVertex,
					rightVertex
				};
				triangles[childIndex+1] = std::vector<LveModel::Vertex>
				{
					leftVertex,
					triangles[currentPosition][1],
					bottomVertex
				};
				triangles[childIndex+2] = std::vector<LveModel::Vertex>
				{
					rightVertex,
					bottomVertex,
					triangles[currentPosition][2]
				};
				currentPosition++;
			}
			currentPower *= 3;
		}

		std::vector<LveModel::Vertex> vertices;
		for (int i = triangles.size() - 1; currentPower > 0; i--, currentPower--)
		{
			vertices.push_back(triangles[i][0]);
			vertices.push_back(triangles[i][1]);
			vertices.push_back(triangles[i][2]);
		}

		lveModel = std::make_unique<LveModel>(lveDevice, vertices);
	}
}