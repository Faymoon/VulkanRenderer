
#include <vector>
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Renderer.hpp"


Renderer::Renderer(const std::string& name)
{
#ifndef NDEBUG
	std::vector<const char*> validationLayers{ "VK_LAYER_LUNARG_standard_validation" };

	if (!CheckValidationLayersSupport(validationLayers))
		throw std::runtime_error("Not Supported : Layers");
#endif // !NDEBUG


	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = name.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions = GetRequiredExtensions();
	
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	createInfo.enabledLayerCount = 0;

#ifndef NDEBUG
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#endif // !NDEBUG

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("Failed : VkInstance creation");

#ifndef NDEBUG
	SetupDebugCallback();
#endif // !NDEBUG

}

Renderer::~Renderer()
{
#ifndef NDEBUG
	auto func = GetExtensionFunc<PFN_vkDestroyDebugReportCallbackEXT>("vkDestroyDebugReportCallbackEXT");
	func(instance, callback, nullptr);
#endif // !NDEBUG

	vkDestroyInstance(instance, nullptr);
}

std::vector<const char*> Renderer::GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensionNames;
	glfwExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensionNames, glfwExtensionNames + glfwExtensionCount);

#ifndef NDEBUG
	extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif // !NDEBUG

	return extensions;
}

std::vector<VkExtensionProperties> Renderer::GetAvailableExtensions()
{
	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	return extensions;
}

#ifndef NDEBUG
bool Renderer::CheckValidationLayersSupport(const std::vector<const char*> requestedLayers)
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	for (auto requestedLayer : requestedLayers)
	{
		bool found = false;

		for (auto& layer : layers)
		{
			if (strcmp(requestedLayer, layer.layerName))
				found = true;
		}

		if (!found)
			return false;
	}

	return true;
}

void Renderer::SetupDebugCallback()
{
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	auto func = GetExtensionFunc<PFN_vkCreateDebugReportCallbackEXT>("vkCreateDebugReportCallbackEXT");
	if (func(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
		throw std::runtime_error("Failed : Debug callback creation");
}

VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData)
{
	std::cerr << layerPrefix << " : " << msg << std::endl;

	return VK_FALSE;
}
#endif // !NDEBUG
