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

		// buffer size for both vertex buffer and staging buffer
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		device.createBuffer(
			bufferSize,	
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,	// Source location for memory transfer
			// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT: buffer is visiable to host side
			// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT: any change in host side will apply to device side
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory );

		// mapping staging buffer to host buffer
		void* data;

		// map memory on host(void *data) to device memory
		vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);

		// write index buffer data into host(void *data)
		// meanwhile, these data will be flushed to device memory
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));

		// once all data has been writtern, 
		// no longer need the  region of data in host side,
		// unmap the staging buffer
		// region on the host side will be deleted
		vkUnmapMemory(device.device(), stagingBufferMemory);

		// create vertex buffer, transfer data from staging buffer to vertex buffer
		device.createBuffer(
			bufferSize,
			// not only a vertex buffer, also a transfer destination buffer(transfer from staging buffer)
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			// Local buffer inside device, not visiable and not coherent to host buffer
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBuffer,
			vertexBufferMemory);

		// copy data from staging buffer to vertex buffer
		device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

		// clean staging buffer
		vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
		vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
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
		// no longer need the  region of data in host side,
		// unmap the indexBufferMemory
		// region on the host side will be deleted
		vkUnmapMemory(device.device(), indexBufferMemory);
	}



	void Model::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer)
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void Model::draw(VkCommandBuffer commandBuffer)
	{
		if (hasIndexBuffer)
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		else
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
 

