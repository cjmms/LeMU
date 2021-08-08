
#include "Descriptor.hpp"
#include <stdexcept>

namespace LeMU
{
	Descriptor::Descriptor(Device& d)
		: device(d)
	{}


	Descriptor::~Descriptor()
	{
		vkDestroyDescriptorSetLayout(device.device(), descriptorSetLayout, nullptr);
	}


	void Descriptor::createDescriptorSetLayout()
	{
		// specify descriptor set layout binding
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;	// only 1 uniform buffer right now
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;	// only pass to vertex shader
		uboLayoutBinding.pImmutableSamplers = nullptr;	// not using any sampler to sample images

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		// create layout
		if (vkCreateDescriptorSetLayout(device.device(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor set layout!");
	}




}

