
#include <vector>
#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Renderer.hpp"


Renderer::Renderer(const std::string& name)
#ifndef NDEBUG
	:
	m_validationLayers{ "VK_LAYER_LUNARG_standard_validation" }
#endif // !NDEBUG
{
#ifndef NDEBUG
	if (!CheckValidationLayersSupport())
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
	createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
	createInfo.ppEnabledLayerNames = m_validationLayers.data();
#endif // !NDEBUG

	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
		throw std::runtime_error("Failed : VkInstance creation");

#ifndef NDEBUG
	SetupDebugCallback();
#endif // !NDEBUG

}

Renderer::~Renderer()
{
#ifndef NDEBUG
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
	func(m_instance, m_callback, nullptr);
#endif // !NDEBUG

	vkDestroyInstance(m_instance, nullptr);
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

#ifndef NDEBUG
bool Renderer::CheckValidationLayersSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	for (auto requestedLayer : m_validationLayers)
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

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
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

void Renderer::SetupDebugCallback()
{
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");
	if (!func ||func(m_instance, &createInfo, nullptr, &m_callback) != VK_SUCCESS)
		throw std::runtime_error("Failed : Debug callback creation");
}
#endif // !NDEBUG
