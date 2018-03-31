#pragma once

#ifndef WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class Window
{
	struct Event;

	public:
		Window(const std::string& name, unsigned int width, unsigned int height);

		Window() = delete;
		Window(const Window&) = delete;
		Window(Window&&) = default;
		~Window();

		bool IsOpen();

		Event PollEvent();

		struct Event {};

	private:
		GLFWwindow* m_window;
};

#endif // !WINDOW_HPP
