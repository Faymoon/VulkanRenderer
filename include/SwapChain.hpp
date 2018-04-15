#pragma once

#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include <Surface.hpp>
#include <Device.hpp>

class SwapChain
{
	friend class Device;
	public:
		SwapChain() = delete;
		SwapChain(Surface& surface, Device& device);
		SwapChain(const SwapChain&) = delete;
		SwapChain(SwapChain&&) = default;
		~SwapChain();

	private:

		struct AvailableDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> modes;
		};

		static AvailableDetails QueryAvailableDetails(VkPhysicalDevice device, Surface& surface);

		Surface& m_surface;
		Device& m_device;
};

#endif // !SWAPCHAIN_HPP
