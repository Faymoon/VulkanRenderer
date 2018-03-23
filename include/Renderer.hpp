
#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <vulkan/vulkan.h>

class Renderer
{
	public:
		Renderer(const std::string& name);

		Renderer() = delete;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = default;
		~Renderer();

		std::vector<VkExtensionProperties> GetAvailableExtensions();// TO DELETE

	private:
		VkInstance instance;
};

#endif // !RENDERER_HPP
