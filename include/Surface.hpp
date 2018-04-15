#pragma once

#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "Renderer.hpp"
#include "Window.hpp"


class Surface
{
	friend class Device;
	friend class SwapChain;

	public:
		Surface() = delete;
		Surface(Window& window, Renderer& renderer);
		Surface(const Surface&) = delete;
		Surface(Surface&&) = default;
		~Surface();

	private:

		VkSurfaceKHR m_surface;

		Window& m_window;
		Renderer& m_renderer;
};

#endif // !SURFACE_HPP
