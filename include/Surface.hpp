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
		Surface(std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer);
		Surface(const Surface&) = delete;
		Surface(Surface&&) = default;
		~Surface();

	private:

		VkSurfaceKHR GetHandle();

		VkSurfaceKHR m_surface;

		std::shared_ptr<Window> m_window;
		std::shared_ptr<Renderer> m_renderer;
};

#endif // !SURFACE_HPP
