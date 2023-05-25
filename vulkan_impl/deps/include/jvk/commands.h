
#pragma once

#include <iostream>


namespace jvk
{
	void create_command_pool()
	{
		vk::CommandPoolCreateInfo pool_info{};
		pool_info.sType = vk::StructureType::eCommandPoolCreateInfo;
		pool_info.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
		pool_info.queueFamilyIndex = indices.graphics_family.value();


		try
		{
			command_pool = logical_device.createCommandPool(pool_info);
		}
		catch(vk::SystemError err)
		{
			throw std::runtime_error("failed to create command pool!");
		}
	}




	void create_command_buffers()
	{
		command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

		vk::CommandBufferAllocateInfo alloc_info{};
		alloc_info.sType = vk::StructureType::eCommandBufferAllocateInfo;
		alloc_info.commandPool = command_pool;
		alloc_info.level = vk::CommandBufferLevel::ePrimary;
		alloc_info.commandBufferCount = (uint32_t) command_buffers.size();

		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			try
			{
				command_buffers[i] = logical_device.allocateCommandBuffers(alloc_info)[0];
			}
			catch(vk::SystemError err)
			{
				throw std::runtime_error("failed to create command buffer!");
			}
		}
	}

	void record_command_buffer(uint32_t &image_index, vk::CommandBuffer &command_buffer)
	{
		vk::CommandBufferBeginInfo begin_info{};
		begin_info.sType = vk::StructureType::eCommandBufferBeginInfo;
		// begin_info.flags = 0; // Optional
		begin_info.pInheritanceInfo = nullptr; // Optional

		command_buffer.begin(begin_info);




		vk::RenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = vk::StructureType::eRenderPassBeginInfo;
		render_pass_info.renderPass = render_pass;
		render_pass_info.framebuffer = swapchain_framebuffers[image_index];
		render_pass_info.renderArea.offset.x = 0;
		render_pass_info.renderArea.offset.y = 0;
		render_pass_info.renderArea.extent = swapchain_extent;

        std::array<vk::ClearValue, 2> clear_values{};
        clear_values[0].color = { std::array<float, 4>{0.0f, 0.5f, 0.5f, 1.0f} };
        clear_values[1].depthStencil = {{1.0f, 0}};

        render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
        render_pass_info.pClearValues = clear_values.data();

		vk::Viewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)swapchain_extent.width;
		viewport.height = (float)swapchain_extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		command_buffer.setViewport(0, 1, &viewport);

		vk::Rect2D scissor{};
		scissor.offset.x = 0.0f;
		scissor.offset.y = 0.0f;
		scissor.extent = swapchain_extent;
		command_buffer.setScissor(0, 1, &scissor);


        vk::Buffer vertex_buffers[] = {vertex_buffer};
        vk::DeviceSize offsets[] = {0};
		command_buffer.beginRenderPass(&render_pass_info, vk::SubpassContents::eInline);
		command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
        command_buffer.bindVertexBuffers(0, 1, vertex_buffers, offsets);
        command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline_layout, 0, 1, &descriptor_sets[current_frame], 0, nullptr);


        for (glm::mat4 transform : mesh_transforms)
        {
        	MeshData mesh;
        	mesh.transformMatrix = transform;
			command_buffer.pushConstants(pipeline_layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(MeshData), &mesh);
			command_buffer.draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);
		}




		command_buffer.endRenderPass();

		command_buffer.end();
	}






	vk::CommandBuffer begin_single_time_commands()
    {
        vk::CommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = vk::StructureType::eCommandBufferAllocateInfo;
        alloc_info.level = vk::CommandBufferLevel::ePrimary;
        alloc_info.commandPool = command_pool;
        alloc_info.commandBufferCount = 1;

        vk::CommandBuffer command_buffer;
        command_buffer = logical_device.allocateCommandBuffers(alloc_info)[0];

        vk::CommandBufferBeginInfo begin_info{};
        begin_info.sType = vk::StructureType::eCommandBufferBeginInfo;
        begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        command_buffer.begin(begin_info);


        return command_buffer;
    }

    void end_single_time_commands(vk::CommandBuffer command_buffer)
    {
		command_buffer.end();

	    vk::SubmitInfo submit_info{};
	    submit_info.sType = vk::StructureType::eSubmitInfo;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer;

	    graphics_queue.submit(submit_info, VK_NULL_HANDLE);
	    graphics_queue.waitIdle();

	    logical_device.freeCommandBuffers(command_pool, 1, &command_buffer);
    }
}