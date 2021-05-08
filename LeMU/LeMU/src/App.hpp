#pragma once

#include "window.hpp"
#include "Pipeline.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

namespace LeMU
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;


		void run();
	private:
		Window window{ WIDTH, HEIGHT, "Hello Vulkan" };
		Device device{window};
		SwapChain swapChain{ device, window.getExtent() };
		Pipeline pipeline{
			device,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			Pipeline::DefaultPipelineConfigInfo(WIDTH, HEIGHT) };
	};






}