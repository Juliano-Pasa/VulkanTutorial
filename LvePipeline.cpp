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

	void LvePipeline::CreateGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		auto vertCode = ReadFile(vertFilePath);
		auto fragCode = ReadFile(fragFilePath);

		std::cout << "Vertex shader code size: " << vertCode.size() << "\n";
		std::cout << "Fragment shader code size: " << fragCode.size() << "\n";
	}

	LvePipeline::LvePipeline(const std::string& vertFilePath, const std::string fragFilePath)
	{
		CreateGraphicsPipeline(vertFilePath, fragFilePath);
	}
}