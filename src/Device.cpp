#include "Device.hpp"

#include <vector>
#include <iostream>
#include <algorithm>

Device::Device(Renderer& renderer, Surface& surface)
	:
	m_renderer(renderer),
	m_surface(surface),
	m_physicalDevice(VK_NULL_HANDLE),
	m_device(VK_NULL_HANDLE),
	m_extensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME }
{
	uint32_t count;
	vkEnumeratePhysicalDevices(m_renderer.m_instance, &count, nullptr);
	if (!count)
		throw std::runtime_error("Not Supported : Physical device");

	std::vector<VkPhysicalDevice> physicalDevices(count);
	vkEnumeratePhysicalDevices(m_renderer.m_instance, &count, physicalDevices.data());
	
	SwapChain::AvailableDetails availableDetails;

	for (auto& device : physicalDevices)
	{
		if (IsDeviceSuitable(device))
		{
			m_queueIndices = PickQueueFamilies(device);
			if (m_queueIndices.IsComplete())
			{
				availableDetails = SwapChain::QueryAvailableDetails(device, m_surface);
				if (!availableDetails.formats.empty() && !availableDetails.modes.empty())
				{
					m_physicalDevice = device;
					break;
				}
			}
		}
	}

	if (m_physicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("Not Suitable : GPU");

	std::vector<int> queues{ m_queueIndices.graphics, m_queueIndices.present };
	queues.erase(std::unique(queues.begin(), queues.end()), queues.end());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	float queuePriority = 1.f;
	for (int queue : queues)
	{
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queue;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueInfo);
	}

	VkPhysicalDeviceFeatures features{};

	VkDeviceCreateInfo deviceInfo{};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceInfo.pEnabledFeatures = &features;

	deviceInfo.enabledExtensionCount = static_cast<uint32_t>(m_extensions.size());
	deviceInfo.ppEnabledExtensionNames = m_extensions.data();

	deviceInfo.enabledLayerCount = 0;
#ifndef NDEBUG
	deviceInfo.enabledLayerCount = static_cast<uint32_t>(m_renderer.m_validationLayers.size());
	deviceInfo.ppEnabledLayerNames = m_renderer.m_validationLayers.data();
#endif // !NDEBUG

	if (vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device) != VK_SUCCESS)
		throw std::runtime_error("Failed : Device creation");

	vkGetDeviceQueue(m_device, m_queueIndices.graphics, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, m_queueIndices.present, 0, &m_presentQueue);
}

Device::~Device()
{
	vkDestroyDevice(m_device, nullptr);
}

bool Device::IsDeviceSuitable(VkPhysicalDevice device)
{
	uint32_t extCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, nullptr);

	std::vector<VkExtensionProperties> extProperties(extCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, extProperties.data());

	for (auto extension : m_extensions)
	{
		bool found = false;

		for (auto& extProperty : extProperties)
		{
			if (strcmp(extension, extProperty.extensionName))
				found = true;
		}
		
		if (!found)
			return false;
	}

	return true;

	/*VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(m_physicalDevice, &features);

	return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;*/
}

Device::QueueFamilyIndices Device::PickQueueFamilies(VkPhysicalDevice device)
{
	uint32_t count;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

	std::vector<VkQueueFamilyProperties> properties(count);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &count, properties.data());

	QueueFamilyIndices queueIndices;

	int i = 0;
	for (auto& property : properties)
	{
		if (property.queueCount > 0 && property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			queueIndices.graphics = i;

		VkBool32 presentSupport;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface.m_surface, &presentSupport);

		if (property.queueCount > 0 && presentSupport)
			queueIndices.present = i;

		if (queueIndices.IsComplete())
			break;
	}
	return queueIndices;
}

bool Device::QueueFamilyIndices::IsComplete()
{
	return graphics >= 0 && present >= 0;
}
