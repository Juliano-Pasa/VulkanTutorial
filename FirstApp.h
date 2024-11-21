#pragma once

#include "LveWindow.h"
#include "LvePipeline.h"

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		LveWindow lveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		LvePipeline lvePipeline{ "SimpleShader.vert.spv", "SimpleShader.frag.spv" };
	};
}