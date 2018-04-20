
#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <vector>
#include <memory>

#ifdef _WIN64
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__linux)
#define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <vulkan/vulkan.h>


class Renderer
{
	friend class Device;
	friend class Surface;

	public:
		Renderer() = delete;
		Renderer(const std::string& name);
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = default;
		~Renderer();

	private:

		VkInstance GetHandle();

		std::vector<const char*> GetRequiredExtensions();

		VkInstance m_instance;

#ifndef NDEBUG
		bool CheckValidationLayersSupport();

		void SetupDebugCallback();

		VkDebugReportCallbackEXT m_callback;

		std::vector<const char*> m_validationLayers;
#endif // !NDEBUG
};

#include "Renderer.inl"

#endif // !RENDERER_HPP
