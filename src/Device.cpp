#include "Device.hpp"

Device::Device(Renderer* renderer)
	:
	m_renderer(renderer),
	m_physicalDevice(VK_NULL_HANDLE)
{
#ifndef NDEBUG
	std::vector<const char*> validationLayers{ "VK_LAYER_LUNARG_standard_validation" };

	if (!m_renderer->CheckValidationLayersSupport(validationLayers))
		throw std::runtime_error("Not Supported : Layers");
#endif // !NDEBUG

	uint32_t count;
	vkEnumeratePhysicalDevices(m_renderer->m_instance, &count, nullptr);
	if (!count)
		throw std::runtime_error("Not Supported : Physical device");

	std::vector<VkPhysicalDevice> physicalDevices;
	vkEnumeratePhysicalDevices(m_renderer->m_instance, &count, physicalDevices.data());

	uint32_t queueFamily = -1;

	for (auto& device : physicalDevices)
	{
		if (IsDeviceSuitable(device))
		{
			m_physicalDevice = device;

			queueFamily = PickQueueFamily();
			if (queueFamily >= 0)
				break;
			else
				m_physicalDevice = VK_NULL_HANDLE;
		}
	}

	if (queueFamily <= 0)
		throw std::runtime_error("Not Supported : Queue");

	VkDeviceQueueCreateInfo queueInfo;
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.queueFamilyIndex = queueFamily;
	queueInfo.queueCount = 1;
	float queuePriority = 1.f;
	queueInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures features;

	VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pQueueCreateInfos = &queueInfo;
	deviceInfo.pEnabledFeatures = &features;
	deviceInfo.enabledLayerCount = 0;

#ifndef NDEBUG
	deviceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	deviceInfo.ppEnabledLayerNames = validationLayers.data();
#endif // !NDEBUG

	if (vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device) != VK_SUCCESS)
		throw std::runtime_error("Failed : Device creation");
}

Device::~Device()
{
	vkDestroyDevice(m_device, nullptr);
}

bool Device::IsDeviceSuitable(VkPhysicalDevice device)
{
	return true;

	/*VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(m_physicalDevice, &features);

	return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && features.geometryShader;*/
}
