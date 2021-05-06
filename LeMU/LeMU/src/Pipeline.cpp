#include "Pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <vector>

namespace LeMU
{
	Pipeline::Pipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		CreateGraphicPipeline(vertFilePath, fragFilePath);
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


	void Pipeline::CreateGraphicPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		auto vertCode = ReadFile(vertFilePath);
		auto fragCode = ReadFile(fragFilePath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
		std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
	}

}
