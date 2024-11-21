#include "LvePipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace lve
{
	std::vector<char> LvePipeline::ReadFile(const std::string& filePath)
	{
		std::ifstream file{ filePath, std::ios::ate | std::ios::binary };

		if (!file.is_open()) throw std::runtime_error("Failed to open file: " + filePath);

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	LvePipeline::LvePipeline(
		LveDevice& device, 
		const std::string& vertFilePath, 
		const std::string fragFilePath, 
		const PipelineConfigInfo& configInfo) : lveDevice{device}
	{
		CreateGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
	}

	void LvePipeline::CreateGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, PipelineConfigInfo configInfo)
	{
		auto vertCode = ReadFile(vertFilePath);
		auto fragCode = ReadFile(fragFilePath);

		std::cout << "Vertex shader code size: " << vertCode.size() << "\n";
		std::cout << "Fragment shader code size: " << fragCode.size() << "\n";
	}

	void LvePipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) throw std::runtime_error("Failed to create shader module");
	}

	PipelineConfigInfo LvePipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
	{
		PipelineConfigInfo configInfo{};

		return configInfo;
	}
}