#pragma once

#include "Device.hpp"


namespace LeMU
{
	class Image 
	{
	public:
		Image(const std::string& textureName, Device &device);

		
	private:

		void createImage(
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties);

		void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		void copyBufferToImage(VkBuffer buffer);

		void loadToStagingBuffer(const std::string& textureName);

		Device &device;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;

		

		int width, height;
	};




}