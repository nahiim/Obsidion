
#pragma once

#include <jvk/vertex.h>

namespace jvk
{
	#ifdef NDEBUG
	    const bool enable_validation_layers = false;
	#else
	    const bool enable_validation_layers = true;
	#endif


	const std::vector<const char*> validation_layers = { "VK_LAYER_KHRONOS_validation" };
	const std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	const int MAX_FRAMES_IN_FLIGHT = 2;
	uint32_t current_frame = 0;


	//instance-related variables
	vk::Instance instance{ nullptr };
	vk::DebugUtilsMessengerEXT debug_messenger{ nullptr };
	vk::DispatchLoaderDynamic dldi;
	vk::SurfaceKHR surface;
	
	//device-related variables
	vk::PhysicalDevice physical_device = { nullptr };
	vk::Device logical_device = { nullptr };
	vk::Queue graphics_queue;
	vk::Queue present_queue;

	// Swapchain-related variables
	vk::SwapchainKHR swap_chain;
	vk::Format swapchain_image_format;
	vk::Extent2D swapchain_extent;
	std::vector<vk::ImageView> swapchain_imageviews;
	std::vector<vk::Framebuffer> swapchain_framebuffers;
	std::vector<vk::Image> swapchain_images;


	vk::RenderPass render_pass;
	vk::PipelineLayout pipeline_layout;
	vk::Pipeline pipeline;

	vk::DescriptorSetLayout descriptorset_layout;
	vk::DescriptorPool descriptor_pool;
    std::vector<vk::DescriptorSet> descriptor_sets;

	vk::CommandPool command_pool;
	std::vector<vk::CommandBuffer> command_buffers;


	std::vector<vk::Semaphore> image_available_semaphores;
	std::vector<vk::Semaphore> render_finished_semaphores;
	std::vector<vk::Fence> in_flight_fences;

	vk::Buffer vertex_buffer;
	vk::DeviceMemory vertex_buffer_memory;

	std::vector<vk::Buffer> uniform_buffers;
	std::vector<vk::DeviceMemory> uniform_buffers_memory;
	
	std::vector<Vertex> vertices;
	std::vector<glm::mat4> mesh_transforms;

	vk::Image depth_image;
	vk::DeviceMemory depth_image_memory;
	vk::ImageView depth_imageview;

	vk::Image texture_image;
    vk::DeviceMemory texture_image_memory;
    vk::ImageView texture_image_view;
    vk::Sampler texture_sampler;

	bool framebuffer_resized = false;
}