#pragma once

#define GLFW_INCLUDE_VULKAN
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
		VkExtent2D GetExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		bool WasWindowResized() { return frameBufferResized; }
		void ResetWindowResizedFlag() { frameBufferResized = false; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void InitializeWindow();
		static void FramebufferResizedCallback(GLFWwindow* window, int width, int height);

		int width;
		int height;
		bool frameBufferResized = false;

		std::string windowName;
		GLFWwindow* window;
	};
}