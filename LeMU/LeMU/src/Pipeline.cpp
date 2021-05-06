#include "Pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

namespace LeMU
{
	Pipeline::Pipeline(
		Device& device,
		const std::string& vertFilePath,
		const std::string& fragFilePath,
		const PipelineConfigInfo configInfo) : device(device)
	{
		CreateGraphicPipeline(vertFilePath, fragFilePath, configInfo);
	}

	std::vector<char> Pipeline::ReadFile(const std::string& filePath)
	{
		std::ifstream file{filePath, std::ios::ate | std::ios::binary};

		if (!file.is_open()) throw std::runtime_error("failed to open file: " + filePath);
		
		size_t fileSize = static_cast<size_t>(file.tellg());	// tellg() is the end of the file
		std::vector<char> buffer(fileSize);

		file.seekg(0);		// move to beginning of the file
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}


	void Pipeline::CreateGraphicPipeline(
		const std::string& vertFilePath, 
		const std::string& fragFilePath, 
		const PipelineConfigInfo configInfo)
	{
		auto vertCode = ReadFile(vertFilePath);
		auto fragCode = ReadFile(fragFilePath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
		std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
	}



	void Pipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();

		// createInfo requires an uint32_t pointer to data
		// it's fine to cast the pointer since data is inside a c++ vector
		// the data inside vector are alligned even for the worst case.
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module");
		}
	}




	PipelineConfigInfo Pipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
	{
		PipelineConfigInfo configInfo{};

		return configInfo;
	}


}
