#include "pch.h"
#include "Application.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	glfwInit();
	m_mainWindow = new Window();
	m_inputManager = new InputManager();
}

void Application::Run()
{
	while (m_appState != ApplicationState::QUIT)
	{
		//m_inputManager->ProcessInput();
		glfwPollEvents();
		
		if(glfwWindowShouldClose(m_mainWindow->window))
			m_appState = ApplicationState::QUIT;

		if (glfwGetKey(m_mainWindow->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			m_appState = ApplicationState::QUIT;

		limitFrameRate();
	}
}

void Application::CleanUp()
{
	delete m_inputManager;
	delete m_mainWindow;
}

void Application::limitFrameRate()
{
	currentTime = std::chrono::high_resolution_clock::now();

	diff = currentTime - lastTime;

	std::chrono::duration<double, std::milli> sleepTime = targetFrameTime - diff;
	std::this_thread::sleep_for(sleepTime);
	lastTime = std::chrono::high_resolution_clock::now();
	diff = lastTime - currentTime;
	m_mainWindow->showFPS(1 / diff.count());
}
