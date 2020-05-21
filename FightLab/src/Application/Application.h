#pragma once
#include "Window.h"
#include "Input/InputManager.h"

enum class ApplicationState
{
	RUNNING, PAUSED, QUIT, MINIZED
};

class Application
{
public:
	Application();
	~Application();

	void Init();

	void Run();

	void CleanUp();

private:
	ApplicationState m_appState = ApplicationState::RUNNING;
	Window* m_mainWindow;
	InputManager* m_inputManager;

	void limitFrameRate();

	std::chrono::duration<double, std::milli> targetFrameTime = std::chrono::duration<double, std::milli>(1000/62);
	std::chrono::steady_clock::time_point currentTime;
	std::chrono::steady_clock::time_point lastTime;
	std::chrono::duration<double> diff;
};

