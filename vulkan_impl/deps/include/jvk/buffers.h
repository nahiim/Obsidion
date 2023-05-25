
#pragma once



#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>




namespace jvk
{
	void copy_buffer(vk::Buffer src_buffer, vk::Buffer dst_buffer, vk::DeviceSize size)
	{
	    vk::CommandBuffer command_buffer = begin_single_time_commands();


		vk::BufferCopy copy_region{};
		copy_region.srcOffset = 0; // Optional
		copy_region.dstOffset = 0; // Optional
		copy_region.size = size;
		command_buffer.copyBuffer(src_buffer, dst_buffer, 1, &copy_region);


		end_single_time_commands(command_buffer);
	}





	void create_buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &buffer_memory)
	{
		vk::BufferCreateInfo buffer_info{};
		buffer_info.sType = vk::StructureType::eBufferCreateInfo;
		buffer_info.size = size;
		buffer_info.usage = usage;
		buffer_info.sharingMode = vk::SharingMode::eExclusive;


		if (logical_device.createBuffer(&buffer_info, nullptr, &buffer) != vk::Result::eSuccess)
		{
			throw std::runtime_error("failed to create buffer!");
		}



		vk::MemoryRequirements mem_requirements;
		logical_device.getBufferMemoryRequirements(buffer, &mem_requirements);


		vk::MemoryAllocateInfo alloc_info{};
		alloc_info.sType = vk::StructureType::eMemoryAllocateInfo;
		alloc_info.allocationSize = mem_requirements.size;
		alloc_info.memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, properties);

		if (logical_device.allocateMemory(&alloc_info, nullptr, &buffer_memory) != vk::Result::eSuccess)
		{
			throw std::runtime_error("failed to Allocate buffer memory!");
		}


        logical_device.bindBufferMemory(buffer, buffer_memory, 0);
	}


	

	void create_vertex_buffer()
	{
		vk::DeviceSize buffer_size = sizeof(vertices[0]) * vertices.size();

		vk::Buffer staging_buffer;
		vk::DeviceMemory staging_buffer_memory;
		create_buffer(
			buffer_size,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			staging_buffer,
			staging_buffer_memory
		);


        void* data;
        if (logical_device.mapMemory(staging_buffer_memory, 0, buffer_size, {}, &data) != vk::Result::eSuccess)
        	{	std::cout << "Map Memory Failed!\n";	}

        memcpy(data, vertices.data(), (size_t) buffer_size);
        logical_device.unmapMemory(staging_buffer_memory);



		create_buffer(
			buffer_size,
			vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
			vk::MemoryPropertyFlagBits::eDeviceLocal,
			vertex_buffer,
			vertex_buffer_memory
		);


		copy_buffer(staging_buffer, vertex_buffer, buffer_size);


		logical_device.destroyBuffer(staging_buffer);
		logical_device.freeMemory(staging_buffer_memory);
	}




	void create_uniform_buffers()
	{
	    vk::DeviceSize buffer_size = sizeof(SceneData);

	    uniform_buffers.resize(MAX_FRAMES_IN_FLIGHT);
	    uniform_buffers_memory.resize(MAX_FRAMES_IN_FLIGHT);

	    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	    {
	        create_buffer(
				buffer_size,
	        	vk::BufferUsageFlagBits::eUniformBuffer,
	        	vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
	        	uniform_buffers[i],
	        	uniform_buffers_memory[i]
	        );
	    }
	}








	void update_uniform_buffer(uint32_t current_image, glm::mat4 view, glm::mat4 projection)
	{
	    static auto start_time = std::chrono::high_resolution_clock::now();

	    auto current_time = std::chrono::high_resolution_clock::now();
	    float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();


        SceneData scene{};

        scene.viewMatrix = view;
        scene.projectionMatrix = projection;
        scene.projectionMatrix[1][1] *= -1;



        void *data;
		if (logical_device.mapMemory(uniform_buffers_memory[current_image], 0, sizeof(scene), {}, &data) != vk::Result::eSuccess)
        	{	std::cout << "Map Memory Failed!\n";	}

        memcpy(data, &scene, sizeof(scene));
        logical_device.unmapMemory(uniform_buffers_memory[current_image]);
	}
}