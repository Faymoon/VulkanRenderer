
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>

#include "Renderer.hpp"
#include "Device.hpp"
#include "Window.hpp"

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 800;

class HelloTriangleApplication
{
	public:
		HelloTriangleApplication(std::string appName)
			:
			name(std::move(appName)),
			window(name, WIDTH, HEIGHT),
			renderer(name),
			surface(window, renderer),
			device(renderer, surface),
			swapchain(surface, device)
		{}

		void run()
		{
			initVulkan();
			mainLoop();
		}

	private:
		std::string name;

		Window window;

		Renderer renderer;
		Surface surface;
		Device device;
		SwapChain swapchain;

		void initVulkan()
		{
			std::cout << "Vulkan Created" << std::endl;
		}

		void mainLoop()
		{
			while (window.IsOpen())
			{
				window.PollEvents();
			}
		}
};

int main()
{
	try
	{
		HelloTriangleApplication app("Vulkan Triangle");

		app.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
