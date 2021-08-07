#include "Model.hpp"

#include <cassert>
#include <cstring>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"

namespace LeMU
{
	Model::Model(Device& device, const Builder& builder)
		: device {device}
	{
		createVertexBuffer(builder.vertices);
		createIndexBuffer(builder.indices);
	}


	Model::~Model()
	{
		deleteBuffer(vertexBuffer, vertexBufferMemory);

		if (hasIndexBuffer)	deleteBuffer(indexBuffer, indexBufferMemory);
	}

	
	void Model::createVertexBuffer(const std::vector<Vertex> &vertices)
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

		copyHostMemToDeviceMem(stagingBufferMemory, bufferSize, vertices.data());

		// create vertex buffer, transfer data from staging buffer to vertex buffer
		device.createBuffer(
			bufferSize,
			// not only a vertex buffer, also a transfer destination buffer(transfer from staging buffer)
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			// Local buffer inside device, not visiable and not coherent to host buffer
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,	
			vertexBuffer,
			vertexBufferMemory);

		device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

		deleteBuffer(stagingBuffer, stagingBufferMemory);
	}


	void Model::createIndexBuffer(const std::vector<uint32_t>& indices)
	{
		indexCount = static_cast<uint32_t>(indices.size());
		hasIndexBuffer = indexCount > 0;
		
		if (!hasIndexBuffer) return;

		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,	
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory);

		copyHostMemToDeviceMem(stagingBufferMemory, bufferSize, indices.data());

		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer,
			indexBufferMemory);

		device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

		deleteBuffer(stagingBuffer, stagingBufferMemory);
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


	void Model::deleteBuffer(VkBuffer buffer, VkDeviceMemory deviceMemory)
	{
		vkDestroyBuffer(device.device(), buffer, nullptr);
		vkFreeMemory(device.device(), deviceMemory, nullptr);
	}


	void Model::copyHostMemToDeviceMem(VkDeviceMemory deviceMemory, VkDeviceSize memSize, const void* source)
	{
		void* data;

		// map memory on host to device memory
		vkMapMemory(device.device(), deviceMemory, 0, memSize, 0, &data);

		// write index buffer data into host(void *data)
		// meanwhile, these data will be flushed to device memory
		memcpy(data, source, static_cast<size_t>(memSize));

		// once all data has been writtern, 
		// no longer need the  region of data in host side,
		// unmap the indexBufferMemory
		// region on the host side will be deleted
		vkUnmapMemory(device.device(), deviceMemory);
	}




	std::unique_ptr<Model> Model::createModelFromFile(Device& device, const std::string& filePath)
	{
		Builder builder{};

		std::cout << "Start loading Model, model path: " << filePath << std::endl;

		builder.loadModel(filePath);

		std::cout << "Vertex count: " << builder.vertices.size() << std::endl;

		return std::make_unique<Model>(device, builder);
	}


	// this function is based on model loader file: tiny_obj_loader.h
	void Model::Builder::loadModel(const std::string& filePath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str()))
		{
			throw std::runtime_error(warn + err);
		}

		vertices.clear();
		indices.clear();

		for (const auto &shape: shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				// vertex position
				if (index.vertex_index >= 0)
				{
					vertex.position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};
				}

				// vertex normal
				if (index.normal_index >= 0)
				{
					vertex.normal = {
						attrib.vertices[3 * index.normal_index + 0],
						attrib.vertices[3 * index.normal_index + 1],
						attrib.vertices[3 * index.normal_index + 2]
					};
				}

				// vertex uv
				if (index.texcoord_index >= 0)
				{
					vertex.uv = {
						attrib.vertices[2 * index.texcoord_index + 0],
						attrib.vertices[2 * index.texcoord_index + 1]
					};
				}

				vertices.push_back(vertex);
			}
		}
	}

}