#pragma once

#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <vulkan/vulkan.h>
#include <vector>

#include "Renderer.hpp"

class Device
{
	public:
		Device(Renderer* renderer);
		Device(const Device&) = delete;
		Device(Device&&) = default;
		~Device();

	private:
		bool IsDeviceSuitable(VkPhysicalDevice device);

		uint32_t PickQueueFamily();

		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;

		Renderer* m_renderer;
};

#endif//!DEVICE_HPP
