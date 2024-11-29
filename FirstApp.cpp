#include "FirstApp.h"
#include "SimpleRenderSystem.h"
#include "LveCamera.h"
#include "KeyboardMovementController.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONEw
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <stdexcept>
#include <cassert>
#include <chrono>
#include <array>

namespace lve
{
	FirstApp::FirstApp()
	{
		LoadGameObjects();
	}

	FirstApp::~FirstApp(){}

	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.GetSwapChainRenderPass() };
        LveCamera camera{};
        camera.SetViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = LveGameObject::CreateGameObject();
        KeyboardMovementController cameraControler{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!lveWindow.ShouldClose())
		{
			glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraControler.MoveInPlaneXZ(lveWindow.GetGLFWwindow(), frameTime, viewerObject);
            camera.SetViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRenderer.GetAspectRatio();
            camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

			if (auto commandBuffer = lveRenderer.BeginFrame())
			{
				lveRenderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(commandBuffer, gameObjects, camera);
				lveRenderer.EndSwapChainRenderPass(commandBuffer);
				lveRenderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

    void FirstApp::LoadGameObjects()
    {
		std::shared_ptr<LveModel> lveModel = LveModel::CreateModelFromFile(lveDevice, "models/smooth_vase.obj");

        auto gameObject = LveGameObject::CreateGameObject();
        gameObject.model = lveModel;
        gameObject.transform.translation = { 0.f, 0.f, 2.5f };
        gameObject.transform.scale = glm::vec3(3.f);

        gameObjects.push_back(std::move(gameObject));
    }
}