#include "App.hpp"



#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/constants.hpp>
#include "RenderSystem.hpp"

// std
#include <array>
#include <stdexcept>
#include <iostream>

namespace LeMU {


    FirstApp::FirstApp() {
        loadGameObjects();
        
    }

    FirstApp::~FirstApp() {  }

    void FirstApp::run() {

        RenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};

        while (!window.shouldClose()) {
            glfwPollEvents();
            
            if (auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);

                renderSystem.renderGameObjects(commandBuffer, gameObjects);

                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }
 


    void FirstApp::loadGameObjects()
    {
        std::vector<Model::Vertex> vertices
        {
            { {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} },
            { {0.5f, 0.5f}, {0.0f, 1.0f, 0.0f} },
            { {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} }
        };

        auto model = std::make_shared<Model>(device, vertices);

        auto triangle = GameObject::createGameObject();
        triangle.model = model;
        triangle.color = { 0.1f, 0.8f, 0.1f };
        triangle.transform2D.translation.x = 0.2f;
        triangle.transform2D.scale = { 2.0f, 0.5f };
        triangle.transform2D.rotation = 0.25f * glm::two_pi<float>();   // radians, 90 degree

        gameObjects.push_back(std::move(triangle));
    }



}  // namespace lve