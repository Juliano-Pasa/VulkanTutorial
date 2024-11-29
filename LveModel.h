#pragma once

#include "LveDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONEw
#include  "glm/glm.hpp"

#include <memory>
#include <vector>

namespace lve
{
	class LveModel
	{
	public:

		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

			bool operator==(const Vertex& other) const 
			{
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void LoadModel(const std::string& filePath);
		};

		LveModel(LveDevice& device, const LveModel::Builder &builder);
		~LveModel();

		LveModel(const LveModel&) = delete;
		LveModel& operator=(const LveModel&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		static std::unique_ptr<LveModel> CreateModelFromFile(LveDevice& device, const std::string& filePath);

	private:
		LveDevice& lveDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
		
		bool hasIndexBuffer = false;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		uint32_t indexCount;

		void CreateVertexBuffers(const std::vector<Vertex>& vertices);
		void CreateIndexBuffers(const std::vector<uint32_t>& indices);
	};
}