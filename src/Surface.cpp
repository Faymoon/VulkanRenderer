#include "Surface.hpp"


Surface::Surface(Window& window, Renderer& renderer)
	:
	m_window(window),
	m_renderer(renderer)
{
	if (glfwCreateWindowSurface(m_renderer.m_instance, m_window.m_window, nullptr, &m_surface) != VK_SUCCESS)
		throw std::runtime_error("Failed : Surface creation");
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(m_renderer.m_instance, m_surface, nullptr);
}
