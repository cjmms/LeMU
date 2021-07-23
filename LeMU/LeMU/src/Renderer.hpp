#pragma once

#include "Device.hpp"
#include "SwapChain.hpp"
#include "window.hpp"


// std
#include <memory>
#include <vector>


namespace LeMU {
	class Renderer {
	public:

		Renderer(Window &window, Device &device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		inline VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
		inline bool isFrameInProgress()const { return isFrameStarted; };

		VkCommandBuffer getCurrentCommandBuffer() const;

		// acquire next image, begin command buffer
		VkCommandBuffer beginFrame();
		
		// end command buffer, submit command buffer
		void endFrame();

		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);


	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();


		Window &window;
		Device &device;
		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		// keep track of current frame
		uint32_t currentImageIndex;
		bool isFrameStarted;
	};
}  // namespace lve