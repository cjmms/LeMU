#include "App.hpp"


#include "Camera.hpp"


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
        Camera camera{};
        camera.setOrthographicProjection(-1, 1, -1, 1, -1, 1);

        while (!window.shouldClose()) {
            glfwPollEvents();
            
            if (auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);

                renderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }
 

    std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset) {
    

        std::vector<Model::Vertex> vertices{
            // left face (white)
         {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
         {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
         {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
         {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
         {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
         {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

         // right face (yellow)
         {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
         {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
         {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
         {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
         {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
         {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

         // top face (orange, remember y axis points down)
         {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
         {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
         {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
         {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
         {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
         {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

         // bottom face (red)
         {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
         {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
         {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
         {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
         {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
         {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

         // nose face (blue)
         {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
         {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
         {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
         {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
         {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
         {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

         // tail face (green)
         {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
         {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
         {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
         {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
         {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
         {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };

        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<Model>(device, vertices);
    
    }



    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<Model> model = createCubeModel(device, { 0.0f, 0.0f, 0.0f });

        auto cube = GameObject::createGameObject();
        cube.model = model;
        cube.transform.translation = { 0.0f, 0.0f, 0.5f };
        cube.transform.scale = { 0.5f, 0.5f, 0.5f };

        gameObjects.push_back(std::move(cube));
    }



}  // namespace lve