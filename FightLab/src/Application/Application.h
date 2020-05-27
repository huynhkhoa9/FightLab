#pragma once
#include "Window.h"
#include "Input/InputManager.h"
#include "Core/EventSystem/EventSystem.h"
#include "Graphics/RenderBackEnd/RenderManager.h"

class Application
{
public:
	Application();
	~Application();

	void Init();

	void Run();

	void CleanUp();

	void SetAppState(ChangeAppStateEvent* event);

private:
	ApplicationState m_appState = ApplicationState::RUNNING;
	Window* m_mainWindow;
	InputManager* m_inputManager;
	EventBus* m_eventBus;
	RenderManager m_renderManager;

	int frames = 0;
	void limitFrameRate();

	std::chrono::duration<double, std::milli> targetFrameTime = std::chrono::duration<double, std::milli>(1000/62);
	std::chrono::steady_clock::time_point currentTime;
	std::chrono::steady_clock::time_point lastTime;
	std::chrono::duration<double> diff;
};

