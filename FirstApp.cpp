#include "FirstApp.h"
#include "SimpleRenderSystem.h"
#include "PointLightSystem.h"
#include "LveCamera.h"
#include "KeyboardMovementController.h"
#include "LveBuffer.h"

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
    struct GlobalUbo
    {
        glm::mat4 projection{ 1.f };
        glm::mat4 view{ 1.f };
        glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, 0.02f };
        glm::vec4 lightPosition{ -1.f };
        glm::vec4 lightColor{ 1.f };
    };


	FirstApp::FirstApp()
	{
        globalPool = LveDescriptorPool::Builder(lveDevice)
            .setMaxSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, LveSwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
		LoadGameObjects();
	}

	FirstApp::~FirstApp(){}

	void FirstApp::run()
	{
        std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<LveBuffer>
                (
                    lveDevice,
                    sizeof(GlobalUbo),
                    1,
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                );
            uboBuffers[i]->map();
        }

        auto globalSetLayout = LveDescriptorSetLayout::Builder(lveDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            LveDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
		PointLightSystem pointLightSystem { lveDevice, lveRenderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        LveCamera camera{};
        camera.SetViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = LveGameObject::CreateGameObject();
        viewerObject.transform.translation.z = -2.5f;
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
                int frameIndex = lveRenderer.GetFrameIndex();
                FrameInfo frameInfo
                {
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjects
                };

                GlobalUbo ubo{};
                ubo.projection = camera.GetProjection();
                ubo.view = camera.GetView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

				lveRenderer.BeginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.RenderGameObjects(frameInfo);
                pointLightSystem.Render(frameInfo);
				lveRenderer.EndSwapChainRenderPass(commandBuffer);
				lveRenderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

    void FirstApp::LoadGameObjects()
    {
		std::shared_ptr<LveModel> lveModel = LveModel::CreateModelFromFile(lveDevice, "models/flat_vase.obj");

        auto flatVase = LveGameObject::CreateGameObject();
        flatVase.model = lveModel;
        flatVase.transform.translation = { -0.5f, 0.5f, 0.f };
        flatVase.transform.scale = {3.f, 1.5f, 3.f};

        gameObjects.emplace(flatVase.GetId(), std::move(flatVase));

        lveModel = LveModel::CreateModelFromFile(lveDevice, "models/smooth_vase.obj");

        auto smoothVase = LveGameObject::CreateGameObject();
        smoothVase.model = lveModel;
        smoothVase.transform.translation = { 0.5f, 0.5f, 0.f };
        smoothVase.transform.scale = { 3.f, 1.5f, 3.f };

        gameObjects.emplace(smoothVase.GetId(), std::move(smoothVase));

        lveModel = LveModel::CreateModelFromFile(lveDevice, "models/quad.obj");

        auto floor = LveGameObject::CreateGameObject();
        floor.model = lveModel;
        floor.transform.translation = { 0.f, 0.5f, 0.f };
        floor.transform.scale = { 3.f, 1.f, 3.f };

        gameObjects.emplace(floor.GetId(), std::move(floor));
    }
}