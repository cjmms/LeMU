#pragma once

#include "window.hpp"
#include "Pipeline.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

#include <memory>

namespace LeMU
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();
		
		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void CreatePipelinelayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void DrawFrame();

		Window window{ WIDTH, HEIGHT, "Hello Vulkan" };
		Device device{window};
		SwapChain swapChain{ device, window.getExtent() };
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};






}