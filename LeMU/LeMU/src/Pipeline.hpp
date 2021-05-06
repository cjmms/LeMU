#pragma once

#include <vector>
#include <iostream>
#include <string>


namespace LeMU
{

	class Pipeline
	{
	public:
		Pipeline(const std::string& vertFilePath, const std::string& fragFilePath);


	private:
		static std::vector<char> ReadFile(const std::string& filePath);


		void CreateGraphicPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
	};

}

