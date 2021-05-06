#include "App.hpp"


namespace LeMU
{
	void FirstApp::run()
	{
		while (!window.ShouldClose())
		{
			glfwPollEvents();
		}
	}



}