#pragma once

#include "LveDevice.h"

#include <string>
#include <vector>

namespace lve
{
	struct PipelineConfigInfo {};

	class LvePipeline
	{
	public:
		LvePipeline(LveDevice& device, const std::string& vertFilePath, const std::string fragFilePath, const PipelineConfigInfo& configInfo);
		~LvePipeline() {};

		LvePipeline(const LvePipeline&) = delete;
		void operator=(const LvePipeline&) = delete;

		static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> ReadFile(const std::string& filePath);
		void CreateGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, PipelineConfigInfo configInfo);

		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		LveDevice &lveDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}