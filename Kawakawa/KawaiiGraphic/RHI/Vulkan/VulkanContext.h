#pragma once

#include <vulkan/vulkan.h>
#include "Vendor/vma/vk_mem_alloc.h"

namespace RHI
{
	class VulkanContext
	{
	public:
		void Init(const char* application_name, const char* engine_name);
		void Shutdown();

	private:
		int examinePhysicalDevice(VkPhysicalDevice physical_device) const;

	private:
		enum
		{
			MAX_COMBINED_IMAGE_SAMPLERS = 32,
			MAX_UNIFORM_BUFFERS = 32,
			MAX_DESCRIPTOR_SETS = 512,
		};

		VkInstance instance = VK_NULL_HANDLE;
		VkDevice device = VK_NULL_HANDLE;
		VkPhysicalDevice physical_device = VK_NULL_HANDLE;

		uint32_t graphics_queue_family = 0xFFFF;
		VkQueue graphics_queue = VK_NULL_HANDLE;

		VkCommandPool command_pool{ VK_NULL_HANDLE };
		VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;

		VkSampleCountFlagBits max_MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		VmaAllocator vram_allocator = VK_NULL_HANDLE;

		VkDebugUtilsMessengerEXT debug_messenger{ VK_NULL_HANDLE };
	};
}