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
			window(std::make_shared<Window>(name, WIDTH, HEIGHT)),
			renderer(std::make_shared<Renderer>(name)),
			surface(std::make_shared<Surface>(window, renderer)),
			device(std::make_shared<Device>(renderer, surface)),
			swapchain(std::make_shared<SwapChain>(surface, device, WIDTH, HEIGHT))
		{}

		void run()
		{
			initVulkan();
			mainLoop();
		}

	private:
		std::string name;

		std::shared_ptr<Window> window;

		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<Surface> surface;
		std::shared_ptr<Device> device;
		std::shared_ptr<SwapChain> swapchain;

		void initVulkan()
		{
			std::cout << "Vulkan Created" << std::endl;
		}

		void mainLoop()
		{
			while (window->IsOpen())
			{
				window->PollEvents();
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
