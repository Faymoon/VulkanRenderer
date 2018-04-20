#include "Window.hpp"

#include <iostream>

Window::Window(const std::string& name, unsigned int width, unsigned int height)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

	glfwDefaultWindowHints();
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Window::IsOpen()
{
	return !glfwWindowShouldClose(m_window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

GLFWwindow* Window::GetHandle()
{
	return m_window;
}
