
#pragma once

#include <iostream>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp




namespace jvk
{
	struct SwapChainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> present_modes;
	};






	SwapChainSupportDetails query_swap_chain_support(vk::PhysicalDevice &physical_device)
	{
		SwapChainSupportDetails details;


	    if (physical_device.getSurfaceCapabilitiesKHR(surface, &details.capabilities) != vk::Result::eSuccess)
		{
			std::cout << "Failed to get Surface Capabilities!\n";
		}

	    uint32_t format_count;
		if (physical_device.getSurfaceFormatsKHR(surface, &format_count, nullptr) != vk::Result::eSuccess)
		{
			std::cout << "Failed to get Surface Formats!\n";
		}


		if (format_count != 0)
		{
		    details.formats.resize(format_count);
		    if (physical_device.getSurfaceFormatsKHR(surface, &format_count, details.formats.data()) != vk::Result::eSuccess)
			{
				std::cout << "Failed to get Surface Formats!\n";
			}
		}

		uint32_t present_mode_count;
		if (physical_device.getSurfacePresentModesKHR(surface, &present_mode_count, nullptr) != vk::Result::eSuccess)
		{
			std::cout << "Failed to get Surface Present Modes!\n";
		}

		if (present_mode_count != 0)
		{
		    details.present_modes.resize(present_mode_count);
		    if (physical_device.getSurfacePresentModesKHR(surface, &present_mode_count, details.present_modes.data()) != vk::Result::eSuccess)
		    {
				std::cout << "Failed to get Surface Present Modes!\n";
		    }
		}


	    return details;
	}





	vk::SurfaceFormatKHR choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR> &available_formats)
	{
		for (const auto& available_format : available_formats)
		{
		    if (available_format.format == vk::Format::eB8G8R8A8Srgb && available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		    {
		        return available_format;
		    }
		}



		return available_formats[0];
	}



	vk::PresentModeKHR choose_swap_present_mode(const std::vector<vk::PresentModeKHR> &available_present_modes)
	{
		for (const auto& available_present_mode : available_present_modes)
		{
	        if (available_present_mode == vk::PresentModeKHR::eMailbox)
	        {
	            return available_present_mode;
	        }
		}



	    return vk::PresentModeKHR::eFifo;
	}



	vk::Extent2D choose_swap_extent(const vk::SurfaceCapabilitiesKHR &capabilities, Window *window)
	{
	    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	    {
	        std::cout << "FRAMEBUFFER:\t" << capabilities.currentExtent.width << ", " << capabilities.currentExtent.height << std::endl;
	        return capabilities.currentExtent;
	    }
	    else
	    {
	        int width, height;
	        window->get_framebuffer_size(width, height);
	        std::cout << "FRAMEBUFFER:\t" << width << ", " << height << std::endl;

	        vk::Extent2D actual_extent =
	        {
	            static_cast<uint32_t>(width),
	            static_cast<uint32_t>(height)
	        };

	        actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	        actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	        return actual_extent;
	    }
	}



	void create_swapchain(Window *window)
	{
		std::cout << "\ncreating Swapchain...\n";
		SwapChainSupportDetails swap_chain_support = query_swap_chain_support(physical_device);

	    vk::SurfaceFormatKHR surface_format = choose_swap_surface_format(swap_chain_support.formats);
	    vk::PresentModeKHR present_mode = choose_swap_present_mode(swap_chain_support.present_modes);
	    vk::Extent2D extent = choose_swap_extent(swap_chain_support.capabilities, window);

	    uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

		std::cout << "\n\n\nASDFASDFASDSFASDF\n" <<  "Image count" << image_count << "\n\n";

		// if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount)
		// {
		//     image_count = swap_chain_support.capabilities.maxImageCount;
		// }


		vk::SwapchainCreateInfoKHR swapchain_create_info{};
		swapchain_create_info.sType = vk::StructureType::eSwapchainCreateInfoKHR;
		swapchain_create_info.surface = surface;
		swapchain_create_info.minImageCount = image_count;
		swapchain_create_info.imageFormat = surface_format.format;
		swapchain_create_info.imageColorSpace = surface_format.colorSpace;
		swapchain_create_info.imageExtent = extent;
		swapchain_create_info.imageArrayLayers = 1;
		swapchain_create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

		uint32_t queue_family_indices[] = { indices.graphics_family.value(), indices.present_family.value() };

		if (indices.graphics_family != indices.present_family)
		{
		    swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent;
		    swapchain_create_info.queueFamilyIndexCount = 2;
		    swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
		}
		else
		{
		    swapchain_create_info.imageSharingMode = vk::SharingMode::eExclusive;
		    // swapchain_create_info.queueFamilyIndexCount = 0; // Optional
		    // swapchain_create_info.pQueueFamilyIndices = queue_family_indices; // Optional
		}

		swapchain_create_info.preTransform = swap_chain_support.capabilities.currentTransform;
		swapchain_create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		swapchain_create_info.presentMode = present_mode;
		swapchain_create_info.clipped = VK_TRUE;
		swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;



		try
		{
			swap_chain = logical_device.createSwapchainKHR(swapchain_create_info);
			std::cout << "\n\n SWAPCHAIN CREATED SUCCESSFULLY \n\n";
		}
		catch (vk::SystemError err)
		{
			throw std::runtime_error("failed to create Swap chain!");
		}

		// Set Swapchain images
		swapchain_images = logical_device.getSwapchainImagesKHR(swap_chain);
		swapchain_images.resize(swapchain_images.size());

		// Set Surface Format and Extent
		swapchain_image_format = surface_format.format;
		swapchain_extent = extent;
	}


	void create_imageviews()
	{
		swapchain_imageviews.resize(swapchain_images.size());

		for (size_t i = 0; i < swapchain_images.size(); i++)
		{
			vk::ImageViewCreateInfo create_info{};
			create_info.sType = vk::StructureType::eImageViewCreateInfo;
			create_info.image = swapchain_images[i];

			create_info.viewType = vk::ImageViewType::e2D;
			create_info.format = swapchain_image_format;

			create_info.components.r = vk::ComponentSwizzle::eIdentity;
			create_info.components.g = vk::ComponentSwizzle::eIdentity;
			create_info.components.b = vk::ComponentSwizzle::eIdentity;
			create_info.components.a = vk::ComponentSwizzle::eIdentity;

			create_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			create_info.subresourceRange.baseMipLevel = 0;
			create_info.subresourceRange.levelCount = 1;
			create_info.subresourceRange.baseArrayLayer = 0;
			create_info.subresourceRange.layerCount = 1;

			try
			{
				swapchain_imageviews[i] = logical_device.createImageView(create_info);
			}
			catch (vk::SystemError)
			{
				throw std::runtime_error("Failed to create Image View!");
			}
		}
	}







	uint32_t find_memory_type(uint32_t type_filter, vk::MemoryPropertyFlags properties)
	{
		vk::PhysicalDeviceMemoryProperties mem_properties;
		physical_device.getMemoryProperties(&mem_properties);

		for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
		{
		    if (type_filter & (1 << i) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
		    {
		        return i;
		    }
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}





    void create_depth_resources()
    {
        // CREATE IMAGE
        vk::ImageCreateInfo image_info = { };
        image_info.sType = vk::StructureType::eImageCreateInfo;
        image_info.pNext = nullptr;
        image_info.imageType = vk::ImageType::e2D;
        image_info.format = vk::Format::eD32Sfloat;
        image_info.extent = vk::Extent3D{ swapchain_extent.width, swapchain_extent.height, 1 };
        image_info.mipLevels = 1;
        image_info.arrayLayers = 1;
        image_info.samples = vk::SampleCountFlagBits::e1;
        image_info.tiling = vk::ImageTiling::eOptimal;
        image_info.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;

        depth_image = logical_device.createImage(image_info, nullptr);





        // ALLOCATE MEMORY
        vk::MemoryRequirements mem_requirements;
        logical_device.getImageMemoryRequirements(depth_image, &mem_requirements);

        vk::MemoryAllocateInfo alloc_info{};
        alloc_info.sType = vk::StructureType::eMemoryAllocateInfo;
        alloc_info.allocationSize = mem_requirements.size;
        alloc_info.memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);

        if (logical_device.allocateMemory(&alloc_info, nullptr, &depth_image_memory) != vk::Result::eSuccess)
        {
            throw std::runtime_error("failed to allocate image memory!");
        }

        logical_device.bindImageMemory(depth_image, depth_image_memory, 0);




        // CREATE IMAGEVIEW
        vk::ImageViewCreateInfo imageview_info = {};
        imageview_info.sType = vk::StructureType::eImageViewCreateInfo;
        imageview_info.pNext = nullptr;
        imageview_info.viewType = vk::ImageViewType::e2D;
        imageview_info.image = depth_image;
        imageview_info.format = vk::Format::eD32Sfloat;
        imageview_info.subresourceRange.baseMipLevel = 0;
        imageview_info.subresourceRange.levelCount = 1;
        imageview_info.subresourceRange.baseArrayLayer = 0;
        imageview_info.subresourceRange.layerCount = 1;
        imageview_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

        depth_imageview = logical_device.createImageView(imageview_info);
    }





	void create_framebuffers()
	{
		swapchain_framebuffers.resize(swapchain_imageviews.size());


		for (size_t i = 0; i < swapchain_imageviews.size(); i++)
		{
		    vk::ImageView attachments[2];
		    attachments[0] = swapchain_imageviews[i];
		    attachments[1] = depth_imageview;

		    vk::FramebufferCreateInfo framebuffer_info{};
		    framebuffer_info.sType = vk::StructureType::eFramebufferCreateInfo;
		    framebuffer_info.renderPass = render_pass;
		    framebuffer_info.attachmentCount = 2;
		    framebuffer_info.pAttachments = attachments;
		    framebuffer_info.width = swapchain_extent.width;
		    framebuffer_info.height = swapchain_extent.height;
		    framebuffer_info.layers = 1;

		    try
		    {
		    	swapchain_framebuffers[i] = logical_device.createFramebuffer(framebuffer_info);
		    }
		    catch(vk::SystemError err)
		    {
		        throw std::runtime_error("Failed to create framebuffer!");
		    }
		}
	}
	



	void cleanup_swapchain()
	{
		logical_device.destroyImageView(depth_imageview);
		logical_device.destroyImage(depth_image);
		logical_device.freeMemory(depth_image_memory);
		
		for (auto frame_buffer : swapchain_framebuffers) { logical_device.destroyFramebuffer(frame_buffer); }

	    for (auto image_view : swapchain_imageviews) { logical_device.destroyImageView(image_view); }

		logical_device.destroySwapchainKHR(swap_chain);
	}




	void recreate_swapchain(Window *window)
	{
		logical_device.waitIdle();

		cleanup_swapchain();

	    create_swapchain(window);
	    create_imageviews();
	    create_depth_resources();
	    create_framebuffers();
	}
}