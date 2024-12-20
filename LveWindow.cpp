#include "LveWindow.h"

#include <stdexcept>

namespace lve {
	LveWindow::LveWindow(int w, int h, std::string name): width(w), height(h), windowName(name)
	{
		InitializeWindow();
	}

	LveWindow::~LveWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void LveWindow::InitializeWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, FramebufferResizedCallback);
	}

	void LveWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) 
			throw std::runtime_error("Failed to create window surface.");
	}

	void LveWindow::FramebufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto lveWindow = reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));
		lveWindow->frameBufferResized = true;
		lveWindow->width = width;
		lveWindow->height = height;
	}
}