#include "App.hpp"



#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/constants.hpp>


// std
#include <array>
#include <stdexcept>
#include <iostream>

namespace LeMU {

    struct SimplePushConstantData
    {
        glm::mat2 transform{1.f};   // default identity matrix as transformation matrix
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };




    FirstApp::FirstApp() {
        loadGameObjects();
        createPipelineLayout();
        createPipeline();
    }

    FirstApp::~FirstApp() { vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr); }

    void FirstApp::run() {
        while (!window.shouldClose()) {
            glfwPollEvents();
            
            if (auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);

                renderGameObjects(commandBuffer);

                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }

    void FirstApp::createPipelineLayout() {

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);


        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }


    void FirstApp::createPipeline() 
    {
        assert(swapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(swapChain != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderer.getSwapChainRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(
            device,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
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


    void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer)
    {
        pipeline->bind(commandBuffer);

        for (auto& obj: gameObjects)
        {
            SimplePushConstantData push{};
            push.offset = obj.transform2D.translation;
            push.color = obj.color;
            push.transform = obj.transform2D.mat2();

            vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);

            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }

    }



}  // namespace lve