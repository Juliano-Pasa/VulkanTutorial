#pragma once

#include "LveModel.h"

#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <unordered_map>

namespace lve
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.0f, 1.0f , 1.0f};
		glm::vec3 rotation;

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};
	class LveGameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, LveGameObject>;

		static LveGameObject CreateGameObject()
		{
			static id_t currentId = 0;
			return LveGameObject{ currentId++ };
		}

		LveGameObject(const LveGameObject&) = delete;
		LveGameObject &operator=(const LveGameObject&) = delete;
		LveGameObject(LveGameObject &&) = default;
		LveGameObject& operator=(LveGameObject &&) = default;

		id_t GetId() { return id; }

		std::shared_ptr<LveModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		LveGameObject(id_t objId) : id{ objId } {}

		id_t id;
	};
}