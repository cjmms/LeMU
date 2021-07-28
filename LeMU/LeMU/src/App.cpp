#include "App.hpp"

#include "KeyboardController.hpp"
#include "Camera.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/constants.hpp>
#include "RenderSystem.hpp"

// std
#include <array>
#include <chrono>
#include <stdexcept>
#include <iostream>

namespace LeMU {


    FirstApp::FirstApp() {
        loadGameObjects();
        
    }

    FirstApp::~FirstApp() {  }

    void FirstApp::run() {

        // render system
        RenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};
        
        // camera
        Camera camera{};
        camera.setViewDirection(glm::vec3(0.0), glm::vec3(0.0f, 0.0f, 1.0f));
        auto cameraObject = GameObject::createGameObject(); // camera, used to store current state

        // timer
        auto currentTime = std::chrono::high_resolution_clock::now();

        // Kayboard Controller
        KeyboardController cameraController{};

        while (!window.shouldClose()) {
            glfwPollEvents();

            // declear after glfwPollEvents(), because glfwPollEvents() may block game loop
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;  // update current time

            // update camera state
            cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, cameraObject);
            camera.setViewYXZ(cameraObject.transform.translation, cameraObject.transform.rotation);

            float aspect = renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);


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
        Model::Builder builder{};

        builder.vertices = {
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

        for (auto& v : builder.vertices) {
            v.position += offset;
        }


        builder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                          12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };


        return std::make_unique<Model>(device, builder);
    
    }



    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<Model> model = createCubeModel(device, { 0.0f, 0.0f, 0.0f });

        auto cube = GameObject::createGameObject();
        cube.model = model;
        cube.transform.translation = { 0.0f, 0.0f, 2.5f };
        cube.transform.scale = { 0.5f, 0.5f, 0.5f };

        gameObjects.push_back(std::move(cube));
    }



}  // namespace lve