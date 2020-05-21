#pragma once
#include "pch.h"
#include <sstream>

struct Window
{
	Window()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
	}

	~Window()
	{
		glfwDestroyWindow(window);
	}

	GLFWwindow* window;

    void showFPS(double fps)
    {
        std::stringstream ss;
        ss << "Vulkan" << " " << "Dev" << " [" << ceil(fps) << " FPS]";

        glfwSetWindowTitle(window, ss.str().c_str());
    }
};