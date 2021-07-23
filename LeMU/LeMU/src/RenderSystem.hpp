#pragma once



#include "Device.hpp"
#include "Pipeline.hpp"
#include "GameObject.hpp"

// std
#include <memory>
#include <vector>

namespace LeMU {
	class RenderSystem {
	public:

		RenderSystem(Device &device, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects);

	private:

		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
		void renderGameObjects(VkCommandBuffer commandBuffer);

		Device &device;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}  // namespace lve