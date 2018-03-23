#include "Window.hpp"

Window::Window(const std::string & name, unsigned int width, unsigned int height)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::IsOpen()
{
	return !glfwWindowShouldClose(window);
}

Window::Event Window::PollEvent()
{
	glfwPollEvents();

	return Event();
}
