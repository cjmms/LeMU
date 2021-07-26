#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace LeMU {

	class Window {
	public:
		Window(int w, int h, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		inline bool shouldClose() { return glfwWindowShouldClose(window); }
		inline VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		inline bool wasWindowResized() { return framebufferResized; }
		inline void resetWindowResizeFlag() { framebufferResized = false; }

		inline GLFWwindow* getGLFWwindow() const { return window; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);
		void initWindow();

		int width;
		int height;

		bool framebufferResized = false;

		std::string windowName;
		GLFWwindow* window;
	};
}  