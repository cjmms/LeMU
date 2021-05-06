#include "window.hpp"
#include <stdexcept>

namespace LeMU
{
	Window::Window(int w, int h, std::string name)
		: width(h), height(h), name(name) 
	{
		InitWindow();
	}


	Window::~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	
	void Window::InitWindow()
	{
		glfwInit();

		// Do not create OpenGL content when GLFW get init
		// since this application is using Vulkan
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		// Dsiable GLFW default resize
		// gonna deal with resize later in other way
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	}



	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(window);
	}


	void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface");
		}


	}


}