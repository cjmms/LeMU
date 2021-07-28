#include "Model.hpp"

#include <cassert>
#include <cstring>

namespace LeMU
{

	Model::Model(Device& device, const Builder& builder)
		: device {device}
	{
		createVertexBuffers(builder.vertices);
		createIndexBuffer(builder.indices);
	}


	Model::~Model()
	{
		// destory vertex buffer
		vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
		vkFreeMemory(device.device(), vertexBufferMemory, nullptr);

		if (hasIndexBuffer)	// destory index buffer
		{
			vkDestroyBuffer(device.device(), indexBuffer, nullptr);
			vkFreeMemory(device.device(), indexBufferMemory, nullptr);
		}
	}


	void Model::createVertexBuffers(const std::vector<Vertex> &vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");

		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory );

		void* data;
		vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(device.device(), vertexBufferMemory);
	}


	void Model::createIndexBuffer(const std::vector<uint32_t>& indices)
	{
		indexCount = static_cast<uint32_t>(indices.size());
		hasIndexBuffer = indexCount > 0;
		
		if (!hasIndexBuffer) return;

		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

		// Create empty buffer inside device(GPU)
		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			indexBuffer,
			indexBufferMemory);

		// the purpose of the code below is mapping data from CPU to GPU

		void* data;	// empty buffer in host(CPU)

		// map memory on host(void *data) to device memory(indexBufferMemory)
		vkMapMemory(device.device(), indexBufferMemory, 0, bufferSize, 0, &data);

		// write index buffer data into host(void *data)
		// meanwhile, these data will be flushed to device memory
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));

		// once all data has been writtern, 
		// no longer need to region of data in host side,
		// unmap the indexBufferMemory
		// region on the host side will be deleted
		vkUnmapMemory(device.device(), indexBufferMemory);
	}



	void Model::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void Model::draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}


	std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescription()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}


	std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

		// pos
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].offset = offsetof(Vertex, position);
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;	// 3 components for position

		// color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;		// match vertex shader
		attributeDescriptions[1].offset = offsetof(Vertex, color);		// makes life a lot easier
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;

		return attributeDescriptions;
	}

}
 

