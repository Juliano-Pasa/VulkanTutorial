#pragma once

#include <string>
#include <vector>

namespace lve
{
	class LvePipeline
	{
	public:
		LvePipeline(const std::string& vertFilePath, const std::string fragFilePath);

	private:
		static std::vector<char> ReadFile(const std::string& filePath);
		void CreateGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
	};
}