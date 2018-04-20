#pragma once

#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "Surface.hpp"
#include "Device.hpp"

#include <memory>

class SwapChain
{
	friend class Device;
	public:
		SwapChain() = delete;
		SwapChain(std::shared_ptr<Surface> surface, std::shared_ptr<Device> device, unsigned int baseWidth, unsigned int baseHeight);
		SwapChain(const SwapChain&) = delete;
		SwapChain(SwapChain&&) = default;
		~SwapChain();

	private:

		VkSwapchainKHR GetHandle();

		struct AvailableDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> modes;
		};

		static AvailableDetails QueryAvailableDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

		VkSurfaceFormatKHR PickSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR PickPresentMode(const std::vector<VkPresentModeKHR>& modes);
		VkExtent2D PickExtent(const VkSurfaceCapabilitiesKHR& capabilities, unsigned int baseWidth, unsigned int baseHeight);

		std::shared_ptr<Surface> m_surface;
		std::shared_ptr<Device> m_device;

		VkExtent2D m_extent;
		VkFormat m_format;

		std::vector<VkImage> m_images;
		std::vector<VkImageView> m_imageViews;

		VkSwapchainKHR m_swapchain;
};

#endif // !SWAPCHAIN_HPP
