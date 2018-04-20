#include "Surface.hpp"


Surface::Surface(std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer)
	:
	m_window(window),
	m_renderer(renderer)
{
	if (glfwCreateWindowSurface(m_renderer->GetHandle(), m_window->GetHandle(), nullptr, &m_surface) != VK_SUCCESS)
		throw std::runtime_error("Failed : Surface creation");
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(m_renderer->GetHandle(), m_surface, nullptr);
}

VkSurfaceKHR Surface::GetHandle()
{
	return m_surface;
}
