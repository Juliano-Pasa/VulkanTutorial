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

    std::unique_ptr<LveModel> CreateCubeModel(LveDevice& device, glm::vec3 offset) {
        LveModel::Builder modelBuilder{};
        modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };
        for (auto& v : modelBuilder.vertices) {
            v.position += offset;
        }

        modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

        return std::make_unique<LveModel>(device, modelBuilder);
    }

    void FirstApp::LoadGameObjects()
    {
        std::shared_ptr<LveModel> lveModel = CreateCubeModel(lveDevice, { 0.f, 0.f, 0.f });

        auto cube = LveGameObject::CreateGameObject();
        cube.model = lveModel;
        cube.transform.translation = { 0.f, 0.f, 2.5f };
        cube.transform.scale = { .5f, .5f, .5f };

        gameObjects.push_back(std::move(cube));
    }
}