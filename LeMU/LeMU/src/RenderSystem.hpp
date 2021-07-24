#pragma once


#include "Camera.hpp"
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

		void renderGameObjects( VkCommandBuffer commandBuffer, 
								std::vector<GameObject> &gameObjects, 
								const Camera &camera);

	private:

		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		Device &device;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}  // namespace lve