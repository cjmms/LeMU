#pragma once

#include "Device.hpp"


namespace LeMU
{
	class Image 
	{
	public:
		Image(const std::string& textureName, Device &device);
		~Image();
		

		

	private:

		void createTextureImageView();

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

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		VkImageView textureImageView;

		int width, height;
	};


	VkImageView createImageView(VkImage image, VkFormat format, const VkDevice& device);

}