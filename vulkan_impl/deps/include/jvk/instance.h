
#pragma once

#include <iostream>
#include <cstring>

#include "declarations.h"



namespace jvk
{
	bool check_validation_layer_support()
	{
		uint32_t layer_count;
		if (vk::enumerateInstanceLayerProperties(&layer_count, nullptr) != vk::Result::eSuccess)
		{
			std::cout << "Failed to enumerate instance layer properties!\n";
		}

		std::vector<vk::LayerProperties> available_layers(layer_count);
		if (vk::enumerateInstanceLayerProperties(&layer_count, available_layers.data()) != vk::Result::eSuccess)
		{
			std::cout << "Failed to enumerate instance layer properties!\n";
		}



		for (const char* layer_name : validation_layers)
		{
		    bool layer_found = false;

		    for (const auto& layer_properties : available_layers)
		    {
		        if (strcmp(layer_name, layer_properties.layerName) == 0)
		        {
		            layer_found = true;
		            break;
		        }
		    }

		    if (!layer_found)
		    {
		        return false;
		    }
		}

		return true;
	}


	void create_instance(Window *window)
	{	
		// Application info
		vk::ApplicationInfo app_info{};
		app_info.sType = vk::StructureType::eApplicationInfo;
		app_info.pApplicationName = "App name";
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.pEngineName = "Jenesis";
		app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion = VK_API_VERSION_1_0;


		// Instance Create Info
		vk::InstanceCreateInfo create_info {};
		create_info.sType = vk::StructureType::eInstanceCreateInfo;
		create_info.pApplicationInfo = &app_info;
		create_info.enabledExtensionCount = static_cast<uint32_t>(window->sdl_extensions.size());
		create_info.ppEnabledExtensionNames = window->sdl_extensions.data();
		create_info.enabledLayerCount = 0;



		uint32_t extension_count = 0;
		if (vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr) != vk::Result::eSuccess)
		{
			std::cout << "Failed to enumerate Extension layer properties!\n";
		}
		std::vector<vk::ExtensionProperties> extensions(extension_count);
		if (vk::enumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data()) != vk::Result::eSuccess)
		{
			std::cout << "Failed to enumerate Extension layer properties!\n";
		}

		std::cout << "available extensions:\n";
		for (const auto& extension : extensions)
		{
		    std::cout << '\t' << extension.extensionName << '\n';
		}



	    if (enable_validation_layers && !check_validation_layer_support())
	    {
	        throw std::runtime_error("validation layers requested, but not available!");
	    }



		if (enable_validation_layers)
		{
		    create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		    create_info.ppEnabledLayerNames = validation_layers.data();
		}
		else
		{
		    create_info.enabledLayerCount = 0;
		}




		// Now we can make the Vulkan instance
		try
		{
			std::cout << "\ncreating Instance...\n";
			instance = vk::createInstance(create_info, nullptr);
		}
		catch (std::exception err)
		{
			std::cout << "Failed to create Instance!\n";
		}
	}
}