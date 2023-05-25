
#pragma once

#include <iostream>
#include <fstream>

#include <jvk/vertex.h>



namespace jvk
{
	struct SceneData
	{
	    alignas(16) glm::mat4 viewMatrix;
	    alignas(16) glm::mat4 projectionMatrix;
	};


	struct MeshData
	{
		glm::mat4 transformMatrix;
	};



	const std::string vertex_shader = "res/shaders/vert.spv";
	const std::string fragment_shader = "res/shaders/frag.spv";
	
	static std::vector<char> read_file(const std::string &filename)
	{
	    std::ifstream file(filename, std::ios::ate | std::ios::binary);

	    if (!file.is_open())
	    {
	        throw std::runtime_error("failed to open file!");
	    }

		size_t file_size = (size_t) file.tellg();
		std::vector<char> buffer(file_size);

		file.seekg(0);
		file.read(buffer.data(), file_size);

		file.close();



		return buffer;
	}


	vk::ShaderModule create_shader_module(const std::vector<char>& code)
	{
		vk::ShaderModuleCreateInfo create_info{};
		create_info.sType = vk::StructureType::eShaderModuleCreateInfo;
		create_info.codeSize = code.size();
		create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

		vk::ShaderModule shader_module;
		try
		{
			shader_module = logical_device.createShaderModule(create_info);
			std::cout << "\n\n SHADER MODULE SUCCESSFULLY \n\n";
		}
		catch (vk::SystemError err)
		{
			throw std::runtime_error("failed to create Shader Module!");
		}



		return shader_module;
	}







    static vk::VertexInputBindingDescription get_binding_description()
    {
        vk::VertexInputBindingDescription binding_description{};
        binding_description.binding = 0;
        binding_description.stride = sizeof(Vertex);
        binding_description.inputRate = vk::VertexInputRate::eVertex;


        return binding_description;
    }


    static std::array<vk::VertexInputAttributeDescription, 2> get_attribute_descriptions()
    {
        std::array<vk::VertexInputAttributeDescription, 2> attribute_descriptions{};
        attribute_descriptions[0].binding = 0;
        attribute_descriptions[0].location = 0;
        attribute_descriptions[0].format = vk::Format::eR32G32B32Sfloat;
        attribute_descriptions[0].offset = offsetof(Vertex, position);

        attribute_descriptions[1].binding = 0;
        attribute_descriptions[1].location = 1;
        attribute_descriptions[1].format = vk::Format::eR32G32B32Sfloat;
        attribute_descriptions[1].offset = offsetof(Vertex, color);


        return attribute_descriptions;
    }





	void create_descriptorset_layout()
	{
        vk::DescriptorSetLayoutBinding ubo_layout_binding{};
        ubo_layout_binding.binding = 0;
        ubo_layout_binding.descriptorCount = 1;
        ubo_layout_binding.descriptorType = vk::DescriptorType::eUniformBuffer;
        ubo_layout_binding.pImmutableSamplers = nullptr;
        ubo_layout_binding.stageFlags = vk::ShaderStageFlagBits::eVertex;


		vk::DescriptorSetLayoutCreateInfo layout_info{};
		layout_info.sType = vk::StructureType::eDescriptorSetLayoutCreateInfo;
		layout_info.bindingCount = 1;
		layout_info.pBindings = &ubo_layout_binding;

		try
		{
			descriptorset_layout = logical_device.createDescriptorSetLayout(layout_info);
		}
		catch(vk::SystemError err)
		{
		    throw std::runtime_error("failed to create descriptor set layout!");
		}
	}


    void create_descriptor_pool()
    {
        vk::DescriptorPoolSize pool_size{};
        pool_size.type = vk::DescriptorType::eUniformBuffer;
        pool_size.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        vk::DescriptorPoolCreateInfo pool_info{};
        pool_info.sType = vk::StructureType::eDescriptorPoolCreateInfo;
        pool_info.poolSizeCount = 1;
        pool_info.pPoolSizes = &pool_size;
        pool_info.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);



		descriptor_pool = logical_device.createDescriptorPool(pool_info);
            // throw std::runtime_error("failed to create descriptor pool!");
    }

    void create_descriptor_sets()
    {
        std::vector<vk::DescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorset_layout);
        vk::DescriptorSetAllocateInfo alloc_info{};
        alloc_info.sType = vk::StructureType::eDescriptorSetAllocateInfo;
        alloc_info.descriptorPool = descriptor_pool;
        alloc_info.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        alloc_info.pSetLayouts = layouts.data();

        descriptor_sets.resize(MAX_FRAMES_IN_FLIGHT);

    	if (logical_device.allocateDescriptorSets(&alloc_info, descriptor_sets.data()) != vk::Result::eSuccess)
        {
	        throw std::runtime_error("failed to allocate descriptor sets!");
		}

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vk::DescriptorBufferInfo buffer_info{};
            buffer_info.buffer = uniform_buffers[i];
            buffer_info.offset = 0;
            buffer_info.range = sizeof(SceneData);

            vk::WriteDescriptorSet descriptor_write{};
            descriptor_write.sType = vk::StructureType::eWriteDescriptorSet;
            descriptor_write.dstSet = descriptor_sets[i];
            descriptor_write.dstBinding = 0;
            descriptor_write.dstArrayElement = 0;
            descriptor_write.descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptor_write.descriptorCount = 1;
            descriptor_write.pBufferInfo = &buffer_info;

            logical_device.updateDescriptorSets(1, &descriptor_write, 0, nullptr);
        }
    }







	void create_renderpass()
	{
		vk::AttachmentDescription color_attachment{};
		color_attachment.format = swapchain_image_format;
		color_attachment.samples = vk::SampleCountFlagBits::e1;
		color_attachment.loadOp = vk::AttachmentLoadOp::eClear;
		color_attachment.storeOp = vk::AttachmentStoreOp::eStore;
		color_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		color_attachment.initialLayout = vk::ImageLayout::eUndefined;
		color_attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

		vk::AttachmentReference color_attachment_ref{};
		color_attachment_ref.attachment = 0;
		color_attachment_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;


		vk::AttachmentDescription depth_attachment{};
		depth_attachment.format = vk::Format::eD32Sfloat;
		depth_attachment.samples = vk::SampleCountFlagBits::e1;
		depth_attachment.loadOp = vk::AttachmentLoadOp::eClear;
		depth_attachment.storeOp = vk::AttachmentStoreOp::eStore;
		depth_attachment.stencilLoadOp = vk::AttachmentLoadOp::eClear;
		depth_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		depth_attachment.initialLayout = vk::ImageLayout::eUndefined;
		depth_attachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

		vk::AttachmentReference depth_attachment_ref{};
		depth_attachment_ref.attachment = 1;
		depth_attachment_ref.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;



		vk::SubpassDescription subpass{};
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;
        subpass.pDepthStencilAttachment = &depth_attachment_ref;


        vk::SubpassDependency color_dependency{};
        color_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        color_dependency.dstSubpass = 0;
        color_dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        color_dependency.srcAccessMask = {};
        color_dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        color_dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

        vk::SubpassDependency depth_dependency{};
        depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        depth_dependency.dstSubpass = 0;
        depth_dependency.srcStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
        depth_dependency.srcAccessMask = {};
        depth_dependency.dstStageMask = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
        depth_dependency.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		//array of 2 attachments, one for the color, and other for depth
		vk::AttachmentDescription attachments[2] = { color_attachment, depth_attachment };
		vk::SubpassDependency dependencies[2] = { color_dependency, depth_dependency };


		vk::RenderPassCreateInfo renderpass_info{};
		renderpass_info.sType = vk::StructureType::eRenderPassCreateInfo;
		renderpass_info.attachmentCount = 2;
		renderpass_info.pAttachments = &attachments[0];
		renderpass_info.subpassCount = 1;
		renderpass_info.pSubpasses = &subpass;
		renderpass_info.dependencyCount = 2;
		renderpass_info.pDependencies = &dependencies[0];


		try
		{
			render_pass = logical_device.createRenderPass(renderpass_info);
		}
		catch(vk::SystemError err)
		{
			throw std::runtime_error("failed to create render pass!");
		}
	}





	void create_graphics_pipeline()
	{
		auto vert_shader_code = read_file(vertex_shader);
		auto frag_shader_code = read_file(fragment_shader);

		vk::ShaderModule vert_shader_module = create_shader_module(vert_shader_code);
		vk::ShaderModule frag_shader_module = create_shader_module(frag_shader_code);

		vk::PipelineShaderStageCreateInfo vert_shader_stage_info{};
		vert_shader_stage_info.sType = vk::StructureType::ePipelineShaderStageCreateInfo;
		vert_shader_stage_info.stage = vk::ShaderStageFlagBits::eVertex;
		vert_shader_stage_info.module = vert_shader_module;
		vert_shader_stage_info.pName = "main";

		vk::PipelineShaderStageCreateInfo frag_shader_stage_info{};
		frag_shader_stage_info.sType = vk::StructureType::ePipelineShaderStageCreateInfo;
		frag_shader_stage_info.stage = vk::ShaderStageFlagBits::eFragment;
		frag_shader_stage_info.module = frag_shader_module;
		frag_shader_stage_info.pName = "main";

		vk::PipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_info, frag_shader_stage_info };



		std::vector<vk::DynamicState> dynamic_states =
		{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};

		vk::PipelineDynamicStateCreateInfo dynamic_state{};
		dynamic_state.sType = vk::StructureType::ePipelineDynamicStateCreateInfo;
		dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
		dynamic_state.pDynamicStates = dynamic_states.data();

		auto binding_description = get_binding_description();
		auto attribute_descriptions = get_attribute_descriptions();

		vk::PipelineVertexInputStateCreateInfo vertex_input_info{};
		vertex_input_info.sType = vk::StructureType::ePipelineVertexInputStateCreateInfo;
		vertex_input_info.vertexBindingDescriptionCount = 1;
		vertex_input_info.pVertexBindingDescriptions = &binding_description; // Optional
		vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
		vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data(); // Optional

		vk::PipelineInputAssemblyStateCreateInfo input_assembly{};
		input_assembly.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;
		input_assembly.topology = vk::PrimitiveTopology::eTriangleList;
		input_assembly.primitiveRestartEnable = VK_FALSE;



		vk::PipelineViewportStateCreateInfo viewport_state{};
		viewport_state.sType = vk::StructureType::ePipelineViewportStateCreateInfo;
		viewport_state.viewportCount = 1;
		// viewport_state.pViewports = &viewport;
		viewport_state.scissorCount = 1;
		// viewport_state.pScissors = &scissor;

		vk::PipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = vk::StructureType::ePipelineRasterizationStateCreateInfo;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = vk::PolygonMode::eFill;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = vk::CullModeFlagBits::eNone;
		rasterizer.frontFace = vk::FrontFace::eClockwise;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional


		vk::PipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = vk::StructureType::ePipelineMultisampleStateCreateInfo;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		vk::PipelineColorBlendAttachmentState color_blend_attachment{};
		color_blend_attachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		color_blend_attachment.blendEnable = VK_FALSE;
		color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eOne; // Optional
		color_blend_attachment.dstColorBlendFactor = vk::BlendFactor::eZero; // Optional
		color_blend_attachment.colorBlendOp = vk::BlendOp::eAdd; // Optional
		color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne; // Optional
		color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero; // Optional
		color_blend_attachment.alphaBlendOp = vk::BlendOp::eAdd; // Optional


		color_blend_attachment.blendEnable = VK_TRUE;
		color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
		color_blend_attachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
		color_blend_attachment.colorBlendOp = vk::BlendOp::eAdd;
		color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
		color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
		color_blend_attachment.alphaBlendOp = vk::BlendOp::eAdd;

		vk::PipelineColorBlendStateCreateInfo color_blending{};
		color_blending.sType = vk::StructureType::ePipelineColorBlendStateCreateInfo;
		color_blending.logicOpEnable = VK_FALSE;
		color_blending.logicOp = vk::LogicOp::eCopy; // Optional
		color_blending.attachmentCount = 1;
		color_blending.pAttachments = &color_blend_attachment;
		color_blending.blendConstants[0] = 0.0f; // Optional
		color_blending.blendConstants[1] = 0.0f; // Optional
		color_blending.blendConstants[2] = 0.0f; // Optional
		color_blending.blendConstants[3] = 0.0f; // Optional


		vk::PipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = vk::StructureType::ePipelineLayoutCreateInfo;
		pipeline_layout_info.setLayoutCount = 1; // Optional
		pipeline_layout_info.pSetLayouts = &descriptorset_layout; // Optional
		pipeline_layout_info.pushConstantRangeCount = 1; // Optional

		vk::PushConstantRange push_constant_info;
		push_constant_info.offset = 0;
		push_constant_info.size = sizeof(MeshData);
		push_constant_info.stageFlags = vk::ShaderStageFlagBits::eVertex;

		pipeline_layout_info.pPushConstantRanges = &push_constant_info; // Optional


		// Create Pipeline Layout
		try
		{
			pipeline_layout = logical_device.createPipelineLayout(pipeline_layout_info);
		}
		catch(vk::SystemError err)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}


		// DEPTH STENCIL INFO
        vk::PipelineDepthStencilStateCreateInfo depth_stencil_info = {};
        depth_stencil_info.sType = vk::StructureType::ePipelineDepthStencilStateCreateInfo;
        depth_stencil_info.pNext = nullptr;
        depth_stencil_info.depthTestEnable = VK_TRUE;
        depth_stencil_info.depthWriteEnable = VK_TRUE;
        depth_stencil_info.depthCompareOp = vk::CompareOp::eLessOrEqual;
        depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
        depth_stencil_info.minDepthBounds = 0.0f; // Optional
        depth_stencil_info.maxDepthBounds = 1.0f; // Optional
        depth_stencil_info.stencilTestEnable = VK_FALSE;



		// Graphics Pipeline Info
		vk::GraphicsPipelineCreateInfo pipeline_info{};
		pipeline_info.sType = vk::StructureType::eGraphicsPipelineCreateInfo;
		pipeline_info.stageCount = 2;
		pipeline_info.pStages = shader_stages;
		pipeline_info.pVertexInputState = &vertex_input_info;
		pipeline_info.pInputAssemblyState = &input_assembly;
		pipeline_info.pViewportState = &viewport_state;
		pipeline_info.pRasterizationState = &rasterizer;
		pipeline_info.pMultisampleState = &multisampling;
		pipeline_info.pDepthStencilState = &depth_stencil_info;
		pipeline_info.pColorBlendState = &color_blending;
		pipeline_info.pDynamicState = &dynamic_state;
		pipeline_info.layout = pipeline_layout;
		pipeline_info.renderPass = render_pass;
		pipeline_info.subpass = 0;
		pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipeline_info.basePipelineIndex = -1; // Optional



		// Create The Graphics Pipeline
		try
		{
			pipeline = logical_device.createGraphicsPipeline(nullptr, pipeline_info).value;
		}
		catch(vk::SystemError err)
		{
		    throw std::runtime_error("failed to create graphics pipeline!");
		}



		// Destroy Shader Modules
		logical_device.destroyShaderModule(frag_shader_module);
		logical_device.destroyShaderModule(vert_shader_module);
	}
}