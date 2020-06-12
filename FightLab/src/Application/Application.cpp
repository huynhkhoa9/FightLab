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
	m_eventBus = new EventBus();

	m_inputManager->Init(m_eventBus, m_mainWindow->window);
	m_eventBus->subscribe(this, &Application::SetAppState);

	m_renderManager.window = (m_mainWindow->window);
	m_renderManager.Initialize();
}

void Application::Run()
{
	lastTime = std::chrono::high_resolution_clock::now();
	while (m_appState != ApplicationState::QUIT)
	{
		m_inputManager->ProcessInput();
		m_renderManager.Draw();

		if (glfwWindowShouldClose(m_mainWindow->window))
			m_appState = ApplicationState::QUIT;
		
		limitFrameRate();
	}
}

void Application::CleanUp()
{
	m_renderManager.CleanUp();
	delete m_eventBus;
	delete m_inputManager;
	delete m_mainWindow;
}

void Application::SetAppState(ChangeAppStateEvent* event)
{
	m_appState = event->m_state;
}

void Application::limitFrameRate()
{
	currentTime = std::chrono::high_resolution_clock::now();

	diff = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime);
	if (diff < targetFrameTime)
	{
		std::chrono::duration<double, std::milli> sleepTime = targetFrameTime - diff;
		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(sleepTime));
	}
	lastTime = std::chrono::high_resolution_clock::now();
	diff = lastTime - currentTime;
	
	m_mainWindow->showFPS(diff.count());
}
