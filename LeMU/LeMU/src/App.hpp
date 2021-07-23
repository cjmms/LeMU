#pragma once

#include "Device.hpp"
#include "Pipeline.hpp"
#include "Renderer.hpp"
#include "window.hpp"
#include "GameObject.hpp"

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
		void loadGameObjects();
		void createPipelineLayout();
		void createPipeline();
		void renderGameObjects(VkCommandBuffer commandBuffer);

		Window window{ WIDTH, HEIGHT, "Hello Vulkan!" };
		Device device{ window };
		Renderer renderer{window, device};

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<GameObject> gameObjects;

	};
}  // namespace lve