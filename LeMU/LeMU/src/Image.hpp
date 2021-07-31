#pragma once


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Device.hpp"


namespace LeMU
{
	class Image 
	{
	public:
		void loadImage();

		void createImage(
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties);


	private:

		Device &device;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;

		int width, height;
	};




}