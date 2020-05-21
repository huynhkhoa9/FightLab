#pragma once
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
};

