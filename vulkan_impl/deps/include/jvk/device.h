
#pragma once

#include <iostream>
#include <optional>
#include <map>
#include <set>




namespace jvk
{
	struct QueueFamilyIndices
	{
	    std::optional<uint32_t> graphics_family;
	    std::optional<uint32_t> present_family;

	    bool is_complete()
	    {
	        return graphics_family.has_value() && present_family.has_value();
	    }
	};
	QueueFamilyIndices indices;



	
    QueueFamilyIndices find_queue_families(vk::PhysicalDevice &physical_device)
    {
        uint32_t queue_family_count = 0;
        physical_device.getQueueFamilyProperties(&queue_family_count, nullptr);

        std::vector<vk::QueueFamilyProperties> queue_families(queue_family_count);
        physical_device.getQueueFamilyProperties(&queue_family_count, queue_families.data());

		// std::cout << "\n\n\nASDFASDFASDSFASDF\n" << queue_family_count << "  " << "\n\n";


		// for(uint32_t i; i < queue_family_count; i++)
		// {
		// 	if (queue_families[i].queueFlags & vk::QueueFlagBits::eGraphics)
		// 	{
		// 		indices.graphics_family = i;
		// 		break;
		// 	}
		// }

		// vk::Bool32 present_support = false;
		// for(uint32_t i; i < queue_family_count; i++)
		// {
		// 	if (physical_device.getSurfaceSupportKHR(i, surface, &present_support) != vk::Result::eSuccess)
		// 	{
		// 		std::cout << "Failed to get Surface Support!\n";
		// 	}

		// 	if (present_support)
		// 	{
		// 		indices.present_family = i;
		// 		break;
		// 	}
		// }

		indices.graphics_family = 0;
		indices.present_family = 0;


        return indices;
    }


	bool check_device_extension_support(vk::PhysicalDevice &physical_device)
	{
	    uint32_t extension_count;
	    if (physical_device.enumerateDeviceExtensionProperties(nullptr, &extension_count, nullptr) != vk::Result::eSuccess)
	    {
			std::cout << "Failed to get Device Extension Properties!\n";
	    }

	    std::vector<vk::ExtensionProperties> available_extensions(extension_count);
	    if (physical_device.enumerateDeviceExtensionProperties(nullptr, &extension_count, available_extensions.data()) != vk::Result::eSuccess)
	    {
			std::cout << "Failed to get Device Extension Properties!\n";
	    }

	    std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

	    for (const auto& extension : available_extensions)
	    {
	        required_extensions.erase(extension.extensionName);
	    }


	    return required_extensions.empty();
	}


    bool is_device_suitable(vk::PhysicalDevice &physical_device)
    {
        indices = find_queue_families(physical_device);

	    bool extensions_supported = check_device_extension_support(physical_device);

		// std::cout << "\n\n\nASDFASDFASDSFASDF\n" << indices.graphics_family.has_value() << "  " << indices.present_family.has_value() << "\n\n";

        return indices.is_complete() && extensions_supported;
    }






	void pick_physical_device()
	{
		uint32_t device_count = 0;
		if (instance.enumeratePhysicalDevices(&device_count, nullptr) != vk::Result::eSuccess)
		{
			std::cout << "Failed to enumerate Physical devices!\n";
		}

		if (device_count == 0)
		{
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		std::vector<vk::PhysicalDevice> physical_devices(device_count);
		if (instance.enumeratePhysicalDevices(&device_count, physical_devices.data())  != vk::Result::eSuccess)
		{
			std::cout << "Failed to enumerate Physical Devices!\n";
		}


		for (vk::PhysicalDevice device : physical_devices)
		{
		    if (is_device_suitable(device))
		    {
		        physical_device = device;
				vk::PhysicalDeviceProperties properties = physical_device.getProperties();
				std::cout <<  "Device Name: " << properties.deviceName << "\n";

		        break;
		    }
		}

		if (!physical_device)
		{
		    throw std::runtime_error("failed to find a suitable GPU!");
		}
	}


	void create_logical_device()
	{
		std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
		std::set<uint32_t> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

		// std::cout << "\n\n\nASDFASDFASDSFASDF\n" << indices.graphics_family.value() << "  " << indices.present_family.value() << "\n\n";

		float queue_priority = 1.0f;
		for (uint32_t queue_family : unique_queue_families)
		{
			vk::DeviceQueueCreateInfo queue_create_info{};
			queue_create_info.sType = vk::StructureType::eDeviceQueueCreateInfo;
			queue_create_info.queueFamilyIndex = queue_family;
			queue_create_info.queueCount = 1;
			queue_create_info.pQueuePriorities = &queue_priority;
			queue_create_infos.push_back(queue_create_info);
		}

		vk::PhysicalDeviceFeatures device_features{};

		vk::DeviceCreateInfo create_info{};
		create_info.sType = vk::StructureType::eDeviceCreateInfo;
		create_info.pQueueCreateInfos = queue_create_infos.data();
		create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
		create_info.pEnabledFeatures = &device_features;

		create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
		create_info.ppEnabledExtensionNames = device_extensions.data();

		if (enable_validation_layers)
		{
		    create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		    create_info.ppEnabledLayerNames = validation_layers.data();
		}
		else
		{
		    create_info.enabledLayerCount = 0;
		}



		try
		{
			logical_device = physical_device.createDevice(create_info);
		}
		catch (vk::SystemError err)
		{
			throw std::runtime_error("failed to create Logical Device");
		}

		logical_device.getQueue(indices.graphics_family.value(), 0, &graphics_queue);
		logical_device.getQueue(indices.present_family.value(), 0, &present_queue);
	}
}