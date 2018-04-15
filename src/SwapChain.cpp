#include "SwapChain.hpp"

SwapChain::SwapChain(Surface& surface, Device& device)
	:
	m_surface(surface),
	m_device(device)
{
	AvailableDetails details = QueryAvailableDetails(m_device.m_physicalDevice, surface);
}

SwapChain::~SwapChain()
{
}

SwapChain::AvailableDetails SwapChain::QueryAvailableDetails(VkPhysicalDevice device, Surface& surface)
{
	AvailableDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface.m_surface, &details.capabilities);

	uint32_t count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.m_surface, &count, nullptr);

	if (count == 0)
		return details;

	details.formats.resize(count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.m_surface, &count, details.formats.data());

	count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.m_surface, &count, nullptr);

	if (count == 0)
		return details;

	details.modes.resize(count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.m_surface, &count, details.modes.data());

	return details;
}
