#pragma once

#include "LveWindow.h"
#include "LveDevice.h"
#include "LveRenderer.h"
#include "LveGameObject.h"

#include <memory>
#include <vector>

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp &operator=(const FirstApp&) = delete;


		void run();

	private:
		void LoadGameObjects();

		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		LveDevice lveDevice{ lveWindow };
		LveRenderer lveRenderer{ lveWindow, lveDevice };

		std::vector<LveGameObject> gameObjects;
	};
}