#pragma once


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>

#include <vector>
#include "Device.hpp"

namespace LeMU
{

	// read vertex data from files
	// allocate memory
	// copy data from memory to GPU
	class Model
	{
	public:

		struct Vertex 
		{
			glm::vec2 position;


			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};


		Model(Device &device, const std::vector<Vertex> &vertices);
		~Model();

		// since memory is not allocated automatically, copy and assign constructor should be deleted
		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);


	private:
		void createVertexBuffers(const std::vector<Vertex> &vertices);

		Device &device;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};



}