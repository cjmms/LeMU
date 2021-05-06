#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "Device.hpp"


namespace LeMU
{
	struct PipelineConfigInfo {};
	 

	class Pipeline
	{
	public:
		Pipeline(
			Device& device, 
			const std::string& vertFilePath, 
			const std::string& fragFilePath, 
			const PipelineConfigInfo configInfo );

		~Pipeline(){}

		Pipeline(const Pipeline&) = delete;
		void operator=(const Pipeline&) = delete;

		static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> ReadFile(const std::string& filePath);


		void CreateGraphicPipeline(
			const std::string& vertFilePath, 
			const std::string& fragFilePath, 
			const PipelineConfigInfo configInfo);

		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		Device& device;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};

}

