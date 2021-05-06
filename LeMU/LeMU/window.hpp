#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace LeMU {

class Window {
public:
	Window(int w, int h, std::string name);
	~Window();

	bool ShouldClose();

	// no copy constructor since we are using pointer
	Window(const Window&) = delete;
	// no assignment contructor
	Window& operator=(const Window&) = delete;

private:
	void InitWindow();


	GLFWwindow* window;

	const int width;
	const int height;

	std::string name;


};

}