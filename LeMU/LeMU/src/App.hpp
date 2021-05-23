#pragma once

#include "Device.hpp"
#include "Pipeline.hpp"
#include "SwapChain.hpp"
#include "window.hpp"

// std
#include <memory>
#include <vector>

namespace LeMU {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		Window window{ WIDTH, HEIGHT, "Hello Vulkan!" };
		Device device{ window };
		SwapChain swapChain{ device, window.getExtent() };
		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}  // namespace lve