
#pragma once


namespace jvk
{
	void create_sync_objects()
	{
	    image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	    render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	    in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

	    vk::SemaphoreCreateInfo semaphore_info{};
	    semaphore_info.sType = vk::StructureType::eSemaphoreCreateInfo;

		vk::FenceCreateInfo fence_info{};
		fence_info.sType = vk::StructureType::eFenceCreateInfo;
		fence_info.flags = vk::FenceCreateFlagBits::eSignaled;


		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			image_available_semaphores[i] = logical_device.createSemaphore(semaphore_info);
			render_finished_semaphores[i] = logical_device.createSemaphore(semaphore_info);
			in_flight_fences[i] = logical_device.createFence(fence_info);
		}
	}
}