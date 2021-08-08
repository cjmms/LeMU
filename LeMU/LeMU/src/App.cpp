

#include "App.hpp"

#include "KeyboardController.hpp"
#include "Camera.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/constants.hpp>
#include "RenderSystem.hpp"
#include "Image.hpp"

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

        Image image("statue.jpg", device);

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
 



    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<Model> model = Model::createModelFromFile(device, "models/viking_room.obj");

        auto obj = GameObject::createGameObject();
        obj.model = model;
        obj.transform.translation = { 0.0f, 0.0f, 2.5f };
        obj.transform.scale = {3.0f};

        gameObjects.push_back(std::move(obj));
    }



}  // namespace lve