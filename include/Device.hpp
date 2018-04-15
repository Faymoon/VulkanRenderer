#pragma once

#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <vulkan/vulkan.h>

#include <SwapChain.hpp>
#include <Renderer.hpp>
#include <Surface.hpp>

class Device
{
	friend class SwapChain;

	public:
		Device() = delete;
		Device(Renderer& renderer, Surface& surface);
		Device(const Device&) = delete;
		Device(Device&&) = default;
		~Device();

	private:

		struct QueueFamilyIndices
		{
			int graphics = -1;
			int present = -1;

			bool IsComplete();
		};

		bool IsDeviceSuitable(VkPhysicalDevice device);

		QueueFamilyIndices PickQueueFamilies(VkPhysicalDevice device);

		std::vector<const char*> m_extensions;

		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;

		QueueFamilyIndices m_queueIndices;

		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

		Renderer& m_renderer;
		Surface& m_surface;
};

#endif//!DEVICE_HPP
