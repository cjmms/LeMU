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
			glm::vec3 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};
		};


		Model(Device &device, const Builder& builder);
		~Model();

		// since memory is not allocated automatically, copy and assign constructor should be deleted
		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);


	private:
		// create a vertex buffer, copy data inside vertices into the buffer
		// this vertex buffer is not visiable and not coherent to the host 
		// 5 steps to create vertex buffer
		// 1. create a staging buffer (inside device, visiable and coherent to host)
		// 2. copy host data to staging buffer (Data flow: Host -> Device)
		// 3. create a vertex buffer (inside device, NOT visiable and coherent to host)
		// 4. copy data inside staging buffer to vertex buffer (Data flow: Device -> Device)
		// 5. delete staging buffer
		void createVertexBuffer(const std::vector<Vertex> &vertices);

		// similar with createVertexBuffer()
		void createIndexBuffer(const std::vector<uint32_t> &indices);

		void deleteBuffer(VkBuffer buffer, VkDeviceMemory deviceMemory);

		// copy data from host memory to device memory
		// deviceMemory: memory in device(GPU)
		// memSize: size of copied data
		// source: copied data in host side(CPU)
		void copyHostMemToDeviceMem(VkDeviceMemory deviceMemory, VkDeviceSize memSize, const void* source);

		Device &device;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		uint32_t indexCount;
	};



}