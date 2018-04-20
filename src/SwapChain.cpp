#include "SwapChain.hpp"

#include <algorithm>
#include <limits>
#include <iostream>


SwapChain::SwapChain(std::shared_ptr<Surface> surface, std::shared_ptr<Device> device, unsigned int baseWidth, unsigned int baseHeight)
	:
	m_surface(surface),
	m_device(device)
{
	AvailableDetails details = QueryAvailableDetails(m_device->GetPhysicalHandle(), m_surface->GetHandle());

	VkSurfaceFormatKHR format = PickSurfaceFormat(details.formats);
	m_format = format.format;
	m_extent = PickExtent(details.capabilities, baseWidth, baseWidth);
	VkPresentModeKHR mode = PickPresentMode(details.modes);

	uint32_t imgCount = details.capabilities.minImageCount + 1;
	if (details.capabilities.maxImageCount > 0 && imgCount > details.capabilities.maxImageCount)
		imgCount = details.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface->GetHandle();
	createInfo.minImageCount = imgCount;
	createInfo.imageFormat = format.format;
	createInfo.imageColorSpace = format.colorSpace;
	createInfo.imageExtent = m_extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;

	uint32_t queues[] = { static_cast<uint32_t>(m_device->m_queueIndices.graphics), static_cast<uint32_t>(m_device->m_queueIndices.present) };
	if (queues[0] != queues[1])
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queues;
	}

	createInfo.preTransform = details.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = mode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_device->GetHandle(), &createInfo, nullptr, &m_swapchain) != VK_SUCCESS)
		throw std::runtime_error("Failed : SwapChain creation");

	uint32_t count;
	vkGetSwapchainImagesKHR(m_device->GetHandle(), m_swapchain, &count, nullptr);
	m_images.resize(count);
	vkGetSwapchainImagesKHR(m_device->GetHandle(), m_swapchain, &count, m_images.data());

	m_imageViews.resize(count);

	size_t i = 0;
	for (auto imageView : m_imageViews)
	{
		VkImageViewCreateInfo imageViewInfo{};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = m_images[i];
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = m_format;

		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(m_device->GetHandle(), &imageViewInfo, nullptr, &imageView) != VK_SUCCESS)
			throw std::runtime_error("Failed : Image view creation");

		++i;
	}
}

SwapChain::~SwapChain()
{
	for (auto imageView : m_imageViews)
	{
		vkDestroyImageView(m_device->GetHandle(), imageView, nullptr);
	}

	vkDestroySwapchainKHR(m_device->GetHandle(), m_swapchain, nullptr);
}

VkSwapchainKHR SwapChain::GetHandle()
{
	return m_swapchain;
}

SwapChain::AvailableDetails SwapChain::QueryAvailableDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	AvailableDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);

	if (count == 0)
		return details;

	details.formats.resize(count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, details.formats.data());

	count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);

	if (count == 0)
		return details;

	details.modes.resize(count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, details.modes.data());

	return details;
}

VkSurfaceFormatKHR SwapChain::PickSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
	VkSurfaceFormatKHR idealFormat{};
	idealFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
	idealFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

	if (formats.size() == 1 && formats.front().format == VK_FORMAT_UNDEFINED)
		return idealFormat;

	for (auto& format : formats)
	{
		if (format.format == idealFormat.format && format.colorSpace == idealFormat.colorSpace)
			return idealFormat;
	}

	return formats.front();
}

VkPresentModeKHR SwapChain::PickPresentMode(const std::vector<VkPresentModeKHR>& modes)
{
	if (std::find(modes.begin(), modes.end(), VK_PRESENT_MODE_MAILBOX_KHR) != modes.end())
		return VK_PRESENT_MODE_MAILBOX_KHR;

	if (std::find(modes.begin(), modes.end(), VK_PRESENT_MODE_IMMEDIATE_KHR) != modes.end())
		return VK_PRESENT_MODE_IMMEDIATE_KHR;

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::PickExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int baseWidth, unsigned int baseHeight)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;
	else
	{
		VkExtent2D extent{ baseWidth, baseHeight };

		extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, baseWidth));
		extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, baseHeight));

		return extent;
	}
}

