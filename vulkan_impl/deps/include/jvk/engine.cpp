

#include "engine.h"
#include "includes.h"


using namespace jvk;


// TODO: Handle Window Resize and Minimize(SwapChain REcreation)




Engine::Engine()
{
	window = new Window();
	window->get_extensions();
}



void Engine::init(std::vector<Vertex> l_vertices, std::vector<glm::mat4> matrices)
{
	for (int i = 0; i < l_vertices.size(); i++)
	{
		vertices.push_back(l_vertices[i]);
	}

	for (int i = 0; i < matrices.size(); i++)
	{
		mesh_transforms.push_back(matrices[i]);
	}



	create_instance(window);

	dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
	create_debug_messenger();

	window->create_surface(surface, instance);

	pick_physical_device();
	create_logical_device();

	create_swapchain(window);
	create_imageviews();
	create_depth_resources();

	create_descriptorset_layout();
	create_renderpass();
	create_graphics_pipeline();

	create_framebuffers();
	
	create_command_pool();
	create_command_buffers();
	create_descriptor_pool();

	create_vertex_buffer();
	create_uniform_buffers();
	create_descriptor_sets();
	
	create_sync_objects();
}


void Engine::render(std::vector<glm::mat4> mesh_data, glm::mat4 view, glm::mat4 projection)
{
	mesh_transforms = mesh_data;

	if (logical_device.waitForFences(1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX) != vk::Result::eSuccess)
		{  }


	uint32_t image_index;
	vk::Result result = logical_device.acquireNextImageKHR(swap_chain, UINT64_MAX, image_available_semaphores[current_frame], nullptr, &image_index);


	if (result == vk::Result::eTimeout)
	{
	    recreate_swapchain(window);
	    return;
	} 
	else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR )
	{
	    throw std::runtime_error("failed to acquire swap chain image!");
	}


	update_uniform_buffer(current_frame, view, projection);


	if (logical_device.resetFences(1, &in_flight_fences[current_frame]) != vk::Result::eSuccess)
		{  }


    command_buffers[current_frame].reset();
	record_command_buffer(image_index, command_buffers[current_frame]);


    vk::SubmitInfo submit_info{};
    submit_info.sType = vk::StructureType::eSubmitInfo;

    vk::Semaphore wait_semaphores[] = { image_available_semaphores[current_frame] };
    vk::PipelineStageFlags wait_stages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput};

    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffers[current_frame];

    vk::Semaphore signal_semaphores[] = { render_finished_semaphores[current_frame] };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;


    graphics_queue.submit(submit_info, in_flight_fences[current_frame]);


    vk::PresentInfoKHR present_info{};
    present_info.sType = vk::StructureType::ePresentInfoKHR;

    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    vk::SwapchainKHR swapChains[] = { swap_chain };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapChains;

    present_info.pImageIndices = &image_index;


    result = present_queue.presentKHR(present_info);
	if (result == vk::Result::eTimeout || result == vk::Result::eSuboptimalKHR || framebuffer_resized)
	{
		framebuffer_resized = false;
	    recreate_swapchain(window);
	}
	else if (result != vk::Result::eSuccess)
	{
	    throw std::runtime_error("failed to present swap chain image!");
	}



    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}



Engine::~Engine()
{
	logical_device.waitIdle();

	cleanup_swapchain();

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        logical_device.destroyBuffer(uniform_buffers[i]);
        logical_device.freeMemory(uniform_buffers_memory[i]);
    }

    logical_device.destroyDescriptorPool(descriptor_pool);
	logical_device.destroyDescriptorSetLayout(descriptorset_layout);

	logical_device.destroyPipeline(pipeline);
	logical_device.destroyPipelineLayout(pipeline_layout);
	logical_device.destroyRenderPass(render_pass);

	logical_device.destroyBuffer(vertex_buffer);
	logical_device.freeMemory(vertex_buffer_memory);


	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		logical_device.destroySemaphore(render_finished_semaphores[i]);
		logical_device.destroySemaphore(image_available_semaphores[i]);
		logical_device.destroyFence(in_flight_fences[i]);
	}

	logical_device.destroyCommandPool(command_pool);


    logical_device.destroy();

	instance.destroySurfaceKHR(surface);

	if (enable_validation_layers) { instance.destroyDebugUtilsMessengerEXT(debug_messenger, nullptr, dldi); }

	instance.destroy();



	std::cout << "ENGINE DESTROYED!!\n";


	delete window;
}