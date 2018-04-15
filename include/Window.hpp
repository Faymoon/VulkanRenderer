#pragma once

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>

#include <string>
#include <functional>

class Window
{
	friend class Surface;

	public:
		Window(const std::string& name, unsigned int width, unsigned int height);

		Window() = delete;
		Window(const Window&) = delete;
		Window(Window&&) = default;
		~Window();

		bool IsOpen();

		void PollEvents();

		struct KeyEvent
		{
			enum Action { RELEASE = GLFW_RELEASE, PRESS = GLFW_PRESS };

			Action action;
			int key;
		};

		void OnKeyEvent(std::function<void(KeyEvent /*event*/)> callback);
		void OnTextEvent();
		void OnMouseMove();
		void OnMouseClick();
		void OnMouseScroll();
		void OnFocusChanged();

	private:
		GLFWwindow* m_window;
};

#endif // !WINDOW_HPP
