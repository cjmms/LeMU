#include "App.hpp"


namespace LeMU
{
	FirstApp::FirstApp()
	{
		CreatePipelinelayout();
		CreatePipeline();
		CreateCommandBuffers();
	}


	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
	}




	void FirstApp::run()
	{
		while (!window.ShouldClose())
		{
			glfwPollEvents();
		}
	}




	void FirstApp::CreatePipelinelayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout.");
		}

	}



	void FirstApp::CreatePipeline()
	{
		auto pipelineConfig = Pipeline::DefaultPipelineConfigInfo(swapChain.width(), swapChain.height());
		pipelineConfig.renderPass = swapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<Pipeline>(
			device,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}


	void FirstApp::CreateCommandBuffers()
	{

	}

	void FirstApp::DrawFrame()
	{

	}


}