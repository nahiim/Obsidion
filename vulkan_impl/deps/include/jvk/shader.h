
#pragma once

#include <iostream>
#include <fstream>


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
}