#pragma once
#include "pch.h"
#include <sstream>

struct Window
{
	Window()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
	}

	~Window()
	{
		glfwDestroyWindow(window);
	}

	GLFWwindow* window;

    void showFPS(double frameTime)
    {
        std::stringstream ss;
        ss << "Vulkan" << " " << "Dev" << " [" << frameTime << " ms][" << (uint16_t)floor(1/ frameTime) << "FPS]";

        glfwSetWindowTitle(window, ss.str().c_str());
    }
};