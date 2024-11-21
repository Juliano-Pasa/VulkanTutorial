#pragma once


#include <GLFW/glfw3.h>
#include <string>

namespace lve {
	class LveWindow
	{
	public:
		LveWindow(int w, int h, std::string name);
		~LveWindow();

		LveWindow(const LveWindow&) = delete;
		LveWindow& operator=(const LveWindow) = delete;

		bool ShouldClose() { return glfwWindowShouldClose(window); }

	private:
		void InitializeWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
}