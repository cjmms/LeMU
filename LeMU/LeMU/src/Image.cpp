
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

#include "Image.hpp"
#include <stdexcept>
#include <string>

namespace LeMU
{
	Image::Image(const std::string& textureName, Device& device)
		:device(device)
	{
		loadToStagingBuffer(textureName);
		
		createImage(VK_FORMAT_R8G8B8A8_SRGB,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	
		transitionImageLayout(	VK_FORMAT_R8G8B8A8_SRGB,				// format 
								VK_IMAGE_LAYOUT_UNDEFINED,				// old layout
								VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);	// new layout
	
		copyBufferToImage(stagingBuffer);

		transitionImageLayout(	VK_FORMAT_R8G8B8A8_SRGB, 
								VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
								VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);	// easy for shader to access
								
		vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
		vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
	}



	Image::~Image()
	{
		vkDestroySampler(device.device(), textureSampler, nullptr);
		vkDestroyImageView(device.device(), textureImageView, nullptr); 

		vkDestroyImageView(device.device(), textureImageView, nullptr);

		vkDestroyImage(device.device(), textureImage, nullptr);
		vkFreeMemory(device.device(), textureImageMemory, nullptr);
	}




	void Image::loadToStagingBuffer(const std::string& textureName)
	{
		int texChannels;

		stbi_uc* pixels = stbi_load("textures/texture.jpg", &width, &height, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = width * height * 4;

		if (!pixels) 
			throw std::runtime_error("failed to load texture image! " + std::string(" ") + stbi_failure_reason());
		
		device.createBuffer(imageSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory);

		void* data;
		if (vkMapMemory(device.device(), stagingBufferMemory, 0, imageSize, 0, &data) != VK_SUCCESS)
			throw std::runtime_error("failed to load image to staging buffer");
		memcpy(data, pixels,  imageSize);
		vkUnmapMemory(device.device(), stagingBufferMemory);

		stbi_image_free(pixels);
	}




	void Image::createImage(
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(width);
		imageInfo.extent.height = static_cast<uint32_t>(height);
		imageInfo.extent.depth = 1;		// it won't be 1 if this image is a 3D resource
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		if (vkCreateImage(device.device(), &imageInfo, nullptr, &textureImage) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device.device(), textureImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		
		allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(device.device(), &allocInfo, nullptr, &textureImageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(device.device(), textureImage, textureImageMemory, 0);
	}


	
	void Image::transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = device.beginSingleTimeCommands();

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		// using image memory barrier to perform layout transition
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		// we are not using image memory barrier to transfer queue family ownership
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.image = textureImage;
		// specify the specific part of the image that get affected
		// our image is not an array and it does not have mipmap level
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		// Undefined -> transfer destination
		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;							
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}

		// Transfer destination -> shader reading
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage,	// which pipeline stage should happen before barrier
			destinationStage,	// which pipeline stage should wait on barrier
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		device.endSingleTimeCommands(commandBuffer);
	}



	void Image::copyBufferToImage(VkBuffer buffer)
	{
		VkCommandBuffer commandBuffer = device.beginSingleTimeCommands();

		// specify which part of buffer will get copied to which part of the image
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height),
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			textureImage,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		device.endSingleTimeCommands(commandBuffer);
	}


	void Image::createTextureImageView()
	{
		textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, device.device());
	}


	void Image::createTextureSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

		// bilinear filtering for both oersampling and undersampling
		samplerInfo.magFilter = VK_FILTER_LINEAR;	
		samplerInfo.minFilter = VK_FILTER_LINEAR;

		// Repeat the texture when going beyond the image dimensions.
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

		// Enable anisotropic filtering
		samplerInfo.anisotropyEnable = VK_TRUE;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(device.getPhysicalDevice(), &properties);
		// boost to max base on physical device
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

		// pick outside the image, and clamping, returns black
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

		// always false, otherwise UV will be in the range of [0, width]
		samplerInfo.unnormalizedCoordinates = VK_FALSE;

		// defines how to compare texels, used for PCF
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(device.device(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}
	}



	VkImageView createImageView(VkImage image, VkFormat format, const VkDevice& device)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;

		if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
			throw std::runtime_error("failed to create texture image view!");

		return imageView;
	}
}

