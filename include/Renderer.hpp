
#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <vulkan/vulkan.h>

class Renderer
{
	friend class Device;

	public:
		Renderer(const std::string& name);

		Renderer() = delete;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = default;
		~Renderer();

	private:

		template<typename Func>
		Func GetExtensionFunc(const std::string& name);
		std::vector<const char*> GetRequiredExtensions();

		VkInstance m_instance;

#ifndef NDEBUG
		bool CheckValidationLayersSupport(const std::vector<const char*> requestedLayers);

		void SetupDebugCallback();

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char* layerPrefix,
			const char* msg,
			void* userData);

		VkDebugReportCallbackEXT m_callback;
#endif // !NDEBUG
};

#include "Renderer.inl"

#endif // !RENDERER_HPP
