
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>

#include "Renderer.hpp"
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
			renderer(name)
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

		void initVulkan()
		{
			std::vector<VkExtensionProperties> extensions = renderer.GetAvailableExtensions();

			std::cout << "Available : extensions :" << std::endl;

			for (const auto& extension : extensions)
			{
				std::cout << "\t\t * " << extension.extensionName << std::endl;
			}
		}

		void mainLoop()
		{
			while (window.IsOpen())
			{
				window.PollEvent();
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
