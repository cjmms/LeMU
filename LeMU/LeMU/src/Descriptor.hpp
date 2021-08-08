#pragma once

#include "Device.hpp"
#include "Model.hpp"

namespace LeMU
{
	struct UniformBufferObject
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	};


	class Descriptor 
	{
	public:
		Descriptor(Device& d);
		~Descriptor();


		void createDescriptorSetLayout();
		inline VkDescriptorSetLayout& getDescriptorSetLayout() { return descriptorSetLayout; }



	private:
		
		VkDescriptorSetLayout descriptorSetLayout;
		Device& device;
	};





}

