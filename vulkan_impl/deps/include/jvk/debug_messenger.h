
#pragma once

#include <iostream>



namespace jvk
{
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::cerr << "validation ERRRRRR: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}


	void create_debug_messenger()
	{
		vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT;
		createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
		createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr; // Optional

		try
		{
			debug_messenger = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
		}
		catch (vk::SystemError err)
		{
			std::cout << "Failed to setup Debug Messenger!\n";
		}
	}
}